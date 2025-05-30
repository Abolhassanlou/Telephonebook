#include "Contact.hpp"
#include <wx/log.h> // For logging errors, useful for debugging
#include <cctype>   // For isdigit

Contact::Contact() {}

Contact::Contact(const wxString& name, const wxString& phone, const wxString& email)
    : name(name), phone(phone), email(email) {}

wxString Contact::GetName() const {
    return name;
}

wxString Contact::GetPhone() const {
    return phone;
}

wxString Contact::GetEmail() const {
    return email;
}

bool Contact::SetName(const wxString& name) {
    if (name.IsEmpty()) {
        wxLogError("Name cannot be empty.");
        return false;
    }
    this->name = name;
    return true;
}

bool Contact::SetPhone(const wxString& phone) {
    if (IsValidPhone(phone)) {
        this->phone = phone;
        return true;
    }
    // Log an error if validation fails
    wxLogError("Invalid phone number: '%s'. Phone must be at least 11 digits and contain only numbers.", phone);
    return false;
}

bool Contact::SetEmail(const wxString& email) {
    if (IsValidEmail(email)) {
        this->email = email;
        return true;
    }
    // Log an error if validation fails
    wxLogError("Invalid email: '%s'. Email must be empty or contain '@' and '.' after '@'.", email);
    return false;
}

// --- Validation Implementations ---

bool Contact::IsValidPhone(const wxString& phone) const {
    // 1. Phone number must be at least 11 characters
    if (phone.Length() < 11) {
        return false;
    }

    // 2. All characters must be digits
    for (wxChar c : phone) {
        if (!wxIsdigit(c)) { // Use wxIsdigit for wxChar
            return false;
        }
    }

    return true;
}

bool Contact::IsValidEmail(const wxString& email) const {
    // If email is empty, it's considered valid (as per your requirement "if it is not empty")
    if (email.IsEmpty()) {
        return true;
    }

    // Must contain '@' and '.'
    int atPos = email.Find('@');
    if (atPos == wxNOT_FOUND || atPos == 0 || atPos == email.Length() - 1) {
        // No '@' found, or '@' is at the beginning, or '@' is at the end
        return false;
    }

    int dotPos = email.Find('.', atPos + 1); // Find '.' after '@'
    if (dotPos == wxNOT_FOUND || dotPos == atPos + 1 || dotPos == email.Length() - 1) {
        // No '.' found after '@', or '.' is immediately after '@', or '.' is at the end
        return false;
    }

    // Optional: Check for spaces (emails shouldn't have spaces)
    if (email.Contains(' ')) {
        return false;
    }

    return true;
}
