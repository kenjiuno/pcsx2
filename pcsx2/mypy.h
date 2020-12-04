#ifndef _MYPY_H_
#define _MYPY_H_

#include <python.h>

extern void MypyElfLoaded();
extern void MypyOnResume();
extern void MypyInit();
extern void MypyFinalize();
extern void MypyOnSuspend();

extern void mypyPrintErr();

#pragma pack(push, 4)

struct MypyBrk
{
    u32 pc;
    u32 len;
    u32 dummy;
    PyObject *pyCb; // callable

    MypyBrk()
        : pc(0)
        , pyCb(NULL)
        , len(0)
        , dummy(0)
    {
    }
    inline bool Test(u32 t)
    {
        return (t - pc < len);
    }
};

#pragma pack(pop)

#define MAX_BRK 32

extern MypyBrk s_mypyBrk[MAX_BRK];  // pc (program counter) break points.
extern MypyBrk s_mypyRBrk[MAX_BRK]; // read mem break points.
extern MypyBrk s_mypyWBrk[MAX_BRK]; // write mem break points.

extern int MypyHitBrk(); // 1=new bp appended (need calling Cpu->Clear).
extern u32 s_mypyHitBrk; // 256=inside-recomp, 512=possible-branch
extern u32 s_mypyEat;    // 1=recompile-after-running-current-instruction, 2=set-new-pc-before-running-current-instruction

extern int MypyTestRBrk();
extern u32 s_mypy_opc;     // Old pc (program counter) for int mode.
extern u32 s_mypy_pc;      // Current pc
extern u32 s_mypy_new_pc;  // New pc set from user py

extern int MypyTestWBrk();

extern bool MypyWriteEETrace(int mask);
extern FILE *s_feet;

#endif // _MYPY_H_
