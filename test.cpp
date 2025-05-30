// test.cpp
#include "TelephoneBookLogic.hpp"
#include "Contact.hpp"
#include <wx/app.h> // Needed for wx initialization
#include <wx/log.h> // For wxLogError messages
#include <wx/string.h>
#include <iostream>
#include <filesystem>
#include <vector> // Required for std::vector

class DummyApp : public wxApp {
public:
    virtual bool OnInit() override { return true; }
};

wxIMPLEMENT_APP_NO_MAIN(DummyApp);

int main(int argc, char** argv) {
    wxEntryStart(argc, argv);
    wxTheApp->CallOnInit();

    wxString dbPath = "test_phonebook.db";
    if (std::filesystem::exists(dbPath.ToStdString())) {
        std::filesystem::remove(dbPath.ToStdString());
    }
    TelephoneBookLogic phonebook(dbPath);

    // --- Test 1: Valid Contacts ---
    std::cout << "--- Testing Valid Contacts ---" << std::endl;
    Contact contact1;
    bool success1 = contact1.SetName("Alice Smith") &&
                    contact1.SetPhone("12345678901") && // 11 digits
                    contact1.SetEmail("alice@example.com");
    std::cout << "Attempting to create Alice (valid): " << (success1 ? "SUCCESS" : "FAILURE") << std::endl;
    if (success1) {
        std::cout << "Adding Alice: " << (phonebook.AddContact(contact1) ? "Success" : "Failure") << std::endl;
    }

    Contact contact2;
    bool success2 = contact2.SetName("Bob Jones") &&
                    contact2.SetPhone("98765432109") && // 11 digits
                    contact2.SetEmail("bob@example.com");
    std::cout << "Attempting to create Bob (valid): " << (success2 ? "SUCCESS" : "FAILURE") << std::endl;
    if (success2) {
        std::cout << "Adding Bob: " << (phonebook.AddContact(contact2) ? "Success" : "Failure") << std::endl;
    }

    // --- Test 2: Invalid Phone Numbers ---
    std::cout << "\n--- Testing Invalid Phone Numbers ---" << std::endl;
    Contact contactInvalidPhone1;
    // Phone too short
    bool successInvalidPhone1 = contactInvalidPhone1.SetName("Charlie Brown") &&
                                contactInvalidPhone1.SetPhone("123") &&
                                contactInvalidPhone1.SetEmail("charlie@example.com");
    std::cout << "Attempting to create Charlie (phone too short): " << (successInvalidPhone1 ? "SUCCESS" : "FAILURE") << std::endl;
    // Phone with non-digits
    Contact contactInvalidPhone2;
    bool successInvalidPhone2 = contactInvalidPhone2.SetName("David Green") &&
                                contactInvalidPhone2.SetPhone("123-456-7890") &&
                                contactInvalidPhone2.SetEmail("david@example.com");
    std::cout << "Attempting to create David (phone with dashes): " << (successInvalidPhone2 ? "SUCCESS" : "FAILURE") << std::endl;


    // --- Test 3: Invalid Email Addresses ---
    std::cout << "\n--- Testing Invalid Email Addresses ---" << std::endl;
    Contact contactInvalidEmail1;
    // Email missing @
    bool successInvalidEmail1 = contactInvalidEmail1.SetName("Eve White") &&
                                contactInvalidEmail1.SetPhone("11223344556") &&
                                contactInvalidEmail1.SetEmail("eveexample.com");
    std::cout << "Attempting to create Eve (email missing '@'): " << (successInvalidEmail1 ? "SUCCESS" : "FAILURE") << std::endl;

    Contact contactInvalidEmail2;
    // Email missing . after @
    bool successInvalidEmail2 = contactInvalidEmail2.SetName("Frank Black") &&
                                contactInvalidEmail2.SetPhone("22334455667") &&
                                contactInvalidEmail2.SetEmail("frank@examplecom");
    std::cout << "Attempting to create Frank (email missing '.' after '@'): " << (successInvalidEmail2 ? "SUCCESS" : "FAILURE") << std::endl;

    Contact contactInvalidEmail3;
    // Email with space
    bool successInvalidEmail3 = contactInvalidEmail3.SetName("Grace Kelly") &&
                                contactInvalidEmail3.SetPhone("33445566778") &&
                                contactInvalidEmail3.SetEmail("grace @example.com");
    std::cout << "Attempting to create Grace (email with space): " << (successInvalidEmail3 ? "SUCCESS" : "FAILURE") << std::endl;


    // --- Test 4: Search, Edit, Delete with valid data ---
    std::cout << "\n--- Continuing with core functionality tests ---" << std::endl;

    // Search for contact
    std::vector<Contact> results = phonebook.SearchContacts("Alice"); // Note: Search usually case-insensitive or needs adjustment
    std::cout << "Search results for 'Alice': " << results.size() << " contact(s) found." << std::endl;
    if (!results.empty()) {
        std::cout << "Found: " << results[0].GetName() << ", " << results[0].GetPhone() << ", " << results[0].GetEmail() << std::endl;
    }

    // Edit contact (ensure updatedAlice uses valid data)
    Contact updatedAlice;
    bool updateAliceSuccess = updatedAlice.SetName("Alice Johnson") &&
                              updatedAlice.SetPhone("12345678901") && // Must be 11 digits and numeric
                              updatedAlice.SetEmail("alicej@example.com"); // Valid email

    if (updateAliceSuccess) {
        std::cout << "Editing Alice: "
                  << (phonebook.EditContact("Alice Smith", "12345678901", updatedAlice) ? "Success" : "Failure")
                  << std::endl;
    } else {
        std::cout << "Skipping Alice edit due to invalid update data." << std::endl;
    }


    // Delete contact
    std::cout << "Deleting Bob: "
              << (phonebook.DeleteContact("Bob Jones", "98765432109") ? "Success" : "Failure")
              << std::endl;

    // Clean up
    wxEntryCleanup();
    return 0;
}
