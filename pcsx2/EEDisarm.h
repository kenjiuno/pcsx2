
#ifndef __EEDISARM_H__
#define __EEDISARM_H__

#include <wx/arrstr.h>

namespace EEDisarm
{
class EEis
{
public:
    wxArrayString al;

    EEis()
    {
    }
    EEis(wxString opc)
    {
        al.Add(opc);
    }
    EEis(wxString opc, wxString opr1)
    {
        al.Add(opc);
        al.Add(opr1);
    }
    EEis(wxString opc, wxString opr1, wxString opr2)
    {
        al.Add(opc);
        al.Add(opr1);
        al.Add(opr2);
    }
    EEis(wxString opc, wxString opr1, wxString opr2, wxString opr3)
    {
        al.Add(opc);
        al.Add(opr1);
        al.Add(opr2);
        al.Add(opr3);
    }

    wxString ToString();
};

EEis &Parse(uint code, uint pc, EEis &e);

extern const EEis EUnk;

extern const wxChar *const eedNameGPR[];
} // namespace EEDisarm

#endif