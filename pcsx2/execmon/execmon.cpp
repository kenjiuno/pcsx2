// kkdf2--
#include "PrecompiledHeader.h"
#include "System.h"
#include "Memory.h"
#include "Hw.h"
#include "R5900.h"

#include "ElfHeader.h"
#include "AppConfig.h"

#include "VU.h"

#include "./execmon.h"

namespace execmon
{
u32 currentPc = 0;

u32 s_mypy_opc = 0;
PyThreadState *s_pyTState = NULL;

namespace utils
{
struct KeyGen
{
    int next = 1;

    int alloc()
    {
        int ret = next;
        ++next;
        return ret;
    }

    void reset()
    {
        next = 1;
    }
} keyGen;

} // namespace utils

namespace exitstatus
{
bool s_newbp;
bool s_newpc;
u32 s_mypy_new_pc;

void __cdecl clear()
{
    s_newbp = false;
    s_newpc = false;
}
int eject()
{
    return (s_newpc ? 2 : 0) | (s_newbp ? 1 : 0);
}

} // namespace exitstatus

utils::UniquePyObject onSuspendCallable;
utils::UniquePyObject onResumeCallable;
utils::UniquePyObject onFlushRWTrace;

namespace brk
{
Items items;
int add(u32 pc, PyObject *callable)
{
    pc &= ~3UL;

    if (Cpu != NULL) {
        Cpu->Clear(pc, 4);
    }

    Items::iterator iter = items.insert(std::make_pair(pc, Item()));
    iter->second.key = utils::keyGen.alloc();
    iter->second.callable.reset(callable);
    Py_XINCREF(callable);

    return iter->second.key;
}

} // namespace brk

namespace rbrk
{
brkCommon::Items items;
int add(u32 addr, u32 size, PyObject *callable)
{
    Items::iterator iter = items.insert(items.end(), Item());
    iter->key = utils::keyGen.alloc();
    iter->callable.reset(callable);
    Py_XINCREF(callable);

    return iter->key;
}
bool remove(int key)
{
    Items::iterator iter = items.begin();
    bool any = false;
    for (; iter != items.end();) {
        if (iter->key == key) {
            iter = items.erase(iter);
            any |= true;
        } else {
            iter++;
        }
    }
    return any;
}
} // namespace rbrk

namespace wbrk
{
brkCommon::Items items;
int add(u32 addr, u32 size, PyObject *callable)
{
    Items::iterator iter = items.insert(items.end(), Item());
    iter->key = utils::keyGen.alloc();
    iter->callable.reset(callable);
    Py_XINCREF(callable);

    return iter->key;
}
bool remove(int key)
{
    Items::iterator iter = items.begin();
    bool any = false;
    for (; iter != items.end();) {
        if (iter->key == key) {
            iter = items.erase(iter);
            any |= true;
        } else {
            iter++;
        }
    }
    return any;
}

} // namespace wbrk

EETrace eetrace;

bool EETrace::MypyWriteEETrace(int mask)
{
    if (s_feet == NULL) {
        return true; // not error
    }

    if (mask & 1) {
        static char header[1024] = {"trac2\n\x1a\0"};
        if (1 != fwrite(header, 1024, 1, s_feet)) {
            return false;
        }
    }

    if (1 != fwrite("$$$$", 4, 1, s_feet)) {
        return false;
    }

    class LocalWriter
    {
        // 32bytes(256 flags) +diffBuff
        u8 buff[4096];
        int pos;
        bool overflow;

    public:
        LocalWriter()
        {
            Reset();
        }

        void Reset()
        {
            memset(buff, 0, 32);
            pos = 32;
            overflow = false;
        }

        void WriteAlways(const void *src, int size)
        {
            if (u32(pos + size) < u32(sizeof(buff)) && !overflow) {
                memcpy(buff + pos, src, size);
                pos += size;
            } else {
                overflow |= true;
            }
        }

        bool WriteTo(FILE *file)
        {
            return !overflow && 1 == fwrite(buff, pos, 1, file);
        }

        void WriteConditional(bool always, const void *latestVal, int size, void *prevAndUpdatableVal, int flagNum)
        {
            if (always || memcmp(latestVal, prevAndUpdatableVal, size) != 0) {
                buff[flagNum / 8] |= 1 << (flagNum & 7);

                memcpy(prevAndUpdatableVal, latestVal, size);
                WriteAlways(latestVal, size);
            }
        }
    };

    static cpuRegisters prevCpuRegs;
    static fpuRegisters prevFpuRegs;
    static VURegs prevVuRegs[2];

    {
        static LocalWriter writer;
        writer.Reset();
        writer.WriteAlways(&s_tracpos, 4);
        writer.WriteAlways(&cpuRegs.pc, 4);
        for (int x = 0; x < 32; x++) {
            writer.WriteConditional(mask, &cpuRegs.GPR.r[x], 16, &prevCpuRegs.GPR.r[x], 0 + x);
        }
        for (int x = 0; x < 32; x++) {
            writer.WriteConditional(mask, &fpuRegs.fpr[x], 4, &prevFpuRegs.fpr[x], 32 + x);
        }
        for (int x = 0; x < 32; x++) {
            writer.WriteConditional(mask, &fpuRegs.fprc[x], 4, &prevFpuRegs.fprc[x], 64 + x);
        }
        writer.WriteConditional(mask, &fpuRegs.ACC, 4, &prevFpuRegs.ACC, 96);
        for (int x = 0; x < 32; x++) {
            writer.WriteConditional(mask, &vuRegs[0].VF[x], 4 * 4, &prevVuRegs[0].VF[x], 128 + x);
        }
        for (int x = 0; x < 32; x++) {
            writer.WriteConditional(mask, &vuRegs[0].VI[x], 4, &prevVuRegs[0].VI[x], 160 + x);
        }
        writer.WriteConditional(mask, &vuRegs[0].ACC, 16, &prevVuRegs[0].ACC, 192);
        writer.WriteConditional(mask, &vuRegs[0].q, 4, &prevVuRegs[0].q, 193);
        writer.WriteConditional(mask, &vuRegs[0].p, 4, &prevVuRegs[0].p, 194);

        static const int zero = 0;
        writer.WriteAlways(&zero, 4);

        if (!writer.WriteTo(s_feet)) {
            return false;
        }
    }

    const int VTLB_PAGE_SIZE = 4096;
    const int VTLB_PAGE_MASK = 4095;
    const size_t RamSize = 32 * 1024 * 1024;

    {
        static u8 prevRamData[RamSize];

        if (mask & 1) {
            static const u32 pair[2] = {0, 1024 * 1024 * 32};
            if (1 != fwrite(&pair[0], 8, 1, s_feet)) {
                return false;
            }
            for (u32 x = pair[0], cx = pair[1]; x < cx; x += VTLB_PAGE_SIZE) {
                const void *pSrc = PSM(x);
                if (1 != fwrite(pSrc, VTLB_PAGE_SIZE, 1, s_feet)) {
                    return false;
                }
                memcpy(prevRamData + x, pSrc, VTLB_PAGE_SIZE);
            }
        } else if (false && false) {
            class PsmHelper
            {
            private:
                const u8 *table;
                u32 selector;

            public:
                PsmHelper()
                    : selector(-1)
                {
                }

                inline u8 ReadByteAt(u32 adr)
                {
                    if (selector != (adr & (~VTLB_PAGE_MASK))) {
                        selector = (adr & (~VTLB_PAGE_MASK));
                        table = reinterpret_cast<const u8 *>(PSM(selector));
                    }
                    return table[adr & VTLB_PAGE_MASK];
                }
            } psmHelper;

            for (u32 x = 0; x < RamSize; x++) {
                u8 readByte;
                if ((readByte = psmHelper.ReadByteAt(x)) != prevRamData[x]) {
                    prevRamData[x] = readByte;
                    u32 scan = x + 1;
                    while (scan < RamSize && (readByte = psmHelper.ReadByteAt(scan)) != prevRamData[scan]) {
                        prevRamData[scan] = readByte;
                        ++scan;
                    }

                    u32 pair[2] = {x, scan - x};
                    if (1 != fwrite(pair, 8, 1, s_feet)) {
                        return false;
                    }
                    if (1 != fwrite(prevRamData + pair[0], pair[1], 1, s_feet)) {
                        return false;
                    }

                    x = scan;
                }
            }
        }
    }

    {
        static const u8 terminator[8] = {0};

        if (1 != fwrite(&terminator[0], 8, 1, s_feet)) {
            return false;
        }
    }

    s_tracpos++;
    return true;
}

namespace py
{
void mypyPrintErr()
{
    PyObject *pyT, *pyV, *pyTB;
    PyErr_Fetch(&pyT, &pyV, &pyTB);
    //Console.WriteLn( L"%p, %p, %p", pyT, pyV, pyTB );
    if (pyV != NULL) {
        PyObject *pySt = PyObject_Str(pyT);
        PyObject *pySv = PyObject_Str(pyV);
        PyObject *pyStb = PyObject_Str(pyTB);
        Console.Error(L"%s: %s\n%s", PyUnicode_AS_UNICODE(pySt), PyUnicode_AS_UNICODE(pySv), PyUnicode_AS_UNICODE(pyStb));
        Py_CLEAR(pySt);
        Py_CLEAR(pySv);
        Py_CLEAR(pyStb);
    }
    Py_CLEAR(pyT);
    Py_CLEAR(pyV);
    Py_CLEAR(pyTB);
}

void mypyEnsureMod(wxString pyFile)
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    Console.WriteLn(L"(execmon) Ensure module: %s ", WX_STR(pyFile));
    PyObject *moduleDict = PyImport_GetModuleDict();
    if (moduleDict != NULL) {
        wxASSERT(PyDict_CheckExact(moduleDict));
        PyObject *loadedModule = PyDict_GetItemString(moduleDict, pyFile.mb_str());
        if (loadedModule != NULL) {
            wxASSERT(PyModule_CheckExact(loadedModule));
            utils::UniquePyObject reloadedModule(PyImport_ReloadModule(loadedModule));
            if (reloadedModule.get() == NULL) {
                mypyPrintErr();
            }
        } else {
            utils::UniquePyObject reloadedModule(PyImport_ImportModule(pyFile.mb_str()));
            if (reloadedModule.get() == NULL) {
                mypyPrintErr();
            }
        }
    } else {
        mypyPrintErr();
    }

