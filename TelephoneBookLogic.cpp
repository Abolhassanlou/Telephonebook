
#include "TelephoneBookLogic.hpp" // Make sure this is included
#include <wx/log.h> // Needed for wxLogMessage

// New constructor implementation
TelephoneBookLogic::TelephoneBookLogic(const wxString& dbPath) : db(nullptr), databasePath(dbPath) {
    wxLogMessage("TelephoneBookLogic constructor started for DB: %s", databasePath);
    OpenDatabase(); // Call OpenDatabase after setting databasePath
    LoadContactsFromDatabase();
    wxLogMessage("TelephoneBookLogic initialized for DB: %s", databasePath);
}

// Ensure destructor cleans up
TelephoneBookLogic::~TelephoneBookLogic() {
    CloseDatabase();
    wxLogMessage("TelephoneBookLogic destructor called.");
}

void TelephoneBookLogic::OpenDatabase() {
    wxLogMessage("Opening database at: %s", databasePath); // Use the member variable
    int rc = sqlite3_open(databasePath.ToStdString().c_str(), &db);
    if (rc != SQLITE_OK) {
        wxLogError("Cannot open database: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = nullptr; // Important to set to nullptr if opening failed
        return;
    }
    // Create contacts table if it doesn't exist
    const char* sql = "CREATE TABLE IF NOT EXISTS contacts (name TEXT, phone TEXT, email TEXT);";
    char* errMsg = nullptr;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        wxLogError("SQL error creating table: %s", errMsg);
        sqlite3_free(errMsg);
    }
}
void TelephoneBookLogic::CloseDatabase() {
    if (db) {
        int rc = sqlite3_close(db);
        if (rc != SQLITE_OK) {
            // Handle error closing the database (e.g., log it)
            wxLogError("Failed to close database: %s", sqlite3_errmsg(db));
        } else {
            wxLogMessage("Database closed successfully.");
        }
        db = nullptr; // Set to nullptr after closing
    }
}

bool TelephoneBookLogic::AddContact(const Contact& contact) {
    if (!db) {
        wxLogError("Database not open, cannot add contact.");
        return false;
    }

    // Check for duplicate phone number
    const char* checkSql = "SELECT COUNT(*) FROM contacts WHERE phone = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, 0);
    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare duplicate check statement: %s", sqlite3_errmsg(db));
        return false;
    }
    sqlite3_bind_text(checkStmt, 1, contact.GetPhone().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(checkStmt);
    int count = sqlite3_column_int(checkStmt, 0);
    sqlite3_finalize(checkStmt);

    if (count > 0) {
        wxLogError("Contact with phone number '%s' already exists.", contact.GetPhone());
        return false; // Prevent adding duplicate phone numbers
    }

    const char* sql = "INSERT INTO contacts (name, phone, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare statement for adding contact: %s", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, contact.GetName().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, contact.GetPhone().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, contact.GetEmail().ToStdString().c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        wxLogError("Failed to insert contact: %s", sqlite3_errmsg(db));
        return false;
    }
    LoadContactsFromDatabase(); // Reload contacts after modification to update in-memory list
    return true;
}

std::vector<Contact> TelephoneBookLogic::SearchContacts(const wxString& query) {
    std::vector<Contact> results;
    if (!db) {
        wxLogError("Database not open, cannot search contacts.");
        return results;
    }

    wxString likeQuery = "%" + query.Lower() + "%"; // Case-insensitive search
    const char* sql = "SELECT name, phone, email FROM contacts WHERE LOWER(name) LIKE ? OR LOWER(phone) LIKE ? OR LOWER(email) LIKE ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare search statement: %s", sqlite3_errmsg(db));
        return results;
    }

    sqlite3_bind_text(stmt, 1, likeQuery.ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, likeQuery.ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, likeQuery.ToStdString().c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        wxString name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        wxString phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        wxString email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        results.emplace_back(name, phone, email);
    }

    sqlite3_finalize(stmt);
    return results;
}

void TelephoneBookLogic::SortContactsByName() {
    std::sort(contacts.begin(), contacts.end(),
              [](const Contact& a, const Contact& b) {
                  return a.GetName().CmpNoCase(b.GetName()) < 0;
              });
    // For sorting, we typically just sort the in-memory 'contacts' vector,
    // as the database itself doesn't need to be reordered for display.
    // If you need persistent sort order, you'd need to modify the DB schema
    // and re-save contacts with an order field.
}

bool TelephoneBookLogic::DeleteContact(const wxString& name, const wxString& phone) {
    if (!db) {
        wxLogError("Database not open, cannot delete contact.");
        return false;
    }

    const char* sql = "DELETE FROM contacts WHERE name = ? AND phone = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare delete statement: %s", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, phone.ToStdString().c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        wxLogError("Failed to delete contact: %s", sqlite3_errmsg(db));
        return false;
    }
    LoadContactsFromDatabase(); // Reload contacts to reflect deletion
    return true;
}

bool TelephoneBookLogic::EditContact(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact) {
    if (!db) {
        wxLogError("Database not open, cannot edit contact.");
        return false;
    }

    const char* sql = "UPDATE contacts SET name = ?, phone = ?, email = ? WHERE name = ? AND phone = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare edit statement: %s", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, updatedContact.GetName().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, updatedContact.GetPhone().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, updatedContact.GetEmail().ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, oldName.ToStdString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, oldPhone.ToStdString().c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        wxLogError("Failed to update contact: %s", sqlite3_errmsg(db));
        return false;
    }
    LoadContactsFromDatabase(); // Reload contacts to reflect update
    return true;
}

void TelephoneBookLogic::LoadContactsFromDatabase() {
    contacts.clear(); // Clear existing contacts
    if (!db) {
        wxLogError("Database not open, cannot load contacts.");
        return;
    }

    const char* sql = "SELECT name, phone, email FROM contacts ORDER BY name COLLATE NOCASE;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        wxLogError("Failed to prepare statement to load contacts: %s", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        wxString name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        wxString phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        wxString email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        contacts.emplace_back(name, phone, email);
    }

    sqlite3_finalize(stmt);
    wxLogMessage("Contacts loaded from database. Count: %zu", contacts.size());
}

// Private helper to save to database (used internally by AddContact, EditContact, DeleteContact)
// This is not called directly from the GUI layer, so it can remain private.
// Note: This function is primarily for internal consistency if you had multiple
// places directly inserting into the DB. Since AddContact, EditContact, DeleteContact
// already handle DB operations, this specific 'SaveContactToDatabase' might be redundant
// unless it's designed for bulk saving. For this context, I'll provide a minimal example.
bool TelephoneBookLogic::SaveContactToDatabase(const Contact& contact) {
    // This function is now implicitly handled by AddContact.
    // If you need a direct "save" independent of "add" for existing contacts,
    // you'd typically use 'UpdateContactInDatabase' or similar.
    // For now, if called, it acts like an add.
    return AddContact(contact); // Reusing AddContact for simplicity here.
}


// Private helper to update contact in database
bool TelephoneBookLogic::UpdateContactInDatabase(const wxString& oldName, const wxString& oldPhone, const Contact& updatedContact) {
    // This function is the core of what EditContact does
    return EditContact(oldName, oldPhone, updatedContact);
}

// Private helper to delete contact from database
bool TelephoneBookLogic::DeleteContactFromDatabase(const wxString& name, const wxString& phone) {
    // This function is the core of what DeleteContact does
    return DeleteContact(name, phone);
}
