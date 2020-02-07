
#include "PrecompiledHeader.h"
#include "Memory.h"

#include "R5900.h"
#include "VU.h"

#include <wx/wx.h>
#include <wx/notebook.h>
#include "App.h"

#include "W2.h"

#include "EEDisarm.h"

using namespace W2;

CWBrk W2::s_wbrk[MAX_WBRK];

static wxMutex s_mtxTmp;
wxCondition W2::s_evExecContinue(s_mtxTmp);

int W2::s_wbrkRes = 0;

#define IfValidPtr(X) (u32(X) >= 65536U)

enum {
    wxID_Base = 2001,
    wxID_R5900DBG = wxID_Base,
    wxID_REGs,
    wxID_MEM,

    wxID_EEC,
    wxID_Hexe,
    wxID_REGTabs,
    wxID_MemPane,
};

enum {
    wxIDC_Base = 2101,
    wxIDC_ShowREGs = wxIDC_Base,
    wxIDC_ShowMEM,
    wxIDC_Run,
    wxIDC_Step1,
    wxIDC_Run2Curs,
    wxIDC_Step2,
    wxIDC_GotoAddr,
    wxIDC_Bkpt,
    wxIDC_UpdateContents,
};

static void resetCodes(u32 pc, u32 size)
{
    if (Cpu != NULL)
        Cpu->Clear(pc & (~0xfffUL), 0x400);
}

class GaDlg : public wxDialog
{
public:
    GaDlg(wxWindow *parent, wxWindowID id, wxString strVal)
        : wxDialog(parent, id, L"Jump: address in hex")
    {
        wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);

        wxTextCtrl *text = new wxTextCtrl(
            this, 101, strVal //, wxDefaultPosition, wxDefaultSize, 0, m_tv
        );
        wxSize size = text->GetSize();
        text->SetSize(size.GetX() * 2, size.GetY());

        topsizer->Add(text);
        topsizer->Add(new wxButton(this, wxID_OK, L"OK"));
        topsizer->Add(new wxButton(this, wxID_CANCEL, L"Cancel"));

        SetSizer(topsizer);
        topsizer->SetSizeHints(this);

