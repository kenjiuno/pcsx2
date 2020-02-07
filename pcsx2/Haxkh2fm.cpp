// kkdf2--
#include "PrecompiledHeader.h"
#include "System.h"
#include "Memory.h"
#include "Hw.h"
#include "R5900.h"

#include "Haxkh2fm.h"
#include <stdio.h>
#include <map>
#include <winsock.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

extern u32 ElfCRC;

static char s_fn[32 + 1] = {0};
static u32 s_addrData = 0;

static char s_findfn[32 + 1] = {0};

u32 g_injectSize = 0; // 0=expose-data, not 0=do-not-expose-data

u32 inject_S_IEXPA()
{
    char fin[128 + 1];
    FILE *f;
    int i;
    _snprintf(fin, 128, "inject.%08x\\%s", ElfCRC, s_fn);
    fin[128] = 0;
    for (i = 0; fin[i] != 0; i++) {
        if (fin[i] == '/')
            fin[i] = '\\';
    }

    f = fopen(fin, "rb");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        long fileSize = ftell(f);
        rewind(f);
        size_t cnt = fread(PSM(s_addrData), 1, fileSize, f);

        if (cnt == fileSize) {
            Console.WriteLn(Color_StrongCyan, "#     S_IEXPA: Inject ok. %u ", 0U + cnt);
            fclose(f);

            return cnt;
        } else {
            Console.WriteLn(Color_StrongCyan, "#     S_IEXPA: Inject failed ", 0U + cnt);
        }

        fclose(f);
    }

    return 0;
}

u32 Haxkh2fm_trap_S_IEXPA_rec()
{
    s_fn[32] = 0;
    for (int x = 0; x < 32; x++)
        s_fn[x] = memRead8(cpuRegs.GPR.n.a0.UL[0] + x);

    s_addrData = cpuRegs.GPR.n.a1.UL[0];

    Console.WriteLn(Color_StrongCyan, "# rec.S_IEXPA: %08x  %s ", s_addrData, s_fn);

    return g_injectSize = inject_S_IEXPA();
}

u32 Haxkh2fm_trap_S_IEXPA_int()
{
    s_fn[32] = 0;
    for (int x = 0; x < 32; x++)
        s_fn[x] = memRead8(cpuRegs.GPR.n.a0.UL[0] + x);

    s_addrData = cpuRegs.GPR.n.a1.UL[0];

    Console.WriteLn(Color_StrongCyan, "# int.S_IEXPA: %08x  %s ", s_addrData, s_fn);

    return g_injectSize = inject_S_IEXPA();
}

static void prepdir(char *fout)
{
    char *p = fout;
    while (true) {
        p = strchr(p, '\\');
        if (p == NULL)
            break;
        *p = 0;
        _mkdir(fout);
        *p = '\\';
        p++;
    }
}

void Haxkh2fm_trap_E_IEXPA()
{
    if (s_fn[0] != 0 && s_addrData != 0 && g_injectSize == 0) {
        const u32 sizeDest = cpuRegs.GPR.n.v0.UL[0];
        {
            // extraction

            char fout[128 + 1];
            FILE *f;
            int i;
            _snprintf(fout, 128, "expa.%08x\\%s", ElfCRC, s_fn);
            fout[128] = 0;
            for (i = 0; fout[i] != 0; i++) {
                if (fout[i] == '/')
                    fout[i] = '\\';
            }

            prepdir(fout);

            f = fopen(fout, "wb");
            if (f != NULL) {
                //Console::Notice("# cb=%u ", params sizeDest);

                fwrite(PSM(s_addrData), sizeDest, 1, f);
                fclose(f);

                Console.WriteLn(Color_StrongCyan, "#     E_IEXPA: Out ok. ");
            }
        }
    }

    s_fn[0] = 0;
    s_addrData = 0;
}

void Haxkh2fm_trap_S_FINDX_int()
{
    s_findfn[32] = 0;
    for (int x = 0; x < 32; x++)
        s_findfn[x] = memRead8(cpuRegs.GPR.n.a1.UL[0] + x);
}

void Haxkh2fm_trap_S_FINDX_rec()
{
    s_findfn[32] = 0;
    for (int x = 0; x < 32; x++)
        s_findfn[x] = memRead8(cpuRegs.GPR.n.a1.UL[0] + x);
}

void Haxkh2fm_trap_E_FINDX()
{
    {
        // test

        char fin[128 + 1];
        int i;
        _snprintf(fin, 128, "inject.%08x\\%s", ElfCRC, s_findfn);
        fin[128] = 0;
        for (i = 0; fin[i] != 0; i++) {
            if (fin[i] == '/')
                fin[i] = '\\';
        }

        struct stat st;
        if (0 == stat(fin, &st)) {
            u32 addr = cpuRegs.GPR.n.v0.UL[0];
            if (addr != 0) {
                u32 sizeFrm = memRead32(addr + 12);
                u32 sizeNew = st.st_size;

                if (sizeFrm != sizeNew) {
                    memWrite32(addr + 12, sizeNew);
                    Console.WriteLn(Color_StrongCyan, "#     E_FINDX: Entry %s resized to %u (from %u) ", s_findfn, sizeNew, sizeFrm);
                }
            }
        }
    }
}

// --kkdf2
