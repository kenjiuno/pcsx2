#include "PrecompiledHeader.h"
#include "EEDisarm.h"

#define GPR32 eedNameGPR

namespace Ma
{
const int M0000 = 0;
const int M0001 = 1;
const int M0010 = 2;
const int M0011 = 3;
const int M0100 = 4;
const int M0101 = 5;
const int M0110 = 6;
const int M0111 = 7;
const int M1000 = 8;
const int M1001 = 9;
const int M1010 = 10;
const int M1011 = 11;
const int M1100 = 12;
const int M1101 = 13;
const int M1110 = 14;
const int M1111 = 15;

const int M00000 = 0;
const int M00001 = 1;
const int M00010 = 2;
const int M00011 = 3;
const int M00100 = 4;
const int M00101 = 5;
const int M00110 = 6;
const int M00111 = 7;
const int M01000 = 8;
const int M01001 = 9;
const int M01010 = 10;
const int M01011 = 11;
const int M01100 = 12;
const int M01101 = 13;
const int M01110 = 14;
const int M01111 = 15;
const int M10000 = 16;
const int M10001 = 17;
const int M10010 = 18;
const int M10011 = 19;
const int M10100 = 20;
const int M10101 = 21;
const int M10110 = 22;
const int M10111 = 23;
const int M11000 = 24;
const int M11001 = 25;
const int M11010 = 26;
const int M11011 = 27;
const int M11100 = 28;
const int M11101 = 29;
const int M11110 = 30;
const int M11111 = 31;

const int M000000 = 0;
const int M000001 = 1;
const int M000010 = 2;
const int M000011 = 3;
const int M000100 = 4;
const int M000101 = 5;
const int M000110 = 6;
const int M000111 = 7;
const int M001000 = 8;
const int M001001 = 9;
const int M001010 = 10;
const int M001011 = 11;
const int M001100 = 12;
const int M001101 = 13;
const int M001110 = 14;
const int M001111 = 15;
const int M010000 = 16;
const int M010001 = 17;
const int M010010 = 18;
const int M010011 = 19;
const int M010100 = 20;
const int M010101 = 21;
const int M010110 = 22;
const int M010111 = 23;
const int M011000 = 24;
const int M011001 = 25;
const int M011010 = 26;
const int M011011 = 27;
const int M011100 = 28;
const int M011101 = 29;
const int M011110 = 30;
const int M011111 = 31;
const int M100000 = 32;
const int M100001 = 33;
const int M100010 = 34;
const int M100011 = 35;
const int M100100 = 36;
const int M100101 = 37;
const int M100110 = 38;
const int M100111 = 39;
const int M101000 = 40;
const int M101001 = 41;
const int M101010 = 42;
const int M101011 = 43;
const int M101100 = 44;
const int M101101 = 45;
const int M101110 = 46;
const int M101111 = 47;
const int M110000 = 48;
const int M110001 = 49;
const int M110010 = 50;
const int M110011 = 51;
const int M110100 = 52;
const int M110101 = 53;
const int M110110 = 54;
const int M110111 = 55;
const int M111000 = 56;
const int M111001 = 57;
const int M111010 = 58;
const int M111011 = 59;
const int M111100 = 60;
const int M111101 = 61;
const int M111110 = 62;
const int M111111 = 63;
} // namespace Ma

#define Oc26 _Oc26()
#define Oc25 _Oc25()
#define Oc21 _Oc21()
#define Oc16 _Oc16()
#define Oc6 _Oc6()
#define Oc0 _Oc0()
#define Oc2c4 _Oc2c4()
#define Oc6c5 _Oc6c5()
#define Ors _Ors()
#define Ort _Ort()
#define Ord _Ord()
#define Osa _Osa()
#define Ostype _Ostype()
#define Ocode6 _Ocode6()
#define Oft _Oft()
#define Ofs _Ofs()
#define Ofd _Ofd()
#define Oi _Oi()
#define Oimm16u _Oimm16u()
#define Oimm16s _Oimm16s()
#define Ooff16 _Ooff16()
#define Ooff26 _Ooff26()
#define Oid2 _Oid2()
#define Ofd2 _Ofd2()
#define Oft2 _Oft2()
#define Ovft _Ovft()
#define Ovfs _Ovfs()
#define Ovfd _Ovfd()
#define Ovbc _Ovbc()
#define Ovimm15 _Ovimm15()
#define Ovimm5 _Ovimm5()
#define Ovidr _Ovidr()
#define Ovisr _Ovisr()
#define Ovitr _Ovitr()
#define Oc32 _Oc32()
#define Trs _Trs()
#define Trt _Trt()
#define Trd _Trd()
#define Tsa _Tsa()
#define Tsa32 _Tsa32()
#define Thint _Thint()
#define Timm16s _Timm16s()
#define Timm16u _Timm16u()
#define Tft _Tft()
#define Tfs _Tfs()
#define Tfd _Tfd()
#define Tcode6 _Tcode6()
#define Toffbrs _Toffbrs()
#define Tid2 _Tid2()
#define Tfd2 _Tfd2()
#define Tft2 _Tft2()
#define Vd _Vd()
#define Vft _Vft()
#define Vftd _Vftd()
#define Vftxyz _Vftxyz()
#define Vftw _Vftw()
#define Vfs _Vfs()
#define Vfsd _Vfsd()
#define Vfd _Vfd()
#define Vfdd _Vfdd()
#define Vfdxyz _Vfdxyz()
#define Vbc _Vbc()
#define Vacc _Vacc()
#define Vaccxyz _Vaccxyz()
#define Vimm15 _Vimm15()
#define Vimm5 _Vimm5()
#define Vfsxyz _Vfsxyz()
#define Vfsf _Vfsf()
#define Vfsfsf _Vfsfsf()
#define Vftf _Vftf()
#define Vftftf _Vftftf()
#define Vftbc _Vftbc()
#define Visr _Visr()
#define Visd _Visd()
#define Vmmisd _Vmmisd()
#define Visppd _Visppd()
#define Vitr _Vitr()
#define Vmmitd _Vmmitd()
#define Vitppd _Vitppd()
#define Vidr _Vidr()

using namespace EEDisarm;

struct MIPSi
{
    uint w;

    MIPSi(uint w)
        : w(w)
    {
    }