        Fit();
        Centre();
    }

    wxString retVal;

    void OnOk(wxCommandEvent &ev)
    {
        if (wxTextCtrl *text = (wxTextCtrl *)FindWindow(101))
            retVal = text->GetValue();
        ev.Skip();
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(GaDlg, wxDialog)
EVT_BUTTON(wxID_OK, OnOk)
END_EVENT_TABLE()

// Hex editor
class Hexe : public wxWindow
{
public:
    uint TopAddr;

    int cnty0, cnty1, cy1;

    Hexe(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id, wxDefaultPosition, wxSize(630, 50))
    {
        TopAddr = 0;
        cnty0 = cnty1 = cy1 = -1;
    }

    class HUt
    {
    public:
        static inline void AddHexs(u8 v, wxString &str)
        {
            static wxChar tbl[] = {L"0123456789ABCDEF"};
            str.Append(tbl[v >> 4]);
            str.Append(tbl[v & 15]);
        }
    };

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cnty0 = size.GetY() / cy1;
        cnty1 = (size.GetY() + cy1 - 1) / cy1;
        int vy = 0;
        uint pc = TopAddr;
        for (; vy < size.GetY(); vy += cy1) {
            wxString strHex, strStr;

            for (int x = 0; x < 16; x++) {
                u8 *p = reinterpret_cast<u8 *>(PSM(pc + x));
                u8 v = IfValidPtr(p) ? *p : 0;
                HUt::AddHexs(v, strHex);
                strHex += L' ';
                if (x == 8)
                    strHex += L' ';

                strStr += isprint(v) ? (wxChar)v : L'.';
            }

            dc.DrawText(wxsFormat(_("%08x: %s %s"), pc, WX_STR(strHex), WX_STR(strStr)), 2, 2 + vy);
            pc += 16;
        }
    }

    int CntDispLines(bool is0)
    {
        if (is0)
            if (cnty0 >= 0)
                return cnty0;
            else
                ;
        else if (cnty1 >= 0)
            return cnty1;

        wxSize size = GetClientSize();
        size.DecBy(2);
        wxClientDC dc(this);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cnty0 = size.GetY() / cy1;
        cnty1 = (size.GetY() + cy1 - 1) / cy1;
        this->cy1 = cy1;
        return is0 ? cnty0 : cnty1;
    }

    void OnKeyDown(wxKeyEvent &ev)
    {
        uint ta = TopAddr;
        uint cntLines = CntDispLines(true);
        uint bytesPage = cntLines * 16;

        switch (ev.GetKeyCode()) {
            case WXK_UP:
                if (ta >= 16)
                    ta -= 16;
                else
                    ta = 0;
                break;
            case WXK_LEFT:
                if (ta != 0)
                    ta--;
                break;
            case WXK_RIGHT:
                ta++;
                break;
            case WXK_DOWN:
                ta += 16;
                break;
            case WXK_PAGEUP:
                if (ta >= bytesPage) {
                    ta -= bytesPage;
                } else {
                    ta = 0;
                }
                break;
            case WXK_PAGEDOWN:
                ta += bytesPage;
                break;
            case 'G':
                ShowGoto();
                return;
        }
        Goto(ta);
    }

    void ShowGoto()
    {
        uint ta = TopAddr;

        GaDlg wndDlg(this, -1, wxsFormat(_("%08x"), ta));
        if (wndDlg.ShowModal() == wxID_OK) {
            unsigned long newta;
            if (wndDlg.retVal.ToULong(&newta, 16)) {
                ta = newta;
            }
        }

        Goto(ta);
    }

    void Goto(uint ta)
    {
        uint cntLines = CntDispLines(true);
        uint bytesPage = cntLines * 4;

        if (TopAddr != ta) {
            TopAddr = ta;
            Refresh();
        }
    }

    void OnMouseWheel(wxMouseEvent &ev)
    {
        uint ta = TopAddr;
        int delta = (ev.GetWheelRotation() / 30) * -4;
        if (delta < 0 && ta < (uint)-delta) {
            ta = 0;
        } else
            ta += delta;

        TopAddr = ta;
        Refresh();
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Hexe, wxWindow)
EVT_PAINT(OnPaint)
EVT_KEY_DOWN(OnKeyDown)
EVT_MOUSEWHEEL(OnMouseWheel)
END_EVENT_TABLE()

// Dump mem EE
class DmEE : public wxWindow
{
public:
    DmEE(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id)
    {
        wxFlexGridSizer *tops = new wxFlexGridSizer(3, 1, 0, 0);
        tops->AddGrowableCol(0);
        tops->AddGrowableRow(1);
        SetSizer(tops);
        {
            wxBoxSizer *adrs = new wxBoxSizer(wxHORIZONTAL);
            {
                adrs->Add(new wxButton(this, wxIDC_GotoAddr, L"Goto addr (G)"));
                adrs->Add(new wxButton(this, wxIDC_UpdateContents, L"Update"));
            }
            tops->Add(adrs);
        }
        {
            Hexe *hexe = new Hexe(this, wxID_Hexe);
            tops->Add(hexe, 1, wxALL | wxEXPAND);
        }
        {
            wxStaticBox *box = new wxStaticBox(this, -1, L"Memory Patch");
            wxStaticBoxSizer *boxs = new wxStaticBoxSizer(box, wxHORIZONTAL);
            {
                wxFlexGridSizer *gs = new wxFlexGridSizer(2, 2, 2, 2);
                {
                    gs->Add(new wxStaticText(this, -1, L"Address"), 0, wxALIGN_CENTER_VERTICAL);
                    gs->Add(new wxTextCtrl(this, -1, L"00000000"), 0, wxALIGN_CENTER_VERTICAL);
                    gs->Add(new wxStaticText(this, -1, L"Data"), 0, wxALIGN_CENTER_VERTICAL);
                    gs->Add(new wxTextCtrl(this, -1, L"00000000"), 0, wxALIGN_CENTER_VERTICAL);
                }
                boxs->Add(gs);
            }
            boxs->Add(new wxButton(this, -1, L"Patch It"), 0, wxALIGN_BOTTOM);
            tops->Add(boxs);
        }
        SetSizerAndFit(tops);
        //tops->Layout();
        //tops->SetSizeHints(this);
        //Fit();
    }

    void OnSize(wxSizeEvent &ev)
    {
        wxSizer *p = GetSizer();
        if (p != NULL) {
            wxRect rc = GetClientRect();
            p->SetDimension(rc.GetLeft(), rc.GetTop(), rc.GetWidth(), rc.GetHeight());
            p->Layout();
        }
    }

    void OnGotoAddr(wxCommandEvent &ev)
    {
        Hexe *p = wxStaticCast(FindWindowById(wxID_Hexe), Hexe);
        if (p != NULL)
            p->ShowGoto(),
                p->SetFocus();
    }

    void OnUpdateContents(wxCommandEvent &ev)
    {
        Hexe *p = wxStaticCast(FindWindowById(wxID_Hexe), Hexe);
        if (p != NULL)
            p->Refresh();
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DmEE, wxWindow)
EVT_BUTTON(wxIDC_GotoAddr, OnGotoAddr)
EVT_BUTTON(wxIDC_UpdateContents, OnUpdateContents)
EVT_SIZE(OnSize)
END_EVENT_TABLE()

// Display registers of EmotionEngine
class DrEE : public wxWindow
{
public:
    DrEE(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id)
    {
    }

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        //dc.SetTextForeground(*wxBLACK);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cy1 += 2;
        int y = 0;
        for (; y < 32; y++) {
            int i = y;
            dc.DrawText(wxsFormat(_("%-4s 0x%08x_%08x_%08x_%08x"), EEDisarm::eedNameGPR[i], cpuRegs.GPR.r[i].UL[3], cpuRegs.GPR.r[i].UL[2], cpuRegs.GPR.r[i].UL[1], cpuRegs.GPR.r[i].UL[0]), 2, 2 + cy1 * y);
        }
        {
            dc.DrawText(wxsFormat(_("%-4s 0x%08x_%08x_%08x_%08x"), L"HI", cpuRegs.HI.UL[3], cpuRegs.HI.UL[2], cpuRegs.HI.UL[1], cpuRegs.HI.UL[0]), 2, 2 + cy1 * y);
            y++;
        }
        {
            dc.DrawText(wxsFormat(_("%-4s 0x%08x_%08x_%08x_%08x"), L"LO", cpuRegs.LO.UL[3], cpuRegs.LO.UL[2], cpuRegs.LO.UL[1], cpuRegs.LO.UL[0]), 2, 2 + cy1 * y);
            y++;
        }
        {
            dc.DrawText(wxsFormat(_("%-4s 0x%08x"), L"pc", cpuRegs.pc), 2, 2 + cy1 * y);
        }
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrEE, wxWindow)
EVT_PAINT(OnPaint)
END_EVENT_TABLE()

// Display registers of COP0
class DrCOP0 : public wxWindow
{
public:
    DrCOP0(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id)
    {
    }

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cy1 += 2;
        int y = 0;
        static wxChar *names[32] = {
            L"Index",
            L"Random",
            L"EntryLo0",
            L"EntryLo1",
            L"Context",
            L"PageMask",
            L"Wired",
            L"Reserved0",
            L"BadVAddr",
            L"Count",
            L"EntryHi",
            L"Compare",
            L"Status",
            L"Cause",
            L"EPC",
            L"PRid",
            L"Config",
            L"LLAddr",
            L"WatchLO",
            L"WatchHI",
            L"XContext",
            L"Reserved1",
            L"Reserved2",
            L"Debug",
            L"DEPC",
            L"PerfCnt",
            L"ErrCtl",
            L"CacheErr",
            L"TagLo",
            L"TagHi",
            L"ErrorEPC",
            L"DESAVE",
        };
        for (; y < 32; y++) {
            int i = y;
            dc.DrawText(wxsFormat(_("%10s %x"), names[y], cpuRegs.CP0.r[i]), 2, 2 + cy1 * y);
        }
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrCOP0, wxWindow)
EVT_PAINT(OnPaint)
END_EVENT_TABLE()

// Display registers of COP1
class DrCOP1 : public wxWindow
{
public:
    DrCOP1(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id)
    {
    }

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cy1 += 2;
        int y = 0;
        for (; y < 32; y++) {
            int i = y;
            dc.DrawText(wxsFormat(_("FP%-2d  %f"), y, fpuRegs.fpr[i].f), 2, 2 + cy1 * y);
        }
        {
            dc.DrawText(wxsFormat(_("FCR0  %x"), y, fpuRegs.fprc[0]), 2, 2 + cy1 * y);
            y++;
            dc.DrawText(wxsFormat(_("FCR31 %x"), y, fpuRegs.fprc[31]), 2, 2 + cy1 * y);
            y++;
            dc.DrawText(wxsFormat(_("Accum %f"), fpuRegs.ACC.f), 2, 2 + cy1 * y);
            y++;
        }
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrCOP1, wxWindow)
EVT_PAINT(OnPaint)
END_EVENT_TABLE()

// Display registers of COP2F
class DrCOP2F : public wxWindow
{
public:
    DrCOP2F(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id)
    {
    }

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cy1 += 2;
        int y = 0;
        for (; y < 32; y++) {
            int i = y;
            dc.DrawText(wxsFormat(_("VF%02d  %f_%f_%f_%f"), y, vuRegs[i].VF[y].F[0], vuRegs[i].VF[y].F[1], vuRegs[i].VF[y].F[2], vuRegs[i].VF[y].F[3]), 2, 2 + cy1 * y);
        }
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DrCOP2F, wxWindow)
EVT_PAINT(OnPaint)
END_EVENT_TABLE()

// Dis asm EmotionEngine viewer Control
class DaEEC : public wxWindow
{
public:
    uint TopAddr;
    uint SelAddr;

    int cnty0;
    int cnty1;
    int cy1;

    DaEEC(wxWindow *parent, wxWindowID id)
        : wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
        , cnty0(-1)
        , cnty1(-1)
        , cy1(-1)
    {
        SelAddr = TopAddr = 0x00100000;
    }

    bool IsBP(uint pc)
    {
        for (int x = 1; x < MAX_WBRK && s_wbrk[x].pc != 0; x++) {
            if (s_wbrk[x].pc == pc)
                return true;
        }
        return false;
    }
    bool IsHere(uint pc)
    {
        return cpuRegs.pc == pc;
    }

    void OnPaint(wxPaintEvent &ev)
    {
        wxPaintDC dc(this);
        dc.SetBackgroundMode(wxTRANSPARENT);
        //dc.SetTextForeground(*wxBLACK);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxSize size = GetClientSize();
        size.DecBy(2);
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cnty0 = size.GetY() / cy1;
        cnty1 = (size.GetY() + cy1 - 1) / cy1;
        this->cy1 = cy1;
        u32 pc = TopAddr;
        wxBrush br(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        dc.SetBrush(br);
        dc.SetPen(pen);
        wxColour clrFore = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
        wxColour clrBk = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
        wxColour clrHFore = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
        wxColour clrHBk = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
        for (int y = 0; y < size.GetY(); y += cy1) {
            wxRect rc(2 + 0, 2 + y, size.GetX() - 4, cy1);
            bool fSel = (pc == SelAddr);
            if (fSel) {
                dc.DrawRectangle(rc.GetLeft(), rc.GetTop(), rc.GetWidth(), rc.GetHeight());
            }
            dc.SetTextForeground(fSel ? clrHFore : clrFore);
            const u8 *pb = reinterpret_cast<const u8 *>(PSM(pc));
            u32 code = IfValidPtr(pb) ? *reinterpret_cast<const u32 *>(pb) : 0;
            EEDisarm::EEis eeis;
            EEDisarm::Parse(code, pc, eeis);
            dc.DrawText(wxsFormat(_("%c%c %08x %08x  %s"), IsBP(pc) ? L'B' : L' ', IsHere(pc) ? L'>' : L' ', pc, code, WX_STR(eeis.ToString())), rc.GetLeft(), rc.GetTop());
            pc += 4;
        }
    }

    int CntDispLines(bool is0)
    {
        if (is0)
            if (cnty0 >= 0)
                return cnty0;
            else
                ;
        else if (cnty1 >= 0)
            return cnty1;

        wxSize size = GetClientSize();
        size.DecBy(2);
        wxClientDC dc(this);
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
        wxCoord cx1, cy1;
        dc.GetTextExtent(L"0123456789", &cx1, &cy1);
        cnty0 = size.GetY() / cy1;
        cnty1 = (size.GetY() + cy1 - 1) / cy1;
        this->cy1 = cy1;
        return is0 ? cnty0 : cnty1;
    }

    void OnKeyDown(wxKeyEvent &ev)
    {
        uint sa = SelAddr;
        uint ta = TopAddr;
        uint cntLines = CntDispLines(true);
        uint bytesPage = cntLines * 4;

        switch (ev.GetKeyCode()) {
            case WXK_UP:
                if (ev.ShiftDown()) {
                    if (sa != ta)
                        ta += 4;
                } else {
                    if (sa >= 4)
                        sa -= 4;
                }
                break;
            case WXK_DOWN:
                if (ev.ShiftDown()) {
                    if (sa < ta + bytesPage - 4)
                        ta -= 4;
                } else {
                    sa += 4;
                }
                break;
            case WXK_PAGEUP:
                if (ta >= bytesPage) {
                    ta -= bytesPage;
                    sa -= bytesPage;
                } else {
                    ta = 0;
                    sa = 0;
                }
                break;
            case WXK_PAGEDOWN:
                ta += bytesPage;
                sa += bytesPage;
                break;
            case 'G':
                ShowGoto();
                return;
            case WXK_F5:
                if (!ev.ControlDown() && !ev.ShiftDown() && !ev.MetaDown()) {
                    Step(-1);
                }
                return;
            case WXK_F9:
                if (!ev.ControlDown() && !ev.ShiftDown() && !ev.MetaDown()) {
                    Bkpt();
                }
                return;
            case WXK_F11:
                if (!ev.ControlDown() && !ev.ShiftDown() && !ev.MetaDown()) {
                    Step(1);
                }
                return;
            case WXK_F10:
                if (!ev.ControlDown() && !ev.ShiftDown() && !ev.MetaDown()) {
                    Step(2);
                } else if (ev.ControlDown() && !ev.ShiftDown() && !ev.MetaDown()) {
                    Run2Curs();
                }
                return;
        }
        Goto(sa, ta);
    }

    void ShowGoto()
    {
        uint sa = SelAddr;

        GaDlg wndDlg(this, -1, wxsFormat(_("%08x"), sa));
        if (wndDlg.ShowModal() == wxID_OK) {
            unsigned long newta;
            if (wndDlg.retVal.ToULong(&newta, 16)) {
                sa = newta & (~3);
            }
        }

        Goto(sa);
    }

    void Goto(uint sa)
    {
        Goto(sa, TopAddr);
    }

    void Goto(uint sa, uint ta)
    {
        uint cntLines = CntDispLines(true);
        uint bytesPage = cntLines * 4;

        if (SelAddr != sa) {
            SelAddr = sa;
            if (sa < ta) {
                ta = sa;
            } else if (ta + bytesPage <= sa) {
                ta = sa - bytesPage + 4;
            }
            Refresh();
        }
        if (TopAddr != ta) {
            TopAddr = ta;
            Refresh();
        }
    }

    void OnMouseWheel(wxMouseEvent &ev)
    {
        uint ta = TopAddr;
        int delta = (ev.GetWheelRotation() / 30) * -4;
        if (delta < 0 && ta < (uint)-delta) {
            ta = 0;
        } else
            ta += delta;

        TopAddr = ta;
        Refresh();
    }

    void OnLeftDown(wxMouseEvent &ev)
    {
        ev.Skip();
        int cnty = CntDispLines(false);
        int y = ev.GetY() / cy1;
        if (y < 0 || y >= cnty)
            return;
        int pos = y;
        uint sa = TopAddr + 4 * pos;
        if (SelAddr != sa) {
            SelAddr = sa;
            Refresh();
        }
    }

    void Step(int mode)
    { // -1=run, 1=step into, 2=step over
        switch (mode) {
            case -1:
                s_wbrk[0].Unset();
                break;
            case 1:
                s_wbrk[0].Set(cpuRegs.pc + 4);
                break;
            case 2:
                s_wbrk[0].Set(cpuRegs.pc + 4);
                break;
        }
        s_evExecContinue.Signal();
    }
    void Run2Curs()
    {
    }
    void Bkpt()
    {
        int fi = -1, ci = -1, maxi = -1;
        for (int x = 1; x < MAX_WBRK; x++) {
            if (s_wbrk[x].pc == 0) {
                if (fi == -1)
                    fi = x;
            } else
                maxi = x;
            if (s_wbrk[x].pc == SelAddr) {
                ci = x;
            }
        }
        if (ci != -1) {
            if (ci != maxi) {
                s_wbrk[ci] = s_wbrk[maxi];
            }
            s_wbrk[maxi].Unset();

            Refresh();
            return;
        }
        if (fi != -1) {
            s_wbrk[fi].Set(SelAddr);
            Refresh();
            return;
        }
        wxMessageBox(L"No more bkpt allowed!", L"pcsx2", wxICON_EXCLAMATION);
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(DaEEC, wxWindow)
EVT_PAINT(OnPaint)
EVT_KEY_DOWN(OnKeyDown)
EVT_MOUSEWHEEL(OnMouseWheel)
EVT_LEFT_DOWN(OnLeftDown)
END_EVENT_TABLE()

class FwEE : public wxFrame
{
public:
    FwEE(wxWindow *parent,
         wxWindowID id,
         const wxString &title,
         const wxPoint &pos = wxDefaultPosition,
         const wxSize &size = wxDefaultSize,
         long style = wxDEFAULT_FRAME_STYLE,
         const wxString &name = wxFrameNameStr)
        : wxFrame(parent, id, title, pos, size, style, name)
    {
        wxFlexGridSizer *tops = new wxFlexGridSizer(3, 1, 0, 0);
        tops->AddGrowableCol(0);
        tops->AddGrowableRow(2);

        SetSizer(tops);

        {
            wxBoxSizer *upp = new wxBoxSizer(wxHORIZONTAL);
            tops->Add(upp);
            upp->Add(new wxButton(this, wxIDC_Run, L"Run (F5)"));
            upp->Add(new wxButton(this, wxIDC_Step1, L"Step Into (F11)"));
            upp->Add(new wxButton(this, wxIDC_Step2, L"Step Over (F10)"));
            upp->Add(new wxButton(this, wxIDC_Run2Curs, L"Run to Cursor (Ctrl+F10)"));
            upp->Add(new wxButton(this, wxIDC_Bkpt, L"Bkpt (F9)"));
        }

        {
            wxBoxSizer *upp = new wxBoxSizer(wxHORIZONTAL);
            tops->Add(upp);
            upp->Add(new wxButton(this, wxIDC_GotoAddr, L"Goto addr (G)"));
            upp->Add(new wxButton(this, wxIDC_ShowREGs, L"REGs"));
            upp->Add(new wxButton(this, wxIDC_ShowMEM, L"MEM"));
            upp->Add(new wxButton(this, wxIDC_UpdateContents, L"Update"));
        }

        DaEEC *b1 = new DaEEC(
            this, wxID_EEC);

        tops->Add(b1, 1, wxEXPAND);

        b1->SetFocus();
    }

    void Upd()
    {
        wxWindow *p = FindWindowById(wxID_EEC);
        if (p != NULL)
            p->Refresh();
    }

    void OnSize(wxSizeEvent &ev)
    {
        wxSizer *p = GetSizer();
        if (p != NULL) {
            wxRect rc = GetClientRect();
            p->SetDimension(rc.GetLeft(), rc.GetTop(), rc.GetWidth(), rc.GetHeight());
            p->Layout();
        }
    }

    void OnShowREGs(wxCommandEvent &ev)
    {
        ShowREGs(ShowREGs(0) ? -1 : 1);
    }
    void OnShowMEM(wxCommandEvent &ev)
    {
        ShowMEM(ShowMEM(0) ? -1 : 1);
    }
    void OnStep1(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Step(1);
            p->SetFocus();
        }
    }
    void OnStep2(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Step(2);
            p->SetFocus();
        }
    }
    void OnRun(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Step(-1);
            p->SetFocus();
        }
    }
    void OnRun2Curs(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Run2Curs();
            p->SetFocus();
        }
    }
    void OnBkpt(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Bkpt();
            p->SetFocus();
        }
    }
    void OnGotoAddr(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->ShowGoto();
            p->SetFocus();
        }
    }
    void OnUpdateContents(wxCommandEvent &ev)
    {
        DaEEC *p = wxStaticCast(FindWindowById(wxID_EEC, this), DaEEC);
        if (p != NULL) {
            p->Refresh();
        }
    }

