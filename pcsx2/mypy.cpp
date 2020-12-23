// kkdf2--
#include "PrecompiledHeader.h"
#include "System.h"
#include "Memory.h"
#include "Hw.h"
#include "R5900.h"

#include "ElfHeader.h"
#include "AppConfig.h"

#include "VU.h"

#include <wx/dir.h>

#include <map>

#include <windows.h>

#include "mypy.h"

u32 s_mypyHitBrk = 0;
u32 s_mypyEat = 0;

static bool s_newbp = false; // A break point have updated. The recompiler needs refresh.
static bool s_newpc = false; // PC updated. The recompiler needs refresh.

FILE *s_feet = NULL;

static u32 s_tracpos = 0;

#define TRACE_MAX 4096
static u32 s_rwTraceIdx = 0;
static u32 s_rwTraceBuff[3 * TRACE_MAX];

static PyObject *pcsx2_WriteLn(PyObject *self, PyObject *args)
{
    Py_UNICODE *pzwOutput = NULL;
    if (!PyArg_ParseTuple(args, "u:pcsx2_WriteLn", &pzwOutput))
        return NULL;

    Console.WriteLn(L"%s", pzwOutput);

    Py_RETURN_NONE;
}

static PyObject *pcsx2_Error(PyObject *self, PyObject *args)
{
    Py_UNICODE *pzwOutput = NULL;
    if (!PyArg_ParseTuple(args, "u:pcsx2_Error", &pzwOutput))
        return NULL;

    Console.Error(L"%s", pzwOutput);

    Py_RETURN_NONE;
}

static PyObject *pcsx2_Warning(PyObject *self, PyObject *args)
{
    Py_UNICODE *pzwOutput = NULL;
    if (!PyArg_ParseTuple(args, "u:pcsx2_Warning", &pzwOutput))
        return NULL;

    Console.Warning(L"%s", pzwOutput);

    Py_RETURN_NONE;
}

MypyBrk s_mypyBrk[MAX_BRK];

static PyObject *pcsx2_AddBrk(PyObject *self, PyObject *args)
{
    uint pc = 0;
    PyObject *pyCb = NULL;
    if (!PyArg_ParseTuple(args, "IO:pcsx2_AddBrk", &pc, &pyCb))
        return NULL;

    pc &= ~3UL;

    if (Cpu != NULL)
        Cpu->Clear(pc, 4);

    int freei = -1;
    int curi = -1;

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyBrk[x];
        if (r.pc == 0) {
            if (freei < 0)
                freei = x;
        } else if (r.pc == pc) {
            if (curi < 0)
                curi = x;
        }
    }

    if (curi >= 0)
        return PyLong_FromLong(curi);
    if (freei < 0)
        return PyErr_SetString(PyExc_OverflowError, "Sorry, no more space to add break point."), NULL;

    MypyBrk &r = s_mypyBrk[freei];
    r.pc = pc;
    r.pyCb = pyCb;
    Py_XINCREF(pyCb);
    s_newbp = true;
    return PyLong_FromLong(freei);
}

MypyBrk s_mypyRBrk[MAX_BRK];

static PyObject *pcsx2_AddRBrk(PyObject *self, PyObject *args)
{
    uint pc = 0, len = 0;
    PyObject *pyCb = NULL;
    if (!PyArg_ParseTuple(args, "IIO:pcsx2_AddRBrk", &pc, &len, &pyCb))
        return NULL;

    int freei = -1;
    int curi = -1;

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyRBrk[x];
        if (r.pc == 0) {
            if (freei < 0)
                freei = x;
        } else if (r.pc == pc) {
            if (curi < 0)
                curi = x;
        }
    }

    if (curi >= 0)
        return PyLong_FromLong(curi);
    if (freei < 0)
        return PyErr_SetString(PyExc_OverflowError, "Sorry, no more space to add read break point."), NULL;

    MypyBrk &r = s_mypyRBrk[freei];
    r.pc = pc;
    r.len = len;
    r.pyCb = pyCb;
    Py_XINCREF(pyCb);
    s_newbp = true;
    return PyLong_FromLong(freei);
}