    PyGILState_Release(gstate);
}

void mypyClearClient()
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    utils::keyGen.reset();

    brk::items.clear();
    rbrk::items.clear();
    wbrk::items.clear();

    onSuspendCallable.release();
    onResumeCallable.release();

    eetrace.close();

    rwtrace.reset();
    onFlushRWTrace.release();

    PyGILState_Release(gstate);
}

bool mypyFlushRWTrace()
{
    if (rwtrace.s_rwTraceIdx == 0) {
        return true;
    }
    if (Py_None == onFlushRWTrace.get()) {
        rwtrace.rewindBuff();
        return true;
    }

    wxASSERT(PyCallable_Check(onFlushRWTrace.get()));

    utils::UniquePyObject bytes(PyBytes_FromStringAndSize((const char *)rwtrace.buffPtr(), rwtrace.buffSize()));
    if (bytes.get() != NULL) {
        utils::UniquePyObject result(PyObject_CallFunction(onFlushRWTrace.get(), "(O)", bytes.get()));
        if (result.get() != NULL) {
            rwtrace.rewindBuff();
            return true;
        }
    }

    mypyPrintErr();

    rwtrace.reset();
    return false;
}

bool lastSuspended = true;

void movetoSuspend()
{
    if (lastSuspended)
        return;
    lastSuspended = true;

    PyGILState_STATE gstate = PyGILState_Ensure();

    if (PyCallable_Check(onSuspendCallable.get())) {
        PyObject *result = PyObject_CallFunction(onSuspendCallable.get(), NULL);
        Py_CLEAR(result);
        if (result == NULL) {
            mypyPrintErr();
        }
    }

    PyGILState_Release(gstate);
}

void movetoResume()
{
    if (!lastSuspended)
        return;
    lastSuspended = false;

    mypyClearClient();
    mypyEnsureMod(wxsFormat(L"monitor.%08X", ElfCRC));

    PyGILState_STATE gstate = PyGILState_Ensure();

    if (PyCallable_Check(onResumeCallable.get())) {
        PyObject *result = PyObject_CallFunction(onResumeCallable.get(), NULL);
        Py_CLEAR(result);
        if (result == NULL) {
            mypyPrintErr();
        }
    }

    PyGILState_Release(gstate);
}

PyObject *pcsx2ModuleBuilder(void)
{
    class methods
    {
    public:
        static PyObject *WriteLn(PyObject *self, PyObject *args)
        {
            Py_UNICODE *pzwOutput = NULL;
            if (!PyArg_ParseTuple(args, "u:pcsx2_WriteLn", &pzwOutput))
                return NULL;

            Console.WriteLn(L"%s", pzwOutput);

            Py_RETURN_NONE;
        }

        static PyObject *Error(PyObject *self, PyObject *args)
        {
            Py_UNICODE *pzwOutput = NULL;
            if (!PyArg_ParseTuple(args, "u:pcsx2_Error", &pzwOutput))
                return NULL;

            Console.Error(L"%s", pzwOutput);

            Py_RETURN_NONE;
        }

        static PyObject *Warning(PyObject *self, PyObject *args)
        {
            Py_UNICODE *pzwOutput = NULL;
            if (!PyArg_ParseTuple(args, "u:pcsx2_Warning", &pzwOutput))
                return NULL;

            Console.Warning(L"%s", pzwOutput);

            Py_RETURN_NONE;
        }

        static PyObject *AddBrk(PyObject *self, PyObject *args)
        {
            uint pc = 0;
            PyObject *pyCb = NULL;
            if (!PyArg_ParseTuple(args, "IO:pcsx2_AddBrk", &pc, &pyCb))
                return NULL;

            int token = brk::add(pc, pyCb);

            exitstatus::s_newbp = true;
            return PyLong_FromLong(token);
        }

        static PyObject *AddRBrk(PyObject *self, PyObject *args)
        {
            uint pc = 0, len = 0;
            PyObject *callable = NULL;
            if (!PyArg_ParseTuple(args, "IIO:pcsx2_AddRBrk", &pc, &len, &callable))
                return NULL;

            int token = rbrk::add(pc, len, callable);

            exitstatus::s_newbp = true;
            return PyLong_FromLongLong(token);
        }

        static PyObject *DelRBrk(PyObject *self, PyObject *args)
        {
            uint token = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_DelRBrk", &token))
                return NULL;

            rbrk::remove(token);

            Py_RETURN_NONE;
        }

        static PyObject *AddWBrk(PyObject *self, PyObject *args)
        {
            uint pc = 0, len = 0;
            PyObject *callable = NULL;
            if (!PyArg_ParseTuple(args, "IIO:pcsx2_AddWBrk", &pc, &len, &callable))
                return NULL;

            int token = wbrk::add(pc, len, callable);

            exitstatus::s_newbp = true;
            return PyLong_FromLong(token);
        }

        static PyObject *DelWBrk(PyObject *self, PyObject *args)
        {
            uint token = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_DelWBrk", &token))
                return NULL;

            wbrk::remove(token);

            Py_RETURN_NONE;
        }

        static PyObject *ReadMem(PyObject *self, PyObject *args)
        {
            uint off = 0, len = 0;
            if (!PyArg_ParseTuple(args, "II:pcsx2_ReadMem", &off, &len))
                return NULL;

            std::unique_ptr<void, utils::ReleasePyMem> bytes(PyMem_Malloc(len));
            if (bytes.get() == NULL)
                return NULL;

            u8 *writer = (u8 *)bytes.get();

            uint x = 0, cx = len;
            for (; x < cx; x++, off++, writer++) {
                u8 *reader = (u8 *)PSM(off);
                *writer = (reader != NULL) ? *reader : 0;
            }

            return PyBytes_FromStringAndSize((const char *)bytes.get(), len);
        }

        static PyObject *WriteMem(PyObject *self, PyObject *args)
        {
            PyObject *pyBytes = NULL;
            uint startIndex = 0, addr = 0, len = 0;
            if (!PyArg_ParseTuple(args, "O!III:pcsx2_WriteMem", &PyBytes_Type, &pyBytes, &startIndex, &addr, &len))
                return NULL;

            char *pMem = NULL;
            Py_ssize_t cbMem = 0;
            if (PyBytes_AsStringAndSize(pyBytes, &pMem, &cbMem) < 0) {
                return NULL;
            }

            if (len > (uint)cbMem || startIndex + len > (uint)cbMem) {
                return PyErr_Format(PyExc_OverflowError, "startIndex %u and len %u exceeds buffer size %u.", startIndex, len, cbMem);
            }

            if (Cpu != NULL)
                Cpu->Clear(startIndex, len);

            const u8 *reader = (const u8 *)pMem;

            reader += startIndex;

            uint x = 0;
            for (; x < len; x++, addr++, reader++) {
                u8 *writer = (u8 *)PSM(addr);
                if (writer != NULL)
                    *writer = *reader;
            }

            Py_RETURN_NONE;
        }


        static PyObject *GetUL0(PyObject *self, PyObject *args)
        {
            PyObject *py = NULL;
            if (!PyArg_ParseTuple(args, "O:pcsx2_GetUL0", &py))
                return NULL;

            if (PyLong_Check(py)) {
                uint regIdx = (uint)PyLong_AsLong(py);
                if (regIdx < 32) {
                    return PyLong_FromUnsignedLong(cpuRegs.GPR.r[regIdx].UL[0]);
                }

                PyErr_Format(PyExc_IndexError, "%d isn't a GPR index. 0 to 31.", regIdx);
                return NULL;
            }
            if (PyUnicode_Check(py)) {
                Py_UNICODE *pswName = PyUnicode_AsUnicode(py);
                if (pswName == NULL) {
                    return NULL;
                }

                static const wchar_t *pswGPRNames[32] = {L"zero", L"at", L"v0", L"v1", L"a0", L"a1", L"a2", L"a3", L"t0", L"t1", L"t2", L"t3", L"t4", L"t5", L"t6", L"t7", L"s0", L"s1", L"s2", L"s3", L"s4", L"s5", L"s6", L"s7", L"t8", L"t9", L"k0", L"k1", L"gp", L"sp", L"s8", L"ra"};

                for (int x = 0; x < 32; x++) {
                    if (wcscmp(pswGPRNames[x], pswName) == 0) {
                        return PyLong_FromUnsignedLong(cpuRegs.GPR.r[x].UL[0]);
                    }
                }

                PyErr_Format(PyExc_KeyError, "'%s' isn't a GPR name. zero,at,v0,v1,a0,a1,a2,a3,t0,t1,t2,t3,t4,t5,t6,t7,s0,s1,s2,s3,s4,s5,s6,s7,t8,t9,k0,k1,gp,sp,s8,ra.", static_cast<const char *>(wxString(pswName).mb_str()));
                return NULL;
            }

            return PyErr_SetString(PyExc_TypeError, "Pass an int, or a str."), NULL;
        }

        static PyObject *SetUL0(PyObject *self, PyObject *args)
        {
            PyObject *py = NULL;
            uint val = 0;
            if (!PyArg_ParseTuple(args, "OI:pcsx2_SetUL0", &py, &val))
                return NULL;

            if (PyLong_Check(py)) {
                uint regIdx = (uint)PyLong_AsLong(py);
                if (regIdx < 32) {
                    cpuRegs.GPR.r[regIdx].UL[0] = val;
                    Py_RETURN_NONE;
                }

                PyErr_Format(PyExc_IndexError, "%d isn't a GPR index. 0 to 31.", regIdx);
                return NULL;
            }
            if (PyUnicode_Check(py)) {
                Py_UNICODE *pswName = PyUnicode_AsUnicode(py);
                if (pswName == NULL) {
                    return NULL;
                }

                static const wchar_t *pswGPRNames[32] = {L"zero", L"at", L"v0", L"v1", L"a0", L"a1", L"a2", L"a3", L"t0", L"t1", L"t2", L"t3", L"t4", L"t5", L"t6", L"t7", L"s0", L"s1", L"s2", L"s3", L"s4", L"s5", L"s6", L"s7", L"t8", L"t9", L"k0", L"k1", L"gp", L"sp", L"s8", L"ra"};

                for (int x = 0; x < 32; x++) {
                    if (wcscmp(pswGPRNames[x], pswName) == 0) {
                        cpuRegs.GPR.r[x & 31].UL[0] = val;
                        Py_RETURN_NONE;
                    }
                }

                PyErr_Format(PyExc_KeyError, "'%s' isn't a GPR name. zero,at,v0,v1,a0,a1,a2,a3,t0,t1,t2,t3,t4,t5,t6,t7,s0,s1,s2,s3,s4,s5,s6,s7,t8,t9,k0,k1,gp,sp,s8,ra.", static_cast<const char *>(wxString(pswName).mb_str()));
                return NULL;
            }

            return PyErr_SetString(PyExc_TypeError, "Pass an int, or a str."), NULL;
        }


        static PyObject *SetPC(PyObject *self, PyObject *args)
        {
            uint newPC = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_SetPC", &newPC))
                return NULL;

            exitstatus::s_newpc = true;
            exitstatus::s_mypy_new_pc = newPC;
            Py_RETURN_NONE;
        }

        static PyObject *ReadUI16(PyObject *self, PyObject *args)
        {
            uint off = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_ReadUI16", &off))
                return NULL;

            u16 *reader = (u16 *)PSM(off);
            return PyLong_FromUnsignedLong(*reader);
        }

        static PyObject *ReadI16(PyObject *self, PyObject *args)
        {
            uint off = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_ReadI16", &off))
                return NULL;

            s16 *reader = (s16 *)PSM(off);
            return PyLong_FromLong(*reader);
        }

        static PyObject *ReadUI32(PyObject *self, PyObject *args)
        {
            uint off = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_ReadUI32", &off))
                return NULL;

            u32 *reader = (u32 *)PSM(off);
            return PyLong_FromUnsignedLong(*reader);
        }

        static PyObject *ReadI32(PyObject *self, PyObject *args)
        {
            uint off = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_ReadI32", &off))
                return NULL;

            s32 *reader = (s32 *)PSM(off);
            return PyLong_FromLong(*reader);
        }

        static PyObject *ReadByte(PyObject *self, PyObject *args)
        {
            uint off = 0;
            if (!PyArg_ParseTuple(args, "I:pcsx2_ReadByte", &off))
                return NULL;

            u8 *reader = (u8 *)PSM(off);
            return PyLong_FromUnsignedLong(*reader);
        }

        static PyObject *WriteUI32(PyObject *self, PyObject *args)
        {
            uint off = 0, val = 0;
            if (!PyArg_ParseTuple(args, "II:pcsx2_WriteUI32", &off, &val))
                return NULL;

            memWrite32(off, val);
            Py_RETURN_NONE;
        }

        static PyObject *WriteUI16(PyObject *self, PyObject *args)
        {
            uint off = 0, val = 0;
            if (!PyArg_ParseTuple(args, "II:pcsx2_WriteUI16", &off, &val))
                return NULL;

            memWrite16(off, val);
            Py_RETURN_NONE;
        }

        static PyObject *WriteByte(PyObject *self, PyObject *args)
        {
            uint off = 0, val = 0;
            if (!PyArg_ParseTuple(args, "II:pcsx2_WriteByte", &off, &val))
                return NULL;

            memWrite8(off, val);
            Py_RETURN_NONE;
        }


        static PyObject *pc(PyObject *self)
        {
            return PyLong_FromLong(cpuRegs.pc);
        }

        static PyObject *opc(PyObject *self)
        {
            return PyLong_FromLong(s_mypy_opc);
        }

        static PyObject *OnSuspend(PyObject *self, PyObject *args)
        {
            PyObject *callable = NULL;
            if (!PyArg_ParseTuple(args, "O:pcsx2_OnSuspend", &callable))
                return NULL;

            if (PyCallable_Check(callable) == 0)
                return PyErr_Format(PyExc_TypeError, "take a callable object.");

            onSuspendCallable.reset(callable);
            Py_XINCREF(callable);

            Py_RETURN_NONE;
        }

        static PyObject *OnResume(PyObject *self, PyObject *args)
        {
            PyObject *callable = NULL;
            if (!PyArg_ParseTuple(args, "O:pcsx2_OnResume", &callable))
                return NULL;

            if (PyCallable_Check(callable) == 0)
                return PyErr_Format(PyExc_TypeError, "take a callable object.");

            onResumeCallable.reset(callable);
            Py_XINCREF(callable);

            Py_RETURN_NONE;
        }

        static PyObject *ClearClient(PyObject *self)
        {
            mypyClearClient();
            Py_RETURN_NONE;
        }

        static PyObject *isRec(PyObject *self)
        {
            if (Cpu == &recCpu)
                Py_RETURN_TRUE;
            Py_RETURN_FALSE;
        }

        static PyObject *ElfCRC(PyObject *self)
        {
            extern u32 ElfCRC;

            return PyLong_FromUnsignedLong(ElfCRC);
        }

        static PyObject *StartEETrace(PyObject *self, PyObject *args)
        {
            Py_UNICODE *pzwOutput = NULL;
            if (!PyArg_ParseTuple(args, "u:pcsx2_StartEETrace", &pzwOutput))
                return NULL;

            if (Cpu != &intCpu)
                Console.WriteLn(L"(mypy)%s requires EmotionEngine is Interpreter!", L"pcsx2_StartEETrace");

            eetrace.close();

            if (!eetrace.open(pzwOutput))
                return PyErr_Format(PyExc_IOError, "failed to open output file.");

            if (!eetrace.MypyWriteEETrace(1)) {
                eetrace.close();
                return PyErr_Format(PyExc_IOError, "failed to write output file.");
            }

            Py_RETURN_NONE;
        }

        static PyObject *EndEETrace(PyObject *self)
        {
            eetrace.close();

            Py_RETURN_NONE;
        }

        static PyObject *SetRWTraceOptions(PyObject *self, PyObject *args)
        {
            uint flags = 0;
            PyObject *callable = NULL;
            if (!PyArg_ParseTuple(args, "IO:pcsx2_SetRWTraceOptions", &flags, &callable))
                return NULL;

            if (callable != Py_None && !PyCallable_Check(callable))
                return PyErr_Format(PyExc_TypeError, "take a callable object.");

            onFlushRWTrace.reset(callable);
            Py_XINCREF(callable);

            exitstatus::s_newbp = true;
            rwtrace.setFlags(flags);
            Py_RETURN_NONE;
        }

        static PyObject *FlushRWTrace(PyObject *self)
        {
            if (!mypyFlushRWTrace()) {
                return NULL; // means exception raised
            }

            Py_RETURN_NONE;
        }
    };

    static PyMethodDef pcsx2_methods[] = {
        {"ClearClient", (PyCFunction)methods::ClearClient, METH_NOARGS},
        {"WriteLn", methods::WriteLn, METH_VARARGS},
        {"AddBrk", methods::AddBrk, METH_VARARGS},
        {"GetUL0", methods::GetUL0, METH_VARARGS},
        {"SetUL0", methods::SetUL0, METH_VARARGS},
        {"SetPC", methods::SetPC, METH_VARARGS},
        {"ReadMem", methods::ReadMem, METH_VARARGS},
        {"ReadUI16", methods::ReadUI16, METH_VARARGS},
        {"ReadUI32", methods::ReadUI32, METH_VARARGS},
        {"ReadI16", methods::ReadI16, METH_VARARGS},
        {"ReadI32", methods::ReadI32, METH_VARARGS},
        {"ReadByte", methods::ReadByte, METH_VARARGS},
        {"WriteMem", methods::WriteMem, METH_VARARGS},
        {"WriteByte", methods::WriteByte, METH_VARARGS},
        {"WriteUI16", methods::WriteUI16, METH_VARARGS},
        {"WriteUI32", methods::WriteUI32, METH_VARARGS},
        {"AddRBrk", methods::AddRBrk, METH_VARARGS},
        {"DelRBrk", methods::DelRBrk, METH_VARARGS},
        {"AddWBrk", methods::AddWBrk, METH_VARARGS},
        {"DelWBrk", methods::DelWBrk, METH_VARARGS},
        {"pc", (PyCFunction)methods::pc, METH_NOARGS},
        {"opc", (PyCFunction)methods::opc, METH_NOARGS},
        {"isRec", (PyCFunction)methods::isRec, METH_NOARGS},
        {"ElfCRC", (PyCFunction)methods::ElfCRC, METH_NOARGS},
        {"Error", methods::Error, METH_VARARGS},
        {"Warning", methods::Warning, METH_VARARGS},
        {"OnSuspend", methods::OnSuspend, METH_VARARGS},
        {"OnResume", methods::OnResume, METH_VARARGS},

        {"StartEETrace", methods::StartEETrace, METH_VARARGS},
        {"EndEETrace", (PyCFunction)methods::EndEETrace, METH_NOARGS},

        {"SetRWTraceOptions", methods::SetRWTraceOptions, METH_VARARGS},
        {"FlushRWTrace", (PyCFunction)methods::FlushRWTrace, METH_NOARGS},
        {NULL, NULL, 0, NULL},
    };

    static PyModuleDef pcsx2module = {
        PyModuleDef_HEAD_INIT,
        "pcsx2",
        "pcsx2 internal access.",
        -1,
        pcsx2_methods, NULL, NULL, NULL, NULL};

    return PyModule_Create(&pcsx2module);
}