protected:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(FwEE, wxFrame)
EVT_BUTTON(wxIDC_ShowREGs, OnShowREGs)
EVT_BUTTON(wxIDC_ShowMEM, OnShowMEM)
EVT_BUTTON(wxIDC_Step1, OnStep1)
EVT_BUTTON(wxIDC_Step2, OnStep2)
EVT_BUTTON(wxIDC_Run, OnRun)
EVT_BUTTON(wxIDC_Run2Curs, OnRun2Curs)
EVT_BUTTON(wxIDC_GotoAddr, OnGotoAddr)
EVT_BUTTON(wxIDC_Bkpt, OnBkpt)
EVT_BUTTON(wxIDC_UpdateContents, OnUpdateContents)
EVT_SIZE(OnSize)
END_EVENT_TABLE()

bool W2::ShowR5900(int f)
{
    wxFrame *frame = static_cast<wxFrame *>(wxWindow::FindWindowById(wxID_R5900DBG));
    if (f > 0) {
        if (frame == NULL) {
            frame = new FwEE(
                NULL,
                wxID_R5900DBG,
                wxT("R5900 Debugger"),
                wxPoint(50, 50),
                wxSize(600, 550));

            frame->Show();
        }
    } else if (f < 0) {
        if (frame != NULL) {
            frame->Destroy();
        }

        return true;
    } else {
    }
    return frame != NULL;
}
bool W2::ShowREGs(int f)
{
    wxWindow *frame = static_cast<wxFrame *>(wxWindow::FindWindowById(wxID_REGs));
    if (f > 0) {
        if (frame == NULL) {
            frame = new wxFrame(
                NULL, wxID_REGs,
                wxT("Debugger"),
                wxPoint(50 + 600 + 20, 50),
                wxSize(380, 550));
            wxNotebook *tabs = new wxNotebook(
                frame, wxID_REGTabs);

            tabs->AddPage(new DrEE(tabs, -1), L"R5900");
            tabs->AddPage(new DrCOP0(tabs, -1), L"COP0");
            tabs->AddPage(new DrCOP1(tabs, -1), L"COP1");
            tabs->AddPage(new DrCOP2F(tabs, -1), L"COP2F");

            frame->Show();
        }
    } else if (f < 0) {
        if (frame != NULL) {
            frame->Destroy();
        }

        return true;
    } else {
    }
    return frame != NULL;
}
bool W2::ShowMEM(int f)
{
    wxWindow *frame = static_cast<wxFrame *>(wxWindow::FindWindowById(wxID_MEM));
    if (f > 0) {
        if (frame == NULL) {
            frame = new wxFrame(
                NULL, wxID_MEM,
                wxT("Memory"),
                wxPoint(50, 50 + 550),
                wxSize(690, 240));

            DmEE *b3 = new DmEE(
                frame, wxID_MemPane);

            frame->Show();
        }
    } else if (f < 0) {
        if (frame != NULL) {
            frame->Destroy();
        }

        return true;
    } else {
    }
    return frame != NULL;
}