    // EE
    wxString Trs { return GPR32[Ors]; }
    wxString Trt { return GPR32[Ort]; }
    wxString Trd { return GPR32[Ord]; }
    wxString Tsa { return wxsFormat(L"%d", Osa); }
    wxString Tsa32 { return wxsFormat(L"%d", Osa); }
    wxString Thint { return wxsFormat(L"%d", Ort); }
    wxString Timm16s { return wxsFormat(L"$%04x", ((s16)Oimm16s)); }
    wxString Timm16u { return wxsFormat(L"$%04x", ((u16)Oimm16u)); }
    wxString Tft { return wxsFormat(L"$f%d", Oft); }
    wxString Tfs { return wxsFormat(L"$f%d", Ofs); }
    wxString Tfd { return wxsFormat(L"$f%d", Ofd); }

    int Oc26 { return (int)((w >> 26) & 0x3F); }
    int Oc25 { return (int)((w >> 25) & 0x01); }
    int Oc21 { return (int)((w >> 21) & 0x1F); }
    int Oc16 { return (int)((w >> 16) & 0x1F); }
    int Oc6 { return (int)((w >> 6) & 0x1F); }
    int Oc0 { return (int)((w)&0x3F); }
    int Oc2c4 { return (int)((w >> 2) & 0x0F); }
    int Oc6c5 { return (int)((w >> 6) & 0x1F); }
    int Ors { return (int)((w >> 21) & 0x1F); }
    int Ort { return (int)((w >> 16) & 0x1F); }
    int Ord { return (int)((w >> 11) & 0x1F); }
    int Osa { return (int)((w >> 6) & 0x1F); }
    int Ostype { return (int)((w >> 6) & 0x1F); }
    int Ocode6 { return (int)((w >> 6) & 0xFFFFF); }
    uint Oc32 { return w; }
    int Oft { return (int)((w >> 16) & 0x1F); }
    int Ofs { return (int)((w >> 11) & 0x1F); }
    int Ofd { return (int)((w >> 6) & 0x1F); }
    int Oi { return (int)(w & 1); }

    int Oimm16u
    {
        return (u16)((w >> 0) & 0xFFFF);
    }
    int Oimm16s
    {
        return (s16)((w >> 0) & 0xFFFF);
    }
    int Ooff16
    {
        return (s16)(w & 0xFFFF);
    }
    int Ooff26
    {
        return (s32)(w & 0x3FFFFFF);
    }

    wxString Toff16(uint pc)
    {
        return wxsFormat(L"$%08x", (pc + 4 + Ooff16 * 4));
    }
    wxString Toff26(uint pc)
    {
        return wxsFormat(L"$%08x", ((pc & 0xF0000000) | (uint)Ooff26 * 4));
    }
    wxString Tcode6 { return wxsFormat(L"$%05x", Ocode6); }
    wxString Toffbrs { return wxsFormat(L"$%04x(%s)", ((s16)Oimm16s), Trs.c_str()); }

    int Oid2 { return (int)((w >> 11) & 0x1F); }
    wxString Tid2 { return wxsFormat(L"$vi%d", Oid2); }
    int Ofd2 { return (int)((w >> 11) & 0x1F); }
    wxString Tfd2 { return wxsFormat(L"vf%d", Ofd2); }
    int Oft2 { return (int)((w >> 16) & 0x1F); }
    wxString Tft2 { return wxsFormat(L"vf%d", Oft2); }

    //int Ort2 { get { return (int)((w >> 16) & 0x1F); } }
    //wxString Trt2 { get { return "vf" + Ort2; } }

    // COP2(VU0)

    /// <summary>
    /// dest M21,4
    /// </summary>
    wxString Vd
    {
        wxString s;
        if ((0 != (w & 0x01000000)))
            s += L"x";
        if ((0 != (w & 0x00800000)))
            s += L"y";
        if ((0 != (w & 0x00400000)))
            s += L"z";
        if ((0 != (w & 0x00200000)))
            s += L"w";
        return s;
    }

    int Ovft { return (int)((w >> 16) & 0x1F); }
    /// <summary>
    /// ftdest M16,5
    /// </summary>
    wxString Vft { return wxsFormat(L"vf%d", Ovft); }
    wxString Vftd { return Vft; }
    wxString Vftxyz { return Vft + L""; }
    wxString Vftw { return Vft + L"w"; }

    int Ovfs { return (int)((w >> 11) & 0x1F); }
    /// <summary>
    /// fsdest M11,5
    /// </summary>
    wxString Vfs { return wxsFormat(L"vf%d", Ovfs); }
    wxString Vfsd { return Vfs; }

    int Ovfd { return (int)((w >> 6) & 0x1F); }
    /// <summary>
    /// fddest M6,5
    /// </summary>
    wxString Vfd { return wxsFormat(L"vf%d", Ovfd); }
    wxString Vfdd { return Vfd; }
    wxString Vfdxyz { return Vfd; }

    int Ovbc { return (int)((w)&3); }
    /// <summary>
    /// bc M0,2
    /// </summary>
    wxString Vbc { return wxsFormat(L"%c", L"xyzw"[Ovbc]); }

    wxString Vacc { return L"ACC"; }
    wxString Vaccxyz { return L"ACC"; }

    int Ovimm15 { return (int)((w >> 6) & 0x7FFF); }
    /// <summary>
    /// Imm15 M6,15
    /// </summary>
    wxString Vimm15 { return wxsFormat(L"$%04x", Ovimm15); }

    int Ovimm5 { return (int)((w >> 6) & 0x1F); }
    /// <summary>
    /// Imm5 M6,5
    /// </summary>
    wxString Vimm5 { return wxsFormat(L"$%04x", Ovimm5); }

    wxString Vfsxyz { return wxsFormat(L"vf%d", Ovfs); }
    wxString Vfsf { return wxsFormat(L"%c", "xyzw"[(int)((w >> 21) & 3)]); }
    wxString Vfsfsf { return wxsFormat(L"vf%d%s", Ovfs, Vfsf.c_str()); }
    wxString Vftf { return wxsFormat(L"%c", "xyzw"[(int)((w >> 23) & 3)]); }
    wxString Vftftf { return wxsFormat(L"vf%d%s", Ovft, Vftf.c_str()); }
    wxString Vftbc { return wxsFormat(L"vf%d%s", Ovft, Vbc.c_str()); }

    int Ovidr { return (int)((w >> 6) & 0x1F); }
    /// <summary>
    /// idreg M6,5
    /// </summary>
    wxString Vidr { return wxsFormat(L"$vi%d", Ovidr); }

    int Ovisr { return (int)((w >> 11) & 0x1F); }
    /// <summary>
    /// isreg M11,5
    /// </summary>
    wxString Visr { return wxsFormat(L"$vi%d", Ovisr); }
    wxString Visd { return wxsFormat(L"$vi%d%s", Ovisr, Vd.c_str()); }
    wxString Vmmisd { return wxsFormat(L"(--$vi%d)%s", Ovisr, Vd.c_str()); }
    wxString Visppd { return wxsFormat(L"($vi%d++)%s", Ovisr, Vd.c_str()); }

    int Ovitr { return (int)((w >> 16) & 0x1F); }
    /// <summary>
    /// itreg M16,5
    /// </summary>
    wxString Vitr { return wxsFormat(L"$vi%d", Ovitr); }
    wxString Vmmitd { return wxsFormat(L"(--$vi%d)%s", Ovitr, Vd.c_str()); }
    wxString Vitppd { return wxsFormat(L"($vi%d++)%s", Ovitr, Vd.c_str()); }
};

namespace EEDisarm
{

EEis &Parse(uint word, uint pc, EEis &e)
{
    MIPSi a(word);
    // (CPU Instruction Set)
    if (false) {
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100000) { //ADD
        return e = EEis(L"ADD", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M001000) { //ADDI
        return e = EEis(L"ADDI", a.Trt, a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M001001) { //ADDIU
        return e = EEis(L"ADDIU", a.Trt, a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100001) { //ADDU
        return e = EEis(L"ADDU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100100) { //AND
        return e = EEis(L"AND", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M001100) { //ANDI
        return e = EEis(L"ANDI", a.Trt, a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000100) { //BEQ
        return e = EEis(L"BEQ", a.Trs, a.Trt, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010100) { //BEQL
        return e = EEis(L"BEQL", a.Trs, a.Trt, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M00001) { //BGEZ
        return e = EEis(L"BGEZ", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M10001) { //BGEZAL
        return e = EEis(L"BGEZAL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M10011) { //BGEZALL
        return e = EEis(L"BGEZALL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M00011) { //BGEZL
        return e = EEis(L"BGEZL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000111) { //BGTZ
        return e = EEis(L"BGTZ", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010111) { //BGTZL
        return e = EEis(L"BGTZL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000110) { //BLEZ
        return e = EEis(L"BLEZ", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010110) { //BLEZL
        return e = EEis(L"BLEZL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M00000) { //BLTZ
        return e = EEis(L"BLTZ", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M10000) { //BLTZAL
        return e = EEis(L"BLTZAL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M10010) { //BLTZALL
        return e = EEis(L"BLTZALL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M00010) { //BLTZL
        return e = EEis(L"BLTZL", a.Trs, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000101) { //BNE
        return e = EEis(L"BNE", a.Trs, a.Trt, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010101) { //BNEL
        return e = EEis(L"BNEL", a.Trs, a.Trt, a.Toff16(pc));
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001101) { //BREAK
        return e = EEis(L"BREAK");
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101100) { //DADD
        return e = EEis(L"DADD", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011000) { //DADDI
        return e = EEis(L"DADDI", a.Trt, a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M011001) { //DADDIU
        return e = EEis(L"DADDIU", a.Trt, a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101101) { //DADDU
        return e = EEis(L"DADDU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011010) { //DIV
        return e = EEis(L"DIV", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011011) { //DIVU
        return e = EEis(L"DIVU", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111000) { //DSLL
        return e = EEis(L"DSLL", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111100) { //DSLL32
        return e = EEis(L"DSLL32", a.Trd, a.Trt, a.Tsa32);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010100) { //DSLLV
        return e = EEis(L"DSLLV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111011) { //DSRA
        return e = EEis(L"DSRA", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111111) { //DSRA32
        return e = EEis(L"DSRA32", a.Trd, a.Trt, a.Tsa32);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010111) { //DSRAV
        return e = EEis(L"DSRAV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111010) { //DSRL
        return e = EEis(L"DSRL", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M111110) { //DSRL32
        return e = EEis(L"DSRL32", a.Trd, a.Trt, a.Tsa32);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010111) { //DSRLV
        return e = EEis(L"DSRLV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101110) { //DSUB
        return e = EEis(L"DSUB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101111) { //DSUBU
        return e = EEis(L"DSUBU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000010) { //J
        return e = EEis(L"J", a.Toff26(pc));
    } else if (a.Oc26 == Ma::M000011) { //JAL
        return e = EEis(L"JAL", a.Toff26(pc));
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001001) { //JALR
        return e = EEis(L"JALR", a.Trd, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001000) { //JR
        return e = EEis(L"JR", a.Trs);
    } else if (a.Oc26 == Ma::M100000) { //LB
        return e = EEis(L"LB", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100100) { //LBU
        return e = EEis(L"LBU", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M110111) { //LD
        return e = EEis(L"LD", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M011010) { //LDL
        return e = EEis(L"LDL", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M011011) { //LDR
        return e = EEis(L"LDR", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100001) { //LH
        return e = EEis(L"LH", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100101) { //LHU
        return e = EEis(L"LHU", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M001111) { //LUI
        return e = EEis(L"LUI", a.Trt, a.Timm16s);
    } else if (a.Oc26 == Ma::M100011) { //LW
        return e = EEis(L"LW", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100010) { //LWL
        return e = EEis(L"LWL", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100110) { //LWR
        return e = EEis(L"LWR", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M100111) { //LWU
        return e = EEis(L"LWU", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010000) { //MFHI
        return e = EEis(L"MFHI", a.Trd);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010010) { //MFLO
        return e = EEis(L"MFLO", a.Trd);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001011) { //MOVN
        return e = EEis(L"MOVN", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001010) { //MOVZ
        return e = EEis(L"MOVZ", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010001) { //MTHI
        return e = EEis(L"MTHI", a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M010011) { //MTLO
        return e = EEis(L"MTLO", a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011000) { //MULT
        return e = EEis(L"MULT", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011001) { //MULTU
        return e = EEis(L"MULTU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100111) { //NOR
        return e = EEis(L"NOR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100101) { //OR
        return e = EEis(L"OR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M001101) { //ORI
        return e = EEis(L"ORI", a.Trt, a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M110011) { //PREF
        return e = EEis(L"PREF", a.Thint, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101000) { //SB
        return e = EEis(L"SB", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M111111) { //SD
        return e = EEis(L"SD", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101100) { //SDL
        return e = EEis(L"SDL", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101101) { //SDR
        return e = EEis(L"SDR", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101001) { //SH
        return e = EEis(L"SH", a.Trt, a.Toffbrs);
    } else if (a.Oc32 == 0) { //NOP
        return e = EEis(L"NOP");
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000000) { //SLL
        return e = EEis(L"SLL", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000100) { //SLLV
        return e = EEis(L"SLLV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101010) { //SLT
        return e = EEis(L"SLT", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M001010) { //SLTI
        return e = EEis(L"SLTI", a.Trt, a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M001011) { //SLTIU
        return e = EEis(L"SLTIU", a.Trt, a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101011) { //SLTU
        return e = EEis(L"SLTU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000011) { //SRA
        return e = EEis(L"SRA", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000111) { //SRAV
        return e = EEis(L"SRAV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000010) { //SRL
        return e = EEis(L"SRL", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M000110) { //SRLV
        return e = EEis(L"SRLV", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100010) { //SUB
        return e = EEis(L"SUB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100011) { //SUBU
        return e = EEis(L"SUBU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M101011) { //SW
        return e = EEis(L"SW", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101010) { //SWL
        return e = EEis(L"SWL", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M101110) { //SWR
        return e = EEis(L"SWR", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001111 && (a.Ostype & 0x10) == 0x00) { //SYNC.L
        return e = EEis(L"SYNC.L");
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001111 && (a.Ostype & 0x10) == 0x10) { //SYNC.P
        return e = EEis(L"SYNC.P");
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M001100) { //SYSCALL
        return e = EEis(L"SYSCALL", a.Tcode6);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110110) { //TEQ
        return e = EEis(L"TEQ", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01100) { //TEQI
        return e = EEis(L"TEQI", a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110000) { //TGE
        return e = EEis(L"TGE", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01000) { //TGEI
        return e = EEis(L"TGEI", a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01000) { //TGEIU
        return e = EEis(L"TGEIU", a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110001) { //TGEU
        return e = EEis(L"TGEU", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110010) { //TLT
        return e = EEis(L"TLT", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01010) { //TLTI
        return e = EEis(L"TLTI", a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01011) { //TLTIU
        return e = EEis(L"TLTIU", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110011) { //TLTU
        return e = EEis(L"TLTU", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M110110) { //TNE
        return e = EEis(L"TNE", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M01110) { //TNEI
        return e = EEis(L"TNEI", a.Trs, a.Timm16s);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M100110) { //XOR
        return e = EEis(L"XOR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M001110) { //XORI
        return e = EEis(L"XORI", a.Trt, a.Trs, a.Timm16u);
    }

    if (false) {
    }
    // (EE-core spec. inst.)
    else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M011010) { //DIV1
        return e = EEis(L"DIV1", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M011011) { //DIVU1
        return e = EEis(L"DIVU1", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011110) { //LQ
        return e = EEis(L"LQ", a.Trt, a.Toffbrs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M000000) { //MADD
        return e = EEis(L"MADD", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M100000) { //MADD1
        return e = EEis(L"MADD1", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M000001) { //MADDU
        return e = EEis(L"MADDU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M100001) { //MADDU1
        return e = EEis(L"MADDU1", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M010000) { //MFHI1
        return e = EEis(L"MFHI1", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M010010) { //MFLO1
        return e = EEis(L"MFLO1", a.Trd);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101000) { //MFSA
        return e = EEis(L"MFSA", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M010001) { //MTHI1
        return e = EEis(L"MTHI1", a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M010011) { //MTLO1
        // http://www.kernel-api.org/docs/online/2.6.17/d2/d5d/mipsregs_8h-source.html
        return e = EEis(L"MTLO1", a.Trs);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M101001) { //MTSA
        return e = EEis(L"MTSA", a.Trs);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M11000) { //MTSAB
        return e = EEis(L"MTSAB", a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000001 && a.Oc16 == Ma::M11001) { //MTSAH
        return e = EEis(L"MTSAH", a.Trs, a.Timm16u);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011000) { //MULT
        return e = EEis(L"MULT", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M011000) { //MULT1
        return e = EEis(L"MULT1", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M000000 && a.Oc0 == Ma::M011001) { //MULTU
        return e = EEis(L"MULTU", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M011001) { //MULTU1
        return e = EEis(L"MULTU1", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M101000) { //PABSH
        return e = EEis(L"PABSH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00001 && a.Oc0 == Ma::M101000) { //PABSW
        return e = EEis(L"PABSW", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M001000) { //PADDB
        return e = EEis(L"PADDB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M001000) { //PADDH
        return e = EEis(L"PADDH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11000 && a.Oc0 == Ma::M001000) { //PADDSB
        return e = EEis(L"PADDSB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10100 && a.Oc0 == Ma::M001000) { //PADDSH
        return e = EEis(L"PADDSH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10000 && a.Oc0 == Ma::M001000) { //PADDSW
        return e = EEis(L"PADDSW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11000 && a.Oc0 == Ma::M101000) { //PADDUB
        return e = EEis(L"PADDUB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10100 && a.Oc0 == Ma::M101000) { //PADDUH
        return e = EEis(L"PADDUH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10000 && a.Oc0 == Ma::M101000) { //PADDUW
        return e = EEis(L"PADDUW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00000 && a.Oc0 == Ma::M001000) { //PADDW
        return e = EEis(L"PADDW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M101000) { //PADSBH
        return e = EEis(L"PADSBH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10010 && a.Oc0 == Ma::M001001) { //PAND
        return e = EEis(L"PAND", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M101000) { //PCEQB
        return e = EEis(L"PCEQB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00110 && a.Oc0 == Ma::M101000) { //PCEQH
        return e = EEis(L"PCEQH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00010 && a.Oc0 == Ma::M101000) { //PCEQW
        return e = EEis(L"PCEQW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M001000) { //PCGTB
        return e = EEis(L"PCGTB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00110 && a.Oc0 == Ma::M001000) { //PCGTH
        return e = EEis(L"PCGTH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00010 && a.Oc0 == Ma::M001000) { //PCGTW
        return e = EEis(L"PCGTW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11011 && a.Oc0 == Ma::M101001) { //PCPYH
        return e = EEis(L"PCPYH", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01110 && a.Oc0 == Ma::M001001) { //PCPYLD
        return e = EEis(L"PCPYLD", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01110 && a.Oc0 == Ma::M101001) { //PCPYUD
        return e = EEis(L"PCPYUD", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11101 && a.Oc0 == Ma::M001001) { //PDIVBW
        return e = EEis(L"PDIVBW", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01101 && a.Oc0 == Ma::M101001) { //PDIVUW
        return e = EEis(L"PDIVUW", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01101 && a.Oc0 == Ma::M001001) { //PDIVW
        return e = EEis(L"PDIVW", a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11010 && a.Oc0 == Ma::M101001) { //PEXCH
        return e = EEis(L"PEXCH", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11110 && a.Oc0 == Ma::M101001) { //PEXCW
        return e = EEis(L"PEXCW", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11010 && a.Oc0 == Ma::M001001) { //PEXEH
        return e = EEis(L"PEXEH", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11110 && a.Oc0 == Ma::M001001) { //PEXEW
        return e = EEis(L"PEXEW", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11110 && a.Oc0 == Ma::M001000) { //PEXT5
        return e = EEis(L"PEXT5", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11010 && a.Oc0 == Ma::M001000) { //PEXTLB
        return e = EEis(L"PEXTLB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10110 && a.Oc0 == Ma::M001000) { //PEXTLH
        return e = EEis(L"PEXTLH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10010 && a.Oc0 == Ma::M001000) { //PEXTLW
        return e = EEis(L"PEXTLW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11010 && a.Oc0 == Ma::M101000) { //PEXTUB
        return e = EEis(L"PEXTUB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10110 && a.Oc0 == Ma::M101000) { //PEXTUH
        return e = EEis(L"PEXTUH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10010 && a.Oc0 == Ma::M101000) { //PEXTUW
        return e = EEis(L"PEXTUW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10001 && a.Oc0 == Ma::M001001) { //PHMADH
        return e = EEis(L"PHMADH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10101 && a.Oc0 == Ma::M001001) { //PHMSBG
        return e = EEis(L"PHMSBG", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M101001) { //PINTEH
        return e = EEis(L"PINTEH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M000100) { //PLZCW
        return e = EEis(L"PLZCW", a.Trd, a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10000 && a.Oc0 == Ma::M001001) { //PMADDH
        return e = EEis(L"PMADDH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00000 && a.Oc0 == Ma::M101001) { //PMADDUW
        return e = EEis(L"PMADDUW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00000 && a.Oc0 == Ma::M001001) { //PMADDW
        return e = EEis(L"PMADDW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M001000) { //PMAXH
        return e = EEis(L"PMAXH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00011 && a.Oc0 == Ma::M001000) { //PMAXW
        return e = EEis(L"PMAXW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M001001) { //PMFHI
        return e = EEis(L"PMFHI", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00011 && a.Oc0 == Ma::M110000) { //PMFHL.LH
        return e = EEis(L"PMFHL.LH", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00000 && a.Oc0 == Ma::M110000) { //PMFHL.LW
        return e = EEis(L"PMFHL.LW", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M110000) { //PMFHL.SH
        return e = EEis(L"PMFHL.SH", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00010 && a.Oc0 == Ma::M110000) { //PMFHL.SLW
        return e = EEis(L"PMFHL.SLW", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00001 && a.Oc0 == Ma::M110000) { //PMFHL.UW
        return e = EEis(L"PMFHL.UW", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01001 && a.Oc0 == Ma::M001001) { //PMFLO
        return e = EEis(L"PMFLO", a.Trd);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M101000) { //PMINH
        return e = EEis(L"PMINH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00011 && a.Oc0 == Ma::M101000) { //PMINW
        return e = EEis(L"PMINW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10100 && a.Oc0 == Ma::M001001) { //PMSUBH
        return e = EEis(L"PMSUBH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M001001) { //PMSUBW
        return e = EEis(L"PMSUBW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M101001) { //PMTHI
        return e = EEis(L"PMTHI", a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00000 && a.Oc0 == Ma::M110001) { //PMTHL.LW
        return e = EEis(L"PMTHL.LW", a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01001 && a.Oc0 == Ma::M101001) { //PMTLO
        return e = EEis(L"PMTLO", a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11100 && a.Oc0 == Ma::M001001) { //PMULTH
        return e = EEis(L"PMULTH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01100 && a.Oc0 == Ma::M101001) { //PMULTUW
        return e = EEis(L"PMULTUW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01100 && a.Oc0 == Ma::M001001) { //PMULTW
        return e = EEis(L"PMULTW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10011 && a.Oc0 == Ma::M101001) { //PNOR
        return e = EEis(L"PNOR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10010 && a.Oc0 == Ma::M101001) { //POR
        return e = EEis(L"POR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11111 && a.Oc0 == Ma::M001000) { //PPAC5
        return e = EEis(L"PPAC5", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11011 && a.Oc0 == Ma::M001000) { //PPACB
        return e = EEis(L"PPACB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10111 && a.Oc0 == Ma::M001000) { //PPACH
        return e = EEis(L"PPACH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10011 && a.Oc0 == Ma::M001000) { //PPACW
        return e = EEis(L"PPACW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11011 && a.Oc0 == Ma::M001001) { //PREVH
        return e = EEis(L"PREVH", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11111 && a.Oc0 == Ma::M001001) { //PROT3W
        return e = EEis(L"PROT3W", a.Trd, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M110100) { //PSLLH
        return e = EEis(L"PSLLH", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00010 && a.Oc0 == Ma::M001001) { //PSLLVW
        return e = EEis(L"PSLLVW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M111100) { //PSLLW
        return e = EEis(L"PSLLW", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M110111) { //PSRAH
        return e = EEis(L"PSRAH", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00011 && a.Oc0 == Ma::M101001) { //PSRAVW
        return e = EEis(L"PSRAVW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M111111) { //PSRAW
        return e = EEis(L"PSRAW", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M110110) { //PSRLH
        return e = EEis(L"PSRLH", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00011 && a.Oc0 == Ma::M001001) { //PSRLVW
        return e = EEis(L"PSRLVW", a.Trd, a.Trt, a.Trs);
    } else if (a.Oc26 == Ma::M011100 && a.Oc0 == Ma::M111110) { //PSRLW
        return e = EEis(L"PSRLW", a.Trd, a.Trt, a.Tsa);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M01001 && a.Oc0 == Ma::M001000) { //PSUBB
        return e = EEis(L"PSUBB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M001000) { //PSUBH
        return e = EEis(L"PSUBH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11001 && a.Oc0 == Ma::M001000) { //PSUBSB
        return e = EEis(L"PSUBSB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10101 && a.Oc0 == Ma::M001000) { //PSUBSH
        return e = EEis(L"PSUBSH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10001 && a.Oc0 == Ma::M001000) { //PSUBSW
        return e = EEis(L"PSUBSW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11001 && a.Oc0 == Ma::M101000) { //PSUBUB
        return e = EEis(L"PSUBUB", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10101 && a.Oc0 == Ma::M101000) { //PSUBUH
        return e = EEis(L"PSUBUH", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10001 && a.Oc0 == Ma::M101000) { //PSUBUW
        return e = EEis(L"PSUBUW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M00001 && a.Oc0 == Ma::M001000) { //PSUBW
        return e = EEis(L"PSUBW", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M10011 && a.Oc0 == Ma::M001001) { //PXOR
        return e = EEis(L"PXOR", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011100 && a.Oc6 == Ma::M11011 && a.Oc0 == Ma::M101000) { //QFSRV
        return e = EEis(L"QFSRV", a.Trd, a.Trs, a.Trt);
    } else if (a.Oc26 == Ma::M011111) { //SQ
        return e = EEis(L"SQ", a.Trt, a.Toffbrs);
    }

    if (false) {
    }
    // (COP0)
    else if (a.Oc26 == Ma::M010000 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00000) { //BC0F
        return e = EEis(L"BC0F", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010000 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00010) { //BC0FL
        return e = EEis(L"BC0FL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010000 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00001) { //BC0T
        return e = EEis(L"BC0T", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010000 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00011) { //BC0TL
        return e = EEis(L"BC0TL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010000 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011000) { //ERET
        return e = EEis(L"ERET");
    }
    // (COP1)
    else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000101) { //ABS.S
        return e = EEis(L"ABS.S", a.Tfd, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000000) { //ADD.S
        return e = EEis(L"ADD.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011000) { //ADDA.S
        return e = EEis(L"ADDA.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00000) { //BC1F
        return e = EEis(L"BC1F", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00010) { //BC1FL
        return e = EEis(L"BC1FL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00001) { //BC1T
        return e = EEis(L"BC1T", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00011) { //BC1TL
        return e = EEis(L"BC1TL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M110010) { //C.EQ.S
        return e = EEis(L"C.EQ.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M110000) { //C.F.S
        return e = EEis(L"C.F.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M110110) { //C.LE.S
        return e = EEis(L"C.LE.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M110100) { //C.LT.S
        return e = EEis(L"C.LT.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M00010) { //CFC1
        return e = EEis(L"CFC1", a.Trt, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M00110) { //CTC1
        return e = EEis(L"CFC1", a.Trt, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10100 && a.Oc0 == Ma::M100000) { //CVT.S.W
        return e = EEis(L"CVT.S.W", a.Tfd, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M100100) { //CVT.W.S
        return e = EEis(L"CVT.W.S", a.Tfd, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000011) { //DIV.S
        return e = EEis(L"DIV.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M110001) { //LWC1
        return e = EEis(L"LWC1", a.Tft, a.Toffbrs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011100) { //MADD.S
        return e = EEis(L"MADD.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011110) { //MADDA.S
        return e = EEis(L"MADDA.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M101000) { //MAX.S
        return e = EEis(L"MAX.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M00000) { //MFC1
        return e = EEis(L"MFC1", a.Trt, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M110000) { //MIN.S
        return e = EEis(L"MIN.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000110) { //MOV.S
        return e = EEis(L"MOV.S", a.Tfd, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011101) { //MSUB.S
        return e = EEis(L"MSUB.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011111) { //MSUBA.S
        return e = EEis(L"MSUBA.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M00100) { //MTC1
        return e = EEis(L"MTC1", a.Trt, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000010) { //MUL.S
        return e = EEis(L"MUL.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011010) { //MULA.S
        return e = EEis(L"MULA.S", a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000111) { //NEG.S
        return e = EEis(L"NEG.S", a.Tfd, a.Tfs);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M010110) { //RSQRT.S
        return e = EEis(L"RSQRT.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000100) { //SQRT.S
        return e = EEis(L"SQRT.S", a.Tfd, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M000001) { //SUB.S
        return e = EEis(L"SUB.S", a.Tfd, a.Tfs, a.Tft);
    } else if (a.Oc26 == Ma::M010001 && a.Oc21 == Ma::M10000 && a.Oc0 == Ma::M011001) { //SUBA.S
        return e = EEis(L"SUBA.S", a.Tfs, a.Tft, L"I");
    } else if (a.Oc26 == Ma::M111001) { //SWC1
        return e = EEis(L"SWC1", a.Tft, a.Toffbrs);
    }
    // (COP2)
    else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00000) { //BC2F
        return e = EEis(L"BC2F", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00010) { //BC2FL
        return e = EEis(L"BC2FL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00001) { //BC2T
        return e = EEis(L"BC2T", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M01000 && a.Oc16 == Ma::M00011) { //BC2TL
        return e = EEis(L"BC2TL", a.Toff16(pc));
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00010 && a.Oi == 1) { //CFC2.I
        return e = EEis(L"CFC2.I", a.Trt, a.Tid2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00010 && a.Oi == 0) { //CFC2
        return e = EEis(L"CFC2", a.Trt, a.Tid2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00110 && a.Oi == 1) { //CTC2.I
        return e = EEis(L"CTC2.I", a.Trt, a.Tid2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00110 && a.Oi == 0) { //CTC2
        return e = EEis(L"CTC2", a.Trt, a.Tid2);
    } else if (a.Oc26 == Ma::M110110) { //LQC2
        return e = EEis(L"LQC2", a.Tft2, a.Toffbrs);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00001 && a.Oi == 1) { //QMFC2.I
        return e = EEis(L"QMFC2.I", a.Trt, a.Tfd2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00001 && a.Oi == 0) { //QMFC2
        return e = EEis(L"QMFC2", a.Trt, a.Tfd2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00101 && a.Oi == 1) { //QMTC2.I
        return e = EEis(L"QMTC2.I", a.Trt, a.Tfd2);
    } else if (a.Oc26 == Ma::M010010 && a.Oc21 == Ma::M00101 && a.Oi == 0) { //QMTC2
        return e = EEis(L"QMTC2", a.Trt, a.Tfd2);
    } else if (a.Oc26 == Ma::M111110) { //SQC2
        return e = EEis(L"SQC2", a.Tft2, a.Toffbrs);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M111101) { //VABS.
        return e = EEis(L"VABS." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101000) { //VADD.
        return e = EEis(L"VADD." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100010) { //VADDi.
        return e = EEis(L"VADDi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100000) { //VADDq.
        return e = EEis(L"VADDq." + a.Vd, a.Vfdd, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0000) { //VADDbc.
        return e = EEis(L"VADD" + a.Vbc + L"." + a.Vd, a.Vfdd, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M111100) { //VADDA.
        return e = EEis(L"VADDA." + a.Vd, a.Vacc, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111110) { //VADDAi.
        return e = EEis(L"VADDAi." + a.Vd, a.Vacc, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111100) { //VADDAq.
        return e = EEis(L"VADDAq." + a.Vd, a.Vacc, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00000 && a.Oc2c4 == Ma::M1111) { //VADDAbc.
        return e = EEis(L"VADDA" + a.Vbc + L"." + a.Vd, a.Vacc, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M111000) { //VCALLMS
        return e = EEis(L"VCALLMS", a.Vimm15);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M111001) { //VCALLMSR
        return e = EEis(L"VCALLMSR", L"$vi27");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M111111) { //VCLIP.
        return e = EEis(L"VCLIPw.xyz", a.Vfsxyz, a.Vftw);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01110 && a.Oc0 == Ma::M111100) { //VDIV
        return e = EEis(L"VDIV", L"Q", a.Vfsfsf, a.Vftftf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M111100) { //VFTOI0.
        return e = EEis(L"VFTOI0." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M111101) { //VFTOI4.
        return e = EEis(L"VFTOI4." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M111110) { //VFTOI12.
        return e = EEis(L"VFTOI12." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00101 && a.Oc0 == Ma::M111111) { //VFTOI15.
        return e = EEis(L"VFTOI15." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M110000) { //VIADD
        return e = EEis(L"VIADD", a.Vidr, a.Visr, a.Vitr);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M110010) { //VIADDI
        return e = EEis(L"VIADDI", a.Vitr, a.Visr, a.Vimm5);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M110000) { //VIAND
        return e = EEis(L"VIAND", a.Vidr, a.Visr, a.Vitr);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01111 && a.Oc0 == Ma::M111110) { //VILWR.
        return e = EEis(L"VILWR." + a.Vd, a.Vitr, a.Visd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M110101) { //VIOR
        return e = EEis(L"VIOR", a.Vidr, a.Visr, a.Vitr);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M110101) { //VISUB
        return e = EEis(L"VISUB", a.Vidr, a.Visr, a.Vitr);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01111 && a.Oc0 == Ma::M111111) { //VISWR.
        return e = EEis(L"VISWR." + a.Vd, a.Vitr, a.Visd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M111100) { //VITOF0.
        return e = EEis(L"VITOF0." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M111101) { //VITOF4.
        return e = EEis(L"VITOF4." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M111110) { //VITOF12.
        return e = EEis(L"VITOF12." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00100 && a.Oc0 == Ma::M111111) { //VITOF15.
        return e = EEis(L"VITOF15." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01101 && a.Oc0 == Ma::M111110) { //VLQD.
        return e = EEis(L"VLQD." + a.Vd, a.Vftd, a.Vmmisd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01101 && a.Oc0 == Ma::M111100) { //VLQI.
        return e = EEis(L"VLQI." + a.Vd, a.Vftd, a.Visppd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101001) { //VMADD.
        return e = EEis(L"VMADD." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100011) { //VMADDi.
        return e = EEis(L"VMADDi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100001) { //VMADDq.
        return e = EEis(L"VMADDq." + a.Vd, a.Vfdd, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0010) { //VMADDbc.
        return e = EEis(L"VMADD" + a.Vbc + L"." + a.Vd, a.Vfd, a.Vfs, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M111101) { //VMADDA.
        return e = EEis(L"VMADDA." + a.Vd, a.Vacc, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111111) { //VMADDAi.
        return e = EEis(L"VMADDAi." + a.Vd, a.Vacc, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111101) { //VMADDAq.
        return e = EEis(L"VMADDAq." + a.Vd, a.Vacc, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M00010 && a.Oc2c4 == Ma::M1111) { //VMADDAbc.
        return e = EEis(L"VMADDA" + a.Vbc + L"." + a.Vd, a.Vacc, a.Vfs, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101011) { //VMAX.
        return e = EEis(L"VMAX." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M011101) { //VMAXi.
        return e = EEis(L"VMAXi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0100) { //VMAXbc.
        return e = EEis(L"VMAX" + a.Vbc + L"." + a.Vd, a.Vfdd, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01111 && a.Oc0 == Ma::M111101) { //VMFIR.
        return e = EEis(L"VMFIR." + a.Vd, a.Vftd, a.Visr);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101111) { //VMINI.
        return e = EEis(L"VMINI." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M011101) { //VMINIi.
        return e = EEis(L"VMINIi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0101) { //VMINIbc.
        return e = EEis(L"VMINI" + a.Vbc + L"." + a.Vd, a.Vfdd, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01100 && a.Oc0 == Ma::M111100) { //VMOVE.
        return e = EEis(L"VMOVE." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01100 && a.Oc0 == Ma::M111101) { //VMR32.
        return e = EEis(L"VMR32." + a.Vd, a.Vftd, a.Vfsd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101101) { //VMSUB.
        return e = EEis(L"VMSUB." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M001111) { //VMSUBi.
        return e = EEis(L"VMSUBi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100101) { //VMSUBq.
        return e = EEis(L"VMSUBq." + a.Vd, a.Vfdd, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M00010 && a.Oc2c4 == Ma::M1111) { //VMSUBbc.
        return e = EEis(L"VMSUB" + a.Vbc + L"." + a.Vd, a.Vfdd, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01011 && a.Oc0 == Ma::M111101) { //VMSUBA.
        return e = EEis(L"VMSUBA." + a.Vd, a.Vacc, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01001 && a.Oc0 == Ma::M111111) { //VMSUBAi.
        return e = EEis(L"VMSUBAi." + a.Vd, a.Vacc, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01001 && a.Oc0 == Ma::M111101) { //VMSUBAq.
        return e = EEis(L"VMSUBAq." + a.Vd, a.Vacc, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M00011 && a.Oc2c4 == Ma::M1111) { //VMSUBAbc.
        return e = EEis(L"VMSUBA" + a.Vbc + L"." + a.Vd, a.Vacc, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01111 && a.Oc0 == Ma::M111100) { //VMTIR.
        return e = EEis(L"VMTIR." + a.Vd, a.Vitr, a.Vfsfsf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101010) { //VMUL.
        return e = EEis(L"VMUL." + a.Vd, a.Vfd, a.Vfs, a.Vft);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M011110) { //VMULi.
        return e = EEis(L"VMULi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M011100) { //VMULq.
        return e = EEis(L"VMULq." + a.Vd, a.Vfdd, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0110) { //VMULbc.
        return e = EEis(L"VMUL" + a.Vbc + L"." + a.Vd, a.Vfd, a.Vfs, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01010 && a.Oc0 == Ma::M111110) { //VMULA.
        return e = EEis(L"VMULA." + a.Vd, a.Vacc, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M111110) { //VMULAi.
        return e = EEis(L"VMULAi." + a.Vd, a.Vacc, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00111 && a.Oc0 == Ma::M111100) { //VMULAq.
        return e = EEis(L"VMULAq." + a.Vd, a.Vacc, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M00110 && a.Oc2c4 == Ma::M1111) { //VMULAbc.
        return e = EEis(L"VMULA" + a.Vbc + L"." + a.Vd, a.Vacc, a.Vfs, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01011 && a.Oc0 == Ma::M111111) { //VNOP
        return e = EEis(L"VNOP");
    }

    if (false) {
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M01011 && a.Oc0 == Ma::M111110) { //VOPMULA.
        return e = EEis(L"VOPMULA.xyz", a.Vaccxyz, a.Vfsxyz, a.Vftxyz);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101110) { //VOPMSUB.
        return e = EEis(L"VOPMSUB.xyz", a.Vfdxyz, a.Vfsxyz, a.Vftxyz);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M10000 && a.Oc0 == Ma::M111101) { //VRGET.
        return e = EEis(L"VRGET." + a.Vd, a.Vftd, L"R");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M10000 && a.Oc0 == Ma::M111110) { //VRINIT
        return e = EEis(L"VRINIT", L"R", a.Vfsfsf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M10000 && a.Oc0 == Ma::M111100) { //VRNEXT
        return e = EEis(L"VRNEXT", a.Vftd, L"R");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M01110 && a.Oc0 == Ma::M111110) { //VRSQRT
        return e = EEis(L"VRSQRT", L"Q", a.Vfsfsf, a.Vftftf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M10000 && a.Oc0 == Ma::M111111) { //VRXOR
        return e = EEis(L"VRINIT", L"R", a.Vfsfsf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M01101 && a.Oc0 == Ma::M111111) { //VRSQD
        return e = EEis(L"VRSQD", L"R", a.Vmmitd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M01101 && a.Oc0 == Ma::M111111) { //VSQI
        return e = EEis(L"VSQI." + a.Vd, a.Vfsd, a.Vitppd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6c5 == Ma::M01110 && a.Oc0 == Ma::M111101) { //VSQRT
        return e = EEis(L"VSQRT", L"Q", a.Vftftf);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M101100) { //VSUB.
        return e = EEis(L"VSUB." + a.Vd, a.Vfdd, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100110) { //VSUBi.
        return e = EEis(L"VSUBi." + a.Vd, a.Vfdd, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc0 == Ma::M100000) { //VSUBq.
        return e = EEis(L"VSUBq." + a.Vd, a.Vfdd, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc2c4 == Ma::M0001) { //VSUBbc.
        return e = EEis(L"VSUB" + a.Vbc + L"." + a.Vd, a.Vfdd, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01011 && a.Oc0 == Ma::M111100) { //VSUBA.
        return e = EEis(L"VSUBA." + a.Vd, a.Vacc, a.Vfsd, a.Vftd);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111110) { //VSUBAi.
        return e = EEis(L"VSUBAi." + a.Vd, a.Vacc, a.Vfsd, L"I");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01000 && a.Oc0 == Ma::M111100) { //VSUBAq.
        return e = EEis(L"VSUBAq." + a.Vd, a.Vacc, a.Vfsd, L"Q");
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M00001 && a.Oc2c4 == Ma::M1111) { //VSUBAbc.
        return e = EEis(L"VSUBA" + a.Vbc + L"." + a.Vd, a.Vacc, a.Vfsd, a.Vftbc);
    } else if (a.Oc26 == Ma::M010010 && a.Oc25 == 1 && a.Oc6 == Ma::M01110 && a.Oc0 == Ma::M111111) { //VWAITQ
        return e = EEis(L"VWAITQ");
    }
    // TODO work HERE
    // ...
    // ...
    //

    return e = EUnk;
};

const EEis EUnk = EEis(L"?");

wxString EEis::ToString()
{
    wxString s = al[0];
    while (s.size() < 7)
        s += L" ";
    s += L" ";
    for (size_t x = 1; x < al.size(); x++) {
        if (x != 1)
            s += L", ";
        s += al[x];
    }
    return s;
}

const wxChar *const eedNameGPR[] = {
    L"zero",
    L"at",
    L"v0",
    L"v1",
    L"a0",
    L"a1",
    L"a2",
    L"a3",
    L"t0",
    L"t1",
    L"t2",
    L"t3",
    L"t4",
    L"t5",
    L"t6",
    L"t7",
    L"s0",
    L"s1",
    L"s2",
    L"s3",
    L"s4",
    L"s5",
    L"s6",
    L"s7",
    L"t8",
    L"t9",
    L"k0",
    L"k1",
    L"gp",
    L"sp",
    L"s8",
    L"ra",
};

} // namespace EEDisarm
