
#ifndef _W2_H_
#define _W2_H_

namespace W2
{
extern bool ShowR5900(int f = 1);
extern bool ShowREGs(int f = 1);
extern bool ShowMEM(int f = 1);
extern void UpdateScreens();
extern int WBrkptHit();
extern wxCondition s_evExecContinue;

extern int s_wbrkRes; // -> s_mypyEat

struct CWBrk
{
    u32 pc;

    CWBrk()
        : pc(0)
    {
    }

    void Set(uint pc);
    void Unset();
};

#define MAX_WBRK 51

extern CWBrk s_wbrk[MAX_WBRK];
} // namespace W2

#endif
