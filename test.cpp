#include "TelephoneBookLogic.hpp"
#include "Contact.hpp"
#include <wx/app.h>      // Needed for wx initialization
#include <wx/log.h>
#include <wx/string.h>
#include <iostream>
#include <filesystem>
#include <regex>
#include <string>

// Dummy wxWidgets app to allow wx usage in console app
class DummyApp : public wxApp {
public:
    virtual bool OnInit() override { return true; }
};

wxIMPLEMENT_APP_NO_MAIN(DummyApp);

// Validate phone number: only digits, length 10 or 11
bool IsValidPhone(const std::string& phone) {
    std::regex phone_regex("^[0-9]{10,11}$");
    return std::regex_match(phone, phone_regex);
}

// Validate email format (simple)
bool IsValidEmail(const std::string& email) {
    std::regex email_regex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return std::regex_match(email, email_regex);
}

int main(int argc, char** argv) {
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();

    wxString dbPath = "test_phonebook.db";
    if (std::filesystem::exists(dbPath.ToStdString())) {
        std::filesystem::remove(dbPath.ToStdString());
    }
    TelephoneBookLogic phonebook(dbPath);

    // Helper lambda to validate and add contact
    auto tryAddContact = [&](const Contact& c) {
        std::string phone = c.GetPhone().ToStdString();
        std::string email = c.GetEmail().ToStdString();

        if (!IsValidPhone(phone)) {
            std::cout << "Invalid phone number: " << phone << std::endl;
            return false;
        }
        if (!IsValidEmail(email)) {
            std::cout << "Invalid email address: " << email << std::endl;
            return false;
        }
        return phonebook.AddContact(c);
    };

    Contact contact1("Alice Smith", "1234567890", "alice@example.com");
    Contact contact2("Bob Jones", "9876543210", "bob@example.com");

    // Valid contacts
    std::cout << "Adding Alice: " << (tryAddContact(contact1) ? "Success" : "Failure") << std::endl;
    std::cout << "Adding Bob: " << (tryAddContact(contact2) ? "Success" : "Failure") << std::endl;

    // Invalid phone number (too short)
    Contact invalidPhone("Tom Error", "1234", "tom@example.com");
    std::cout << "Adding Tom with invalid phone: " << (tryAddContact(invalidPhone) ? "Success" : "Failure") << std::endl;

    // Invalid email (missing @)
    Contact invalidEmail("Jane Error", "1234567890", "janeexample.com");
    std::cout << "Adding Jane with invalid email: " << (tryAddContact(invalidEmail) ? "Success" : "Failure") << std::endl;

    // Search for contact
    std::vector<Contact> results = phonebook.SearchContacts("alice");
    std::cout << "Search results for 'alice': " << results.size() << " contact(s) found." << std::endl;

    // Edit contact - valid update
    Contact updatedAlice("Alice Johnson", "1234567890", "alicej@example.com");
    std::cout << "Editing Alice: "
              << (phonebook.EditContact("Alice Smith", "1234567890", updatedAlice) ? "Success" : "Failure")
              << std::endl;

    // Edit contact - invalid phone (should fail if validation is added to EditContact)
    Contact badUpdate("Alice Johnson", "123", "alicej@example.com");
    std::cout << "Editing Alice with invalid phone: "
              << (phonebook.EditContact("Alice Johnson", "1234567890", badUpdate) ? "Success" : "Failure")
              << std::endl;

    // Delete contact
    std::cout << "Deleting Bob: "
              << (phonebook.DeleteContact("Bob Jones", "9876543210") ? "Success" : "Failure")
              << std::endl;

    wxEntryCleanup();
    return 0;
}