void _UpdateScreens()
{
    {
        wxWindow *targetWindow = wxWindow::FindWindowById(wxID_R5900DBG);
        if (targetWindow != nullptr) {
            wxStaticCast(targetWindow, FwEE)->Upd();
        }
    }
    {
        wxWindow *targetWindow = wxWindow::FindWindowById(wxID_REGs);
        if (targetWindow != nullptr) {
            wxFrame *frame = wxStaticCast(targetWindow, wxFrame);
            wxWindow *tabs = wxWindow::FindWindowById(wxID_REGTabs, frame);
            if (tabs != nullptr) {
                wxStaticCast(tabs, wxNotebook)->Update();
            }
        }
    }
    {
        wxWindow *targetWindow = wxWindow::FindWindowById(wxID_MEM);
        if (targetWindow != NULL) {
            wxFrame *frame = wxStaticCast(targetWindow, wxFrame);
            wxWindow *panel = wxWindow::FindWindowById(wxID_MemPane, frame);
            if (panel != nullptr) {
                DmEE *view = wxStaticCast(panel, DmEE);
                wxWindow *child = wxWindow::FindWindowById(wxID_Hexe, view);
                if (child != nullptr) {
                    wxStaticCast(child, Hexe)->Refresh();
                }
            }
        }
    }
}