static PyObject *pcsx2_DelRBrk(PyObject *self, PyObject *args)
{
    uint x = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_DelRBrk", &x))
        return NULL;

    if (x >= MAX_BRK)
        return PyErr_SetString(PyExc_IndexError, "Out of index for read break point."), NULL;

    MypyBrk &r = s_mypyRBrk[x];
    if (r.pyCb != NULL)
        Py_CLEAR(r.pyCb);
    r = MypyBrk();

    Py_RETURN_NONE;
}

MypyBrk s_mypyWBrk[MAX_BRK];

static PyObject *pcsx2_AddWBrk(PyObject *self, PyObject *args)
{
    uint pc = 0, len = 0;
    PyObject *pyCb = NULL;
    if (!PyArg_ParseTuple(args, "IIO:pcsx2_AddWBrk", &pc, &len, &pyCb))
        return NULL;

    int freei = -1;
    int curi = -1;

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyWBrk[x];
        if (r.pc == 0) {
            if (freei < 0)
                freei = x;
        } else if (r.pc == pc) {
            if (curi < 0)
                curi = x;
        }
    }

    if (curi >= 0)
        return PyLong_FromLong(curi);
    if (freei < 0)
        return PyErr_SetString(PyExc_OverflowError, "Sorry, no more space to add write break point."), NULL;

    MypyBrk &r = s_mypyWBrk[freei];
    r.pc = pc;
    r.len = len;
    r.pyCb = pyCb;
    Py_XINCREF(pyCb);
    s_newbp = true;
    return PyLong_FromLong(freei);
}

static PyObject *pcsx2_DelWBrk(PyObject *self, PyObject *args)
{
    uint x = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_DelWBrk", &x))
        return NULL;

    if (x >= MAX_BRK)
        return PyErr_SetString(PyExc_IndexError, "Out of index for write break point."), NULL;

    MypyBrk &r = s_mypyWBrk[x];
    if (r.pyCb != NULL)
        Py_CLEAR(r.pyCb);
    r = MypyBrk();

    Py_RETURN_NONE;
}

static PyObject *pcsx2_ReadMem(PyObject *self, PyObject *args)
{
    uint off = 0, len = 0;
    if (!PyArg_ParseTuple(args, "II:pcsx2_ReadMem", &off, &len))
        return NULL;

    void *pvTmp = PyMem_Malloc(len);
    if (pvTmp == NULL)
        return NULL;

    u8 *pWr = (u8 *)pvTmp;

    uint x = 0, cx = len;
    for (; x < cx; x++, off++, pWr++) {
        u8 *pRd = (u8 *)PSM(off);
        *pWr = (pRd != NULL) ? *pRd : 0;
    }

    PyObject *buff = PyBytes_FromStringAndSize((const char *)pvTmp, len);
    if (buff == NULL) {
        PyMem_Free(pvTmp);
        return NULL;
    }

    PyMem_Free(pvTmp);
    return buff;
}

static PyObject *pcsx2_WriteMem(PyObject *self, PyObject *args)
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

    const u8 *pRd = (const u8 *)pMem;

    pRd += startIndex;

    uint x = 0;
    for (; x < len; x++, addr++, pRd++) {
        u8 *pWr = (u8 *)PSM(addr);
        if (pWr != NULL)
            *pWr = *pRd;
    }

    Py_RETURN_NONE;
}

static PyObject *pcsx2_GetUL0(PyObject *self, PyObject *args)
{
    PyObject *py = NULL;
    if (!PyArg_ParseTuple(args, "O:pcsx2_GetUL0", &py))
        return NULL;

    if (PyLong_Check(py)) {
        long ri = PyLong_AsLong(py);
        if ((uint)ri < 32) {
            return PyLong_FromUnsignedLong(cpuRegs.GPR.r[ri & 31].UL[0]);
        }

        PyErr_Format(PyExc_IndexError, "%d isn't a GPR index. 0 to 31.", ri);
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
                return PyLong_FromUnsignedLong(cpuRegs.GPR.r[x & 31].UL[0]);
            }
        }

        PyErr_Format(PyExc_KeyError, "'%s' isn't a GPR name. zero,at,v0,v1,a0,a1,a2,a3,t0,t1,t2,t3,t4,t5,t6,t7,s0,s1,s2,s3,s4,s5,s6,s7,t8,t9,k0,k1,gp,sp,s8,ra.", static_cast<const char *>(wxString(pswName).mb_str()));
        return NULL;
    }

    return PyErr_SetString(PyExc_TypeError, "Pass an int, or a str."), NULL;
}

