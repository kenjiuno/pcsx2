// kkdf2--
#include "PrecompiledHeader.h"
#include "System.h"
#include "Memory.h"
#include "Hw.h"
#include "R5900.h"
#include "Utilities/Path.h"
#include "gui/AppConfig.h"

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
    wxFileName injectFrom =
        PathDefs::GetProgramDataDir() + wxFileName(wxString::Format(L"inject.%08x\\%s", ElfCRC, WX_STR(wxString(s_fn))), wxPATH_NATIVE);

    if (injectFrom.FileExists()) {
        wxFile file(injectFrom.GetFullPath());

        wxFileOffset fileSize = file.Length();

        ssize_t readSize = file.Read(PSM(s_addrData), fileSize);

        if (readSize == fileSize) {
            Console.WriteLn(Color_StrongCyan, "#     S_IEXPA: Inject ok. %u ", 0U + readSize);
            return readSize;
        } else {
            Console.WriteLn(Color_StrongCyan, "#     S_IEXPA: Inject failed ", 0U + readSize);
        }
    }

    return 0;
}

u32 Haxkh2fm_trap_S_IEXPA_rec()
{
    s_fn[32] = 0;
    for (int x = 0; x < 32; x++) {
        s_fn[x] = memRead8(cpuRegs.GPR.n.a0.UL[0] + x);
    }

    s_addrData = cpuRegs.GPR.n.a1.UL[0];

    Console.WriteLn(Color_StrongCyan, "# rec.S_IEXPA: %08x  %s ", s_addrData, s_fn);

    return g_injectSize = inject_S_IEXPA();
}

u32 Haxkh2fm_trap_S_IEXPA_int()
{
    s_fn[32] = 0;
    for (int x = 0; x < 32; x++) {
        s_fn[x] = memRead8(cpuRegs.GPR.n.a0.UL[0] + x);
    }

    s_addrData = cpuRegs.GPR.n.a1.UL[0];

    Console.WriteLn(Color_StrongCyan, "# int.S_IEXPA: %08x  %s ", s_addrData, s_fn);

    return g_injectSize = inject_S_IEXPA();
}

void Haxkh2fm_trap_E_IEXPA()
{
    if (s_fn[0] != 0 && s_addrData != 0 && g_injectSize == 0) {
        const u32 sizeDest = cpuRegs.GPR.n.v0.UL[0];
        {
            // extraction
            wxFileName extractTo =
                PathDefs::GetProgramDataDir() + wxFileName(wxString::Format(L"expa.%08x\\%s", ElfCRC, WX_STR(wxString(s_fn))), wxPATH_NATIVE);

            if (extractTo.Mkdir(511, wxPATH_MKDIR_FULL)) {
                wxFile file(extractTo.GetFullPath(), wxFile::write);

                if (file.IsOpened()) {
                    file.Write(PSM(s_addrData), sizeDest);
                    file.Close();

                    Console.WriteLn(Color_StrongCyan, "#     E_IEXPA: Out ok. ");
                }
            }
        }
    }

    s_fn[0] = 0;
    s_addrData = 0;
}

void Haxkh2fm_trap_S_FINDX_int()
{
    s_findfn[32] = 0;
    for (int x = 0; x < 32; x++) {
        s_findfn[x] = memRead8(cpuRegs.GPR.n.a1.UL[0] + x);
    }
}

void Haxkh2fm_trap_S_FINDX_rec()
{
    s_findfn[32] = 0;
    for (int x = 0; x < 32; x++) {
        s_findfn[x] = memRead8(cpuRegs.GPR.n.a1.UL[0] + x);
    }
}

void Haxkh2fm_trap_E_FINDX()
{
    wxFileName injectFrom =
        PathDefs::GetProgramDataDir() + wxFileName(wxString::Format(L"inject.%08x\\%s", ElfCRC, WX_STR(wxString(s_findfn))), wxPATH_NATIVE);

    if (injectFrom.FileExists()) {
        u32 addr = cpuRegs.GPR.n.v0.UL[0];
        if (addr != 0) {
            u32 sizeFrm = memRead32(addr + 12);
            u32 sizeNew = injectFrom.GetSize().ToULong();

            if (sizeFrm != sizeNew) {
                memWrite32(addr + 12, sizeNew);
                Console.WriteLn(Color_StrongCyan, "#     E_FINDX: Entry %s resized to %u (from %u) ", s_findfn, sizeNew, sizeFrm);
            }
        }
    }
}

// --kkdf2