DECLARE_APP(Pcsx2App)

void W2::UpdateScreens()
{
    wxGetApp().ProcessMethod(_UpdateScreens);
}

class SysExecEvent_UpdateWBrkpt : public SysExecEvent
{
public:
    u32 pc;

    SysExecEvent_UpdateWBrkpt(u32 pc)
        : pc(pc)
    {
    }

    virtual ~SysExecEvent_UpdateWBrkpt() throw() {}

    wxString GetEventName() const { return L"UpdateWBrkpt"; }

protected:
    void InvokeEvent()
    {
        u32 pc = this->pc & (~3UL);

        resetCodes(pc, 4);
    }
};

void W2::CWBrk::Set(uint pc)
{
    this->pc = pc;

    if (CoreThread.IsOpen()) {
        GetSysExecutorThread().PostEvent(new SysExecEvent_UpdateWBrkpt(this->pc));
        s_wbrkRes |= 1;
    }
}

void W2::CWBrk::Unset()
{
    if (CoreThread.IsOpen()) {
        GetSysExecutorThread().PostEvent(new SysExecEvent_UpdateWBrkpt(this->pc));
        s_wbrkRes |= 1;
    }

    this->pc = 0;
}

void _WBrkptHit()
{
    _UpdateScreens();
    {
        FwEE *frame = wxStaticCast(wxWindow::FindWindowById(wxID_R5900DBG), FwEE);
        if (frame != NULL) {
            DaEEC *vw = wxStaticCast(wxWindow::FindWindowById(wxID_EEC), DaEEC);
            if (vw != NULL)
                vw->Goto(cpuRegs.pc);
        }
    }
}

int W2::WBrkptHit()
{
    static u32 curcnt = 1, prevcnt = 0;

    if (curcnt != prevcnt) {
        s_wbrkRes = 0;
        wxGetApp().ProcessMethod(_WBrkptHit);
        prevcnt = curcnt;
    }

    //	if (s_evExecContinue.WaitTimeout(100) == wxCOND_TIMEOUT) {
    //		return s_wbrkRes | 2;
    //	}

    //	s_evExecContinue.Wait();

    while (true) {
        if (!GetSysExecutorThread().IsRunning())
            return 2;
        if (s_evExecContinue.WaitTimeout(100) == wxCOND_TIMEOUT)
            continue;

        break;
    }

    curcnt++;
    return s_wbrkRes;
}
