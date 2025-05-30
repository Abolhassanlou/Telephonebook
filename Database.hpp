#ifndef TELEPHONEBOOKLOGIC_HPP
#define TELEPHONEBOOKLOGIC_HPP

#include "Contact.hpp"
#include "sqlite3.h"
#include <vector>
#include <wx/string.h> // Make sure this is included for wxString

class TelephoneBookLogic {
public:
    // THIS IS THE CRUCIAL CHANGE: Add this constructor
    TelephoneBookLogic(const wxString& dbPath = "contacts.db");
    ~TelephoneBookLogic();

    // ... (rest of your public methods)
    bool AddContact(const Contact& contact);
    std::vector<Contact> SearchContacts(const wxString& query);
    void SortContactsByName();
    bool DeleteContact(const wxString& name, const wxString& phone);
    bool EditContact(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact);
   const std::vector<Contact>& GetAllContacts() const {
    return contacts;
}

private:
    std::vector<Contact> contacts;
    sqlite3* db;
    wxString databasePath; // Make sure this member variable exists

    void OpenDatabase();
    void CloseDatabase();
    bool SaveContactToDatabase(const Contact& contact);
    void LoadContactsFromDatabase();
    bool DeleteContactFromDatabase(const wxString& name, const wxString& phone);
    bool UpdateContactInDatabase(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact);
};

#endif // TELEPHONEBOOKLOGIC_HPP
