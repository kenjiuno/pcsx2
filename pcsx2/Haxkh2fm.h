#ifndef _Haxkh2fm_
#define _Haxkh2fm_

u32 Haxkh2fm_trap_S_IEXPA_rec(void);
u32 Haxkh2fm_trap_S_IEXPA_int(void);
void Haxkh2fm_trap_E_IEXPA(void);

void Haxkh2fm_trap_S_FINDX_int(void);
void Haxkh2fm_trap_S_FINDX_rec();
void Haxkh2fm_trap_E_FINDX(void);

extern u32 g_injectSize;

#endif // _Haxkh2fm_