void finalize()
{
    if (!Py_IsInitialized())
        return;

    PyEval_RestoreThread(s_pyTState);

    mypyClearClient();

    Py_Finalize();
    Console.WriteLn(L"Python finalized.");
}

void elfLoaded()
{
    mypyClearClient();
    mypyEnsureMod(wxsFormat(L"monitor.%08X", ElfCRC));
}

void init()
{
    wxChar wcFileName[260] = {0};
    GetModuleFileName(NULL, wcFileName, 260);
    Py_SetProgramName(wcFileName);

    PyImport_AppendInittab("pcsx2", pcsx2ModuleBuilder);

    Py_Initialize();
    PyEval_InitThreads();

    Console.WriteLn(L"Python initialized.");
    Console.WriteLn(L"-- ProgramName: %s ", Py_GetProgramName());
    Console.WriteLn(L"-- Prefix: %s ", Py_GetPrefix());
    Console.WriteLn(L"-- ExecPrefix: %s ", Py_GetExecPrefix());
    Console.WriteLn(L"-- Path: %s ", Py_GetPath());
    Console.WriteLn("-- Version: %s ", Py_GetVersion());
    Console.WriteLn("-- Platform: %s ", Py_GetPlatform());
    Console.WriteLn("-- Copyright: %s ", Py_GetCopyright());

    lastSuspended = true;

    Console.WriteLn(L"(mypy)Import module: %s ", L"monitor.main");

    PyObject *module = PyImport_ImportModule("monitor.main");
    Py_CLEAR(module);

    if (module == NULL) {
        mypyPrintErr();
    }

    s_pyTState = PyEval_SaveThread();
}

