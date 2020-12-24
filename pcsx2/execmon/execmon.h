#pragma once

#include <python.h>
#include <map>
#include <vector>

namespace execmon
{
extern u32 currentPc;
extern u32 s_mypy_opc;

void __cdecl encounterLoadOp();
void __cdecl encounterStoreOp();
void __cdecl invokeCpuReset();
bool testAnyInjection();

namespace exitstatus
{
extern bool s_newbp;
extern bool s_newpc;
extern u32 s_mypy_new_pc;

enum EatenFlags {
    resetCpu = 1,
    Deorbit = 2, // set pc to new
};

void __cdecl clear();
int eject();
} // namespace exitstatus

namespace utils
{
struct ReleasePyObject
{
    void operator()(PyObject *it)
    {
        Py_XDECREF(it);
    }
};

struct ReleasePyMem
{
    void operator()(void *ptr)
    {
        PyMem_Free(ptr);
    }
};
typedef std::unique_ptr<PyObject, ReleasePyObject> UniquePyObject;
} // namespace utils
namespace brk
{
struct Item
{
    int key;
    u32 pc;
    utils::UniquePyObject callable;
};
typedef std::multimap<u32, Item> Items;
extern Items items;
int add(u32 pc, PyObject *callable);
void __cdecl invoke();
} // namespace brk

namespace brkCommon
{
struct Item
{
    int key;
    u32 addr;
    u32 size;
    utils::UniquePyObject callable;

    inline bool ifHit(u32 target)
    {
        return (target - addr < size);
    }
};
typedef std::vector<Item> Items;
} // namespace brkCommon
namespace rbrk
{
typedef brkCommon::Item Item;
typedef brkCommon::Items Items;
extern brkCommon::Items items;
int add(u32 addr, u32 size, PyObject *callable);
bool remove(int key);
void __cdecl invoke();
} // namespace rbrk
namespace wbrk
{
typedef brkCommon::Item Item;
typedef brkCommon::Items Items;
extern brkCommon::Items items;
int add(u32 addr, u32 size, PyObject *callable);
bool remove(int key);
void __cdecl invoke();
} // namespace wbrk

namespace py
{
void init();
void finalize();

void elfLoaded();

void movetoSuspend();
void movetoResume();
} // namespace py

struct RWTrace
{
    RWTrace()
        : s_mypy_rwTrace(0)
    {
    }

    static const int MaxTrace = 4096;

    int s_mypy_rwTrace;
    int s_rwTraceIdx;
    u32 s_rwTraceBuff[3 * MaxTrace];

    inline const void *buffPtr() { return s_rwTraceBuff; }
    inline int buffSize() { return 3 * s_rwTraceIdx; }

    inline bool traceRead() { return 0 != (1 & s_mypy_rwTrace); }

    void rewindBuff()
    {
        s_rwTraceIdx = 0;
    }

    void reset()
    {
        s_mypy_rwTrace = 0;
        s_rwTraceIdx = 0;
    }

    void setFlags(int flags)
    {
        s_mypy_rwTrace = flags;
    }

    void mypyRecordRW(int flags);
};

extern RWTrace rwtrace;

struct EETrace
{
    EETrace()
        : s_feet(NULL)
        , s_tracpos(0)
    {
    }

    FILE *s_feet;
    u32 s_tracpos;

    void close()
    {
        if (s_feet != NULL) {
            fclose(s_feet);
            s_feet = NULL;
        }

        s_tracpos = 0;
    }

    bool open(LPCWSTR pzwOutput)
    {
        close();
        s_feet = _wfopen(pzwOutput, L"wb");
        return s_feet != NULL;
    }

    bool MypyWriteEETrace(int mask);
};

extern EETrace eetrace;

} // namespace execmon
