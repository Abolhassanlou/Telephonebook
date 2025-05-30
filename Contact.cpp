#include "Contact.hpp"

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
//refers to the member variable name of the current object.name (without this->) refers to the parameter of the function.
void Contact::SetName(const wxString& name) {
     // Member variable     Parameter
    this->name = name;
}

void Contact::SetPhone(const wxString& phone) {
    this->phone = phone;
}

void Contact::SetEmail(const wxString& email) {
    this->email = email;
}
