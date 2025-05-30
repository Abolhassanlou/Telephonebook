#ifndef TELEPHONEBOOKLOGIC_HPP
#define TELEPHONEBOOKLOGIC_HPP

#include <wx/string.h>
#include <vector>
#include <sqlite3.h>
#include "Contact.hpp"  // Assuming you have a Contact class header

class TelephoneBookLogic {
public:
    // Constructor / Destructor
    explicit TelephoneBookLogic(const wxString& dbPath = "contacts.db");
    ~TelephoneBookLogic();

    // Public interface
    bool AddContact(const Contact& contact);
    std::vector<Contact> SearchContacts(const wxString& query);
    void SortContactsByName();
    bool DeleteContact(const wxString& name, const wxString& phone);
    bool EditContact(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact);

    // Getter for the in-memory contacts list
    const std::vector<Contact>& GetContacts() const { return contacts; }

private:
    // Database handling
    void OpenDatabase();
    void CloseDatabase();

    // Loads contacts from DB into memory vector
    void LoadContactsFromDatabase();

    // Internal helpers for DB operations
    bool SaveContactToDatabase(const Contact& contact);
    bool UpdateContactInDatabase(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact);
    bool DeleteContactFromDatabase(const wxString& name, const wxString& phone);

private:
    sqlite3* db = nullptr;                // SQLite database handle
    wxString databasePath;               // Path to the SQLite database file
    std::vector<Contact> contacts;       // In-memory cache of contacts
};

#endif // TELEPHONEBOOKLOGIC_HPP