static PyObject *pcsx2_SetUL0(PyObject *self, PyObject *args)
{
    PyObject *py = NULL;
    uint val = 0;
    if (!PyArg_ParseTuple(args, "OI:pcsx2_SetUL0", &py, &val))
        return NULL;

    if (PyLong_Check(py)) {
        long ri = PyLong_AsLong(py);
        if ((uint)ri < 32) {
            cpuRegs.GPR.r[ri & 31].UL[0] = val;
            Py_RETURN_NONE;
        }

        PyErr_Format(PyExc_IndexError, "%d isn't a GPR index. 0 to 31.", ri);
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

static PyObject *pcsx2_SetPC(PyObject *self, PyObject *args)
{
    uint newPC = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_SetPC", &newPC))
        return NULL;

    s_newpc = true;
    s_mypy_new_pc = newPC;
    Py_RETURN_NONE;
}

static PyObject *pcsx2_ReadUI16(PyObject *self, PyObject *args)
{
    uint off = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_ReadUI16", &off))
        return NULL;

    u16 *pRd = (u16 *)PSM(off);
    return PyLong_FromUnsignedLong(*pRd);
}

static PyObject *pcsx2_ReadI16(PyObject *self, PyObject *args)
{
    uint off = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_ReadI16", &off))
        return NULL;

    s16 *pRd = (s16 *)PSM(off);
    return PyLong_FromLong(*pRd);
}

static PyObject *pcsx2_ReadUI32(PyObject *self, PyObject *args)
{
    uint off = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_ReadUI32", &off))
        return NULL;

    u32 *pRd = (u32 *)PSM(off);
    return PyLong_FromUnsignedLong(*pRd);
}

static PyObject *pcsx2_ReadI32(PyObject *self, PyObject *args)
{
    uint off = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_ReadI32", &off))
        return NULL;

    s32 *pRd = (s32 *)PSM(off);
    return PyLong_FromLong(*pRd);
}

static PyObject *pcsx2_ReadByte(PyObject *self, PyObject *args)
{
    uint off = 0;
    if (!PyArg_ParseTuple(args, "I:pcsx2_ReadByte", &off))
        return NULL;

    u8 *pRd = (u8 *)PSM(off);
    return PyLong_FromUnsignedLong(*pRd);
}

static PyObject *pcsx2_WriteUI32(PyObject *self, PyObject *args)
{
    uint off = 0, val = 0;
    if (!PyArg_ParseTuple(args, "II:pcsx2_WriteUI32", &off, &val))
        return NULL;

    memWrite32(off, val);
    Py_RETURN_NONE;
}

static PyObject *pcsx2_WriteUI16(PyObject *self, PyObject *args)
{
    uint off = 0, val = 0;
    if (!PyArg_ParseTuple(args, "II:pcsx2_WriteUI16", &off, &val))
        return NULL;

    memWrite16(off, val);
    Py_RETURN_NONE;
}

static PyObject *pcsx2_WriteByte(PyObject *self, PyObject *args)
{
    uint off = 0, val = 0;
    if (!PyArg_ParseTuple(args, "II:pcsx2_WriteByte", &off, &val))
        return NULL;

    memWrite8(off, val);
    Py_RETURN_NONE;
}

u32 s_mypy_pc = 0;
u32 s_mypy_opc = 0;
u32 s_mypy_new_pc = 0;

static PyObject *pcsx2_pc(PyObject *self)
{
    return PyLong_FromLong(s_mypy_pc);
}

static PyObject *pcsx2_opc(PyObject *self)
{
    return PyLong_FromLong(s_mypy_opc);
}

PyObject *s_pylSuspend = NULL;

static PyObject *pcsx2_OnSuspend(PyObject *self, PyObject *args)
{
    PyObject *pyC = NULL;
    if (!PyArg_ParseTuple(args, "O:pcsx2_OnSuspend", &pyC))
        return NULL;

    if (PyCallable_Check(pyC) == 0)
        return PyErr_Format(PyExc_TypeError, "take a callable object.");

    if (s_pylSuspend != NULL) {
        PyList_Append(s_pylSuspend, pyC);
    }

    Py_RETURN_NONE;
}

PyObject *s_pylResume = NULL;

static PyObject *pcsx2_OnResume(PyObject *self, PyObject *args)
{
    PyObject *pyC = NULL;
    if (!PyArg_ParseTuple(args, "O:pcsx2_OnResume", &pyC))
        return NULL;

    if (PyCallable_Check(pyC) == 0)
        return PyErr_Format(PyExc_TypeError, "take a callable object.");

    if (s_pylResume != NULL) {
        PyList_Append(s_pylResume, pyC);
    }

    Py_RETURN_NONE;
}

extern void mypyClearClient();

static PyObject *pcsx2_ClearClient(PyObject *self)
{
    mypyClearClient();
    Py_RETURN_NONE;
}

static PyObject *pcsx2_isRec(PyObject *self)
{
    if (Cpu == &recCpu)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyObject *pcsx2_ElfCRC(PyObject *self)
{
    extern u32 ElfCRC;

    return PyLong_FromUnsignedLong(ElfCRC);
}

static PyObject *pcsx2_StartEETrace(PyObject *self, PyObject *args)
{
    Py_UNICODE *pzwOutput = NULL;
    if (!PyArg_ParseTuple(args, "u:pcsx2_StartEETrace", &pzwOutput))
        return NULL;

    if (Cpu != &intCpu)
        Console.WriteLn(L"(mypy)%s requires EmotionEngine is Interpreter!", L"pcsx2_StartEETrace");

    if (s_feet != NULL)
        fclose(s_feet);

    s_feet = _wfopen(pzwOutput, L"wb");

    if (s_feet == NULL)
        return PyErr_Format(PyExc_IOError, "failed to open output file.");

    s_tracpos = 0;

    if (!MypyWriteEETrace(1)) {
        fclose(s_feet);
        s_feet = NULL;
        return PyErr_Format(PyExc_IOError, "failed to write output file.");
    }

    Py_RETURN_NONE;
}

static PyObject *pcsx2_EndEETrace(PyObject *self)
{
    if (s_feet != NULL) {
        fclose(s_feet);
        s_feet = NULL;
    }

    Py_RETURN_NONE;
}

PyObject *s_pycRWTrace = NULL;

static PyObject *pcsx2_SetRWTraceOptions(PyObject *self, PyObject *args)
{
    uint flags = 0;
    PyObject *pyCb = NULL;
    if (!PyArg_ParseTuple(args, "IO:pcsx2_SetRWTraceOptions", &flags, &pyCb))
        return NULL;

    if (pyCb != Py_None && !PyCallable_Check(pyCb))
        return PyErr_Format(PyExc_TypeError, "take a callable object.");

	Py_CLEAR(s_pycRWTrace);
    s_pycRWTrace = pyCb;
    Py_XINCREF(s_pycRWTrace);

    s_newbp = true;
    s_mypy_rwTrace = flags;
    Py_RETURN_NONE;
}

static PyObject *pcsx2_FlushRWTrace(PyObject *self)
{
    mypyFlushRWTrace();

    Py_RETURN_NONE;
}

static PyMethodDef pcsx2_methods[] = {
    {"ClearClient", (PyCFunction)pcsx2_ClearClient, METH_NOARGS},
    {"WriteLn", pcsx2_WriteLn, METH_VARARGS},
    {"AddBrk", pcsx2_AddBrk, METH_VARARGS},
    {"GetUL0", pcsx2_GetUL0, METH_VARARGS},
    {"SetUL0", pcsx2_SetUL0, METH_VARARGS},
    {"SetPC", pcsx2_SetPC, METH_VARARGS},
    {"ReadMem", pcsx2_ReadMem, METH_VARARGS},
    {"ReadUI16", pcsx2_ReadUI16, METH_VARARGS},
    {"ReadUI32", pcsx2_ReadUI32, METH_VARARGS},
    {"ReadI16", pcsx2_ReadI16, METH_VARARGS},
    {"ReadI32", pcsx2_ReadI32, METH_VARARGS},
    {"ReadByte", pcsx2_ReadByte, METH_VARARGS},
    {"WriteMem", pcsx2_WriteMem, METH_VARARGS},
    {"WriteByte", pcsx2_WriteByte, METH_VARARGS},
    {"WriteUI16", pcsx2_WriteUI16, METH_VARARGS},
    {"WriteUI32", pcsx2_WriteUI32, METH_VARARGS},
    {"AddRBrk", pcsx2_AddRBrk, METH_VARARGS},
    {"DelRBrk", pcsx2_DelRBrk, METH_VARARGS},
    {"AddWBrk", pcsx2_AddWBrk, METH_VARARGS},
    {"DelWBrk", pcsx2_DelWBrk, METH_VARARGS},
    {"pc", (PyCFunction)pcsx2_pc, METH_NOARGS},
    {"opc", (PyCFunction)pcsx2_opc, METH_NOARGS},
    {"isRec", (PyCFunction)pcsx2_isRec, METH_NOARGS},
    {"ElfCRC", (PyCFunction)pcsx2_ElfCRC, METH_NOARGS},
    //	{"AddMBrk", pcsx2_AddMBrk, METH_VARARGS},
    //	{"DelMBrk", pcsx2_DelMBrk, METH_VARARGS},
    {"Error", pcsx2_Error, METH_VARARGS},
    {"Warning", pcsx2_Warning, METH_VARARGS},
    {"OnSuspend", pcsx2_OnSuspend, METH_VARARGS},
    {"OnResume", pcsx2_OnResume, METH_VARARGS},

    {"StartEETrace", pcsx2_StartEETrace, METH_VARARGS},
    {"EndEETrace", (PyCFunction)pcsx2_EndEETrace, METH_NOARGS},

    {"SetRWTraceOptions", pcsx2_SetRWTraceOptions, METH_VARARGS},
    {"FlushRWTrace", (PyCFunction)pcsx2_FlushRWTrace, METH_NOARGS},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef pcsx2module = {
    PyModuleDef_HEAD_INIT,
    "pcsx2",
    "pcsx2 internal access.",
    -1,
    pcsx2_methods, NULL, NULL, NULL, NULL};

PyObject *PyInit_pcsx2(void)
{
    return PyModule_Create(&pcsx2module);
}

static PyThreadState *s_pyTState = NULL;

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

static bool s_lastSuspend = true;

void MypyInit()
{
    wxChar wcFileName[260] = {0};
    GetModuleFileName(NULL, wcFileName, 260);
    Py_SetProgramName(wcFileName);

    PyImport_AppendInittab("pcsx2", PyInit_pcsx2);

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

    s_pylSuspend = PyList_New(0);
    s_pylResume = PyList_New(0);

    s_lastSuspend = true;

    Console.WriteLn(L"(mypy)Import module: %s ", L"monitor.main");

    PyObject *pyM = PyImport_ImportModule("monitor.main");
    if (pyM != NULL) {
        Py_CLEAR(pyM);
    } else {
        mypyPrintErr();
    }

    s_pyTState = PyEval_SaveThread();
}

void MypyFinalize()
{
    if (!Py_IsInitialized())
        return;

    PyEval_RestoreThread(s_pyTState);

    mypyClearClient();

    Py_CLEAR(s_pylSuspend);
    Py_CLEAR(s_pylResume);

    Py_Finalize();
    Console.WriteLn(L"Python finalized.");
}

void mypyEnsureMod(wxString fp)
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    Console.WriteLn(L"(mypy)Ensure module: %s ", WX_STR(fp));
    PyObject *pyD = PyImport_GetModuleDict();
    if (pyD != NULL) {
        wxASSERT(PyDict_CheckExact(pyD));
        PyObject *pyM = PyDict_GetItemString(pyD, fp.mb_str());
        if (pyM != NULL) {
            wxASSERT(PyModule_CheckExact(pyM));
            PyObject *pyM2 = PyImport_ReloadModule(pyM);
            if (pyM2 != NULL) {
                Py_CLEAR(pyM2);
            } else
                mypyPrintErr(), PyErr_Clear();
        } else {
            PyObject *pyM2 = PyImport_ImportModule(fp.mb_str());
            if (pyM2 != NULL) {
                Py_CLEAR(pyM2);
            } else
                mypyPrintErr(), PyErr_Clear();
        }
    }

    PyGILState_Release(gstate);
}

void mypyClearClient()
{
    PyGILState_STATE gstate = PyGILState_Ensure();

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyBrk[x];
        if (r.pyCb != NULL)
            Py_CLEAR(r.pyCb);
        if (r.pc != 0) {
            if (Cpu != NULL)
                Cpu->Clear(r.pc, 4);
        }
        r = MypyBrk();
    }

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyRBrk[x];
        if (r.pyCb != NULL)
            Py_CLEAR(r.pyCb);
        r = MypyBrk();
    }

    for (int x = 0; x < MAX_BRK; x++) {
        MypyBrk &r = s_mypyWBrk[x];
        if (r.pyCb != NULL)
            Py_CLEAR(r.pyCb);
        r = MypyBrk();
    }

    if (s_pylSuspend != NULL) {
        Py_CLEAR(s_pylSuspend);

        s_pylSuspend = PyList_New(0);

        if (s_pylSuspend == NULL)
            PyErr_Clear();
    }

    if (s_pylResume != NULL) {
        Py_CLEAR(s_pylResume);

        s_pylResume = PyList_New(0);

        if (s_pylResume == NULL)
            PyErr_Clear();
    }

    if (s_feet != NULL) {
        fclose(s_feet);
        s_feet = NULL;
    }

    s_mypy_rwTrace = 0;
    Py_CLEAR(s_pycRWTrace);
    s_rwTraceIdx = 0;

    s_mypyHitBrk = 0;

    PyGILState_Release(gstate);
}

void MypyOnSuspend()
{
    if (s_lastSuspend)
        return;
    s_lastSuspend = true;

    PyGILState_STATE gstate = PyGILState_Ensure();

    wxASSERT(PyList_Check(s_pylSuspend));

    int x = 0, cx = PyList_Size(s_pylSuspend);
    for (; x < cx; x++) {
        PyObject *pyC = PyList_GetItem(s_pylSuspend, x);
        wxASSERT(PyCallable_Check(pyC));
        PyObject *pyR = PyObject_CallFunction(pyC, NULL);
        if (pyR != NULL) {
            Py_CLEAR(pyR);
        } else
            mypyPrintErr(), PyErr_Clear();
    }

    PyGILState_Release(gstate);
}

void MypyElfLoaded()
{
    mypyClearClient();
    mypyEnsureMod(wxsFormat(L"monitor.%08X", ElfCRC));
}

void MypyOnResume()
{
    if (!s_lastSuspend)
        return;
    s_lastSuspend = false;

    mypyClearClient();
    mypyEnsureMod(wxsFormat(L"monitor.%08X", ElfCRC));

    PyGILState_STATE gstate = PyGILState_Ensure();

    wxASSERT(PyList_Check(s_pylResume));

    int x = 0, cx = PyList_Size(s_pylResume);
    for (; x < cx; x++) {
        PyObject *pyC = PyList_GetItem(s_pylResume, x);
        wxASSERT(PyCallable_Check(pyC));
        PyObject *pyR = PyObject_CallFunction(pyC, NULL);
        if (pyR != NULL) {
            Py_CLEAR(pyR);
        } else
            mypyPrintErr(), PyErr_Clear();
    }

    PyGILState_Release(gstate);
}

int MypyHitBrk()
{
    {
        s_newbp = false;
        s_newpc = false;

        int index = s_mypyHitBrk & 255;
        PyObject *pyfn = s_mypyBrk[index].pyCb;

        PyGILState_STATE gstate = PyGILState_Ensure();

        if (PyCallable_Check(pyfn) == 1) {
            PyObject *pyR = PyObject_CallObject(pyfn, NULL); // Return value: New reference
            if (pyR != NULL) {
                Py_CLEAR(pyR);
            } else {
                mypyPrintErr();
                PyErr_Clear();
            }
        }

        PyGILState_Release(gstate);

        if (s_newbp || s_newpc) {
            return (s_newbp ? 1 : 0) | (s_newpc ? 2 : 0);
        }
    }

    return 0;
}

int MypyTestRBrk()
{
    const u32 target = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;

    s_newbp = false;
    s_newpc = false;

    for (int x = 0; x < MAX_BRK; x++) {
        if (!s_mypyRBrk[x].Test(target)) {
            continue;
        }

        int index = x;
        PyObject *pyfn = s_mypyRBrk[index].pyCb;

        PyGILState_STATE gstate = PyGILState_Ensure();

        if (PyCallable_Check(pyfn) == 1) {
            PyObject *pyR = PyObject_CallFunction(pyfn, "(I)", target); // Return value: New reference
            if (pyR != NULL) {
                Py_CLEAR(pyR);
            } else {
                mypyPrintErr();
                PyErr_Clear();
            }
        }

        PyGILState_Release(gstate);
    }

    return (s_newbp ? 1 : 0) | (s_newpc ? 2 : 0);
}

int MypyTestWBrk()
{
    const u32 target = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;

    s_newbp = false;
    s_newpc = false;

    for (int x = 0; x < MAX_BRK; x++) {
        if (!s_mypyWBrk[x].Test(target)) {
            continue;
        }

        int index = x;
        PyObject *pyfn = s_mypyWBrk[index].pyCb;

        PyGILState_STATE gstate = PyGILState_Ensure();

        if (PyCallable_Check(pyfn) == 1) {
            PyObject *pyR = PyObject_CallFunction(pyfn, "(I)", target); // Return value: New reference
            if (pyR != NULL) {
                Py_CLEAR(pyR);
            } else {
                mypyPrintErr();
                PyErr_Clear();
            }
        }

        PyGILState_Release(gstate);
    }

    return (s_newbp ? 1 : 0) | (s_newpc ? 2 : 0);
}

extern VURegs vuRegs[2];

// mask=0: write diff
// mask=1: write full
bool MypyWriteEETrace(int mask)
{
    if (s_feet == NULL) {
        return false;
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
        writer.WriteAlways(&s_mypy_pc, 4);
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

u32 s_mypy_rwTrace = 0;

bool mypyFlushRWTrace()
{
	if (s_rwTraceIdx == 0) {
		return true;
	}
	if (Py_None == s_pycRWTrace) {
		s_rwTraceIdx = 0;
		return true;
	}

	wxASSERT(PyCallable_Check(s_pycRWTrace));

	PyObject *pyBuff = PyBytes_FromStringAndSize((const char *)s_rwTraceBuff, 12 * s_rwTraceIdx);
	if (pyBuff != NULL) {
		PyObject *pyRes = PyObject_CallFunction(s_pycRWTrace, "(O)", pyBuff); // Return value: New reference
		Py_CLEAR(pyBuff);
		if (pyRes != NULL) {
			Py_CLEAR(pyRes);
			s_rwTraceIdx = 0;
			return true;
		}
	}

	mypyPrintErr();
    PyErr_Clear();

    s_rwTraceIdx = 0;
    s_mypy_rwTrace = 0;

    return false;
}

void __cdecl mypyRecordRW(int flags)
{
    if (s_mypy_rwTrace == 0) {
        // stop recording by py err.
        return;
    }

    if (s_rwTraceIdx >= TRACE_MAX) {
        PyGILState_STATE gstate = PyGILState_Ensure();
        bool ok = mypyFlushRWTrace();
        PyGILState_Release(gstate);
        if (!ok) {
            return;
        }
    }

    int pos = 3 * s_rwTraceIdx;
    s_rwTraceBuff[pos + 0] = s_mypy_pc;
    s_rwTraceBuff[pos + 1] = cpuRegs.GPR.r[_Rs_].SL[0] + _Imm_;
    s_rwTraceBuff[pos + 2] = flags;
	++s_rwTraceIdx;
}

void mypyResetCpu() {
	Cpu->Reset();
}

// --kkdf2