#ifndef CONTACT_HPP
#define CONTACT_HPP

#pragma once
#include <wx/string.h>

class Contact {
public:
    Contact();
    Contact(const wxString& name, const wxString& phone, const wxString& email);

    wxString GetName() const;
    wxString GetPhone() const;
    wxString GetEmail() const;

    // Use a return type (e.g., bool) to indicate success/failure of setting
    bool SetName(const wxString& name);
    bool SetPhone(const wxString& phone); // Now returns bool
    bool SetEmail(const wxString& email); // Now returns bool

    // New validation methods
    bool IsValidPhone(const wxString& phone) const;
    bool IsValidEmail(const wxString& email) const;

private:
    wxString name;
    wxString phone;
    wxString email;
};

#endif // CONTACT_HPP
