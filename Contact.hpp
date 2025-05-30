#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <wx/string.h>

class Contact {
public:
    Contact();
    Contact(const wxString& name, const wxString& phone, const wxString& email);

    wxString GetName() const;
    wxString GetPhone() const;
    wxString GetEmail() const;

    void SetName(const wxString& name);
    void SetPhone(const wxString& phone);
    void SetEmail(const wxString& email);

private:
    wxString name;
    wxString phone;
    wxString email;
};

#endif // CONTACT_HPP