void suspend()
{
}

void resume()
{
}

} // namespace py

namespace brk
{
void __cdecl invoke()
{
    Items::iterator iter = items.lower_bound(currentPc);
    for (; iter->first == currentPc && iter != items.end(); iter++) {
        PyGILState_STATE gstate = PyGILState_Ensure();
        PyObject *callable = iter->second.callable.get();

        if (PyCallable_Check(callable) == 1) {
            utils::UniquePyObject result(PyObject_CallObject(callable, NULL));
            if (result.get() == NULL) {
                py::mypyPrintErr();
            }
        }

        PyGILState_Release(gstate);
    }
}
} // namespace brk

namespace rbrk
{
void __cdecl invoke()
{
    const u32 target = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;

    Items::iterator iter = items.begin();
    for (; iter != items.end(); iter++) {
        if (iter->ifHit(target)) {
            PyGILState_STATE gstate = PyGILState_Ensure();
            PyObject *callable = iter->callable.get();

            if (PyCallable_Check(callable) == 1) {
                utils::UniquePyObject result(PyObject_CallFunction(callable, "(I)", target));

                if (result.get() == NULL) {
                    py::mypyPrintErr();
                }
            }

            PyGILState_Release(gstate);
        }
    }
}
} // namespace rbrk


namespace wbrk
{
void __cdecl invoke()
{
    const u32 target = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;

    Items::iterator iter = items.begin();
    for (; iter != items.end(); iter++) {
        if (iter->ifHit(target)) {
            PyGILState_STATE gstate = PyGILState_Ensure();
            PyObject *callable = iter->callable.get();

            if (PyCallable_Check(callable) == 1) {
                utils::UniquePyObject result(PyObject_CallFunction(callable, "(I)", target));

                if (result.get() == NULL) {
                    py::mypyPrintErr();
                }
            }

            PyGILState_Release(gstate);
        }
    }
}
} // namespace wbrk

RWTrace rwtrace;

void RWTrace::mypyRecordRW(int flags)
{
    if (!(s_mypy_rwTrace & flags)) {
        return;
    }

    if (s_rwTraceIdx >= MaxTrace) {
        PyGILState_STATE gstate = PyGILState_Ensure();
        bool ok = py::mypyFlushRWTrace();
        PyGILState_Release(gstate);
        if (!ok) {
            return;
        }
    }

    int pos = 3 * s_rwTraceIdx;
    s_rwTraceBuff[pos + 0] = currentPc;
    s_rwTraceBuff[pos + 1] = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;
    s_rwTraceBuff[pos + 2] = flags;
    ++s_rwTraceIdx;
}

void __cdecl encounterLoadOp()
{
    rbrk::invoke();
    rwtrace.mypyRecordRW(1);
}
void __cdecl encounterStoreOp()
{
    wbrk::invoke();
    rwtrace.mypyRecordRW(2);
}

void __cdecl invokeCpuReset()
{
    Cpu->Reset();
}

int testInjections(u32 pc)
{
    return ((brk::items.find(pc) == brk::items.end()) ? 0 : 4) | (rbrk::items.empty() ? 0 : 1) | (wbrk::items.empty() ? 0 : 2);
}

} // namespace execmon

// --kkdf2