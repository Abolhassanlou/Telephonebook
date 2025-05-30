#include "TelephoneBook.hpp"
#include "TelephoneBookLogic.hpp"

#include <wx/msgdlg.h> // For wxMessageBox
#include <wx/log.h>    // For wxLogMessage and wxLogError
#include <wx/sizer.h>  // For wxBoxSizer (used in GUI layout)

// Constructor for the TelephoneBook GUI frame
TelephoneBook::TelephoneBook(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(450, 1000)) {

    wxLogMessage("TelephoneBook GUI constructor started.");

    // IMPORTANT: Instantiate the core logic class.
    // This is where the TelephoneBookLogic object is created.
    coreLogic = new TelephoneBookLogic();

    // Create a panel to hold the UI elements
    wxPanel* panel = new wxPanel(this);

    // Create a vertical sizer for layout management
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Create labels for input fields
    wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, wxT("Name:"));
    wxStaticText* phoneLabel = new wxStaticText(panel, wxID_ANY, wxT("Phone:"));
    wxStaticText* emailLabel = new wxStaticText(panel, wxID_ANY, wxT("Email:"));
    wxStaticText* searchLabel = new wxStaticText(panel, wxID_ANY, wxT("Search:"));

    // Create input text controls
    nameInput = new wxTextCtrl(panel, wxID_ANY);
    phoneInput = new wxTextCtrl(panel, wxID_ANY);
    emailInput = new wxTextCtrl(panel, wxID_ANY);
    searchInput = new wxTextCtrl(panel, wxID_ANY);

    // Create buttons (using wxID_ANY now instead of hardcoded IDs for better practice)
    addButton = new wxButton(panel, wxID_ANY, wxT("Add Contact"));
    searchButton = new wxButton(panel, wxID_ANY, wxT("Search Contact"));
    sortButton = new wxButton(panel, wxID_ANY, wxT("Sort /All"));
    deleteButton = new wxButton(panel, wxID_ANY, wxT("Delete"));
    editButton = new wxButton(panel, wxID_ANY, wxT("Edit"));

    // Bind buttons to their respective event handlers
    addButton->Bind(wxEVT_BUTTON, &TelephoneBook::OnAddContact, this);
    searchButton->Bind(wxEVT_BUTTON, &TelephoneBook::OnSearchContact, this);
    sortButton->Bind(wxEVT_BUTTON, &TelephoneBook::OnSortContact, this);
    deleteButton->Bind(wxEVT_BUTTON, &TelephoneBook::OnDeleteContact, this);
    editButton->Bind(wxEVT_BUTTON, &TelephoneBook::OnEditContact, this);

    // Create the wxListCtrl to display contacts
    contactList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(300, 200), wxLC_REPORT);
    // Add columns to the list control
    contactList->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 120);
    contactList->InsertColumn(1, "Phone", wxLIST_FORMAT_LEFT, 100);
    contactList->InsertColumn(2, "Email", wxLIST_FORMAT_LEFT, 150);

    // Arrange labels and input fields in the sizer
    vbox->Add(nameLabel, 0, wxEXPAND | wxALL, 5);
    vbox->Add(nameInput, 0, wxEXPAND | wxALL, 5);
    vbox->Add(phoneLabel, 0, wxEXPAND | wxALL, 5);
    vbox->Add(phoneInput, 0, wxEXPAND | wxALL, 5);
    vbox->Add(emailLabel, 0, wxEXPAND | wxALL, 5);
    vbox->Add(emailInput, 0, wxEXPAND | wxALL, 5);

    vbox->Add(searchLabel, 0, wxEXPAND | wxALL, 5);
    vbox->Add(searchInput, 0, wxEXPAND | wxALL, 5);

    // Arrange buttons and the contact list
    vbox->Add(editButton, 0, wxEXPAND | wxALL, 5);
    vbox->Add(deleteButton, 0, wxEXPAND | wxALL, 5);
    vbox->Add(addButton, 0, wxEXPAND | wxALL, 5);
    vbox->Add(searchButton, 0, wxEXPAND | wxALL, 5);
    vbox->Add(sortButton, 0, wxEXPAND | wxALL, 5);
    vbox->Add(contactList, 1, wxEXPAND | wxALL, 5); // Use proportion 1 to allow the list to expand

    // Set the sizer for the panel and fit it
    panel->SetSizerAndFit(vbox);

    // Bind the list selection event to its handler
    contactList->Bind(wxEVT_LIST_ITEM_SELECTED, &TelephoneBook::OnContactSelected, this);

    // Load and display contacts from the database initially
    RefreshList(); // Calls the logic to get all contacts
    wxLogMessage("TelephoneBook GUI initialized.");
}

// Destructor for the TelephoneBook GUI frame
TelephoneBook::~TelephoneBook() {
    // IMPORTANT: Clean up the dynamically allocated core logic object
    delete coreLogic;
    coreLogic = nullptr; // Prevent dangling pointer
    wxLogMessage("TelephoneBook GUI destructor called.");
}

// Method to refresh the wxListCtrl with contacts from the coreLogic
void TelephoneBook::RefreshList() {
    contactList->DeleteAllItems(); // Clear current items in the list

    // Get contacts from the core logic
    const std::vector<Contact>& currentContacts = coreLogic->GetContacts();

    // Populate the list with contacts from the provided vector
    for (size_t i = 0; i < currentContacts.size(); ++i) {
        contactList->InsertItem(i, currentContacts[i].GetName());
        contactList->SetItem(i, 1, currentContacts[i].GetPhone());
        contactList->SetItem(i, 2, currentContacts[i].GetEmail());
    }
}

// --- Event Handlers (contain GUI interactions and delegate to coreLogic) ---

// Handler for the "Add Contact" button
void TelephoneBook::OnAddContact(wxCommandEvent& event) {
    wxString name = nameInput->GetValue();
    wxString phone = phoneInput->GetValue();
    wxString email = emailInput->GetValue();

    // Input validation (remains in GUI as it's user-facing)
    if (name.IsEmpty() || phone.IsEmpty()) {
        wxMessageBox("Name and Phone fields cannot be empty!", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
    if (!phone.IsNumber()) {
        wxMessageBox("Phone must contain only numbers!", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
    if (phone.Length() < 11) {
        wxMessageBox("Phone number must be at least 11 digits!", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
    // Simple email validation (can be enhanced)
    if(!email.IsEmpty()){if (!email.Contains("@") || !email.Contains(".")) {
        wxMessageBox("Invalid email! It must contain both '@' and '.' characters.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
}

    Contact newContact(name, phone, email);

    // Delegate the adding of the contact to the core logic
    if (coreLogic->AddContact(newContact)) {
        wxMessageBox("Contact added successfully!", "Success", wxOK | wxICON_INFORMATION);
        // Clear input fields after successful addition
        nameInput->Clear();
        phoneInput->Clear();
        emailInput->Clear();
        searchInput->Clear();
        // Refresh the list to show the newly added contact
        RefreshList();
    } else {
        wxMessageBox("Failed to add contact! See logs for details.", "Error", wxOK | wxICON_ERROR);
    }
}

// Handler for the "Search Contact" button
void TelephoneBook::OnSearchContact(wxCommandEvent& event) {
    wxString search = searchInput->GetValue();

    // Delegate the search operation to the core logic
    std::vector<Contact> searchResults = coreLogic->SearchContacts(search);

    contactList->DeleteAllItems(); // Clear the list before displaying results

    if (searchResults.empty()) {
        wxMessageBox("No contacts found matching your search.", "Search Results", wxOK | wxICON_INFORMATION);
    }
    // Populate the list with search results
    for (size_t i = 0; i < searchResults.size(); ++i) {
        contactList->InsertItem(i, searchResults[i].GetName());
        contactList->SetItem(i, 1, searchResults[i].GetPhone());
        contactList->SetItem(i, 2, searchResults[i].GetEmail());
    }

    // If search field was empty, refresh to show all contacts
    if (search.IsEmpty()) {
        wxMessageBox("The search field is empty. Displaying all contacts.", "Info", wxOK | wxICON_INFORMATION);
        RefreshList();
    }
}

// Handler for the "Sort Contacts" button
void TelephoneBook::OnSortContact(wxCommandEvent& event) {
    // Delegate the sort operation to the core logic
    coreLogic->SortContactsByName();
    // Refresh the list to show the sorted order
    RefreshList();
    wxMessageBox("Contacts sorted by name.", "Sort", wxOK | wxICON_INFORMATION);
}

// Handler for quitting the application (e.g., from a menu item if one existed)
void TelephoneBook::OnQuit(wxCommandEvent& event) {
    Close(true); // Close the frame
}

// Handler for when an item in the wxListCtrl is selected
void TelephoneBook::OnContactSelected(wxListEvent& event) {
    long itemIndex = event.GetIndex(); // Get the index of the selected item

    if (itemIndex == wxNOT_FOUND) {
        return; // No item was selected
    }

    // Get the values from the selected item's columns
    wxString name = contactList->GetItemText(itemIndex, 0);  // Column 0 = Name
    wxString phone = contactList->GetItemText(itemIndex, 1); // Column 1 = Phone
    wxString email = contactList->GetItemText(itemIndex, 2); // Column 2 = Email

    // Populate the input fields with the selected contact's details
    nameInput->SetValue(name);
    phoneInput->SetValue(phone);
    emailInput->SetValue(email);
}

// Handler for the "Delete" button
void TelephoneBook::OnDeleteContact(wxCommandEvent& event) {
    long selected = contactList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == -1) {
        wxMessageBox("Please select a contact to delete.", "Deletion Error", wxOK | wxICON_ERROR);
        return;
    }

    // Get the name and phone of the selected contact to identify it
    wxString nameToDelete = contactList->GetItemText(selected, 0);
    wxString phoneToDelete = contactList->GetItemText(selected, 1); // Using phone as part of key

    // Ask for confirmation before deleting
    int res = wxMessageBox("Are you sure you want to delete this contact?", "Confirm Delete", wxYES_NO | wxICON_QUESTION);
    if (res != wxYES) {
        return; // User cancelled
    }

    // Delegate the deletion to the core logic
    if (coreLogic->DeleteContact(nameToDelete, phoneToDelete)) {
        wxMessageBox("Contact deleted successfully.", "Success", wxOK | wxICON_INFORMATION);
        // Clear input fields and refresh list after successful deletion
        nameInput->Clear();
        phoneInput->Clear();
        emailInput->Clear();
        searchInput->Clear();
        RefreshList();
    } else {
        wxMessageBox("Failed to delete contact! See logs for details.", "Error", wxOK | wxICON_ERROR);
    }
}

// Handler for the "Edit" button
void TelephoneBook::OnEditContact(wxCommandEvent& event) {
    long selected = contactList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected == -1) {
        wxMessageBox("Please select a contact to edit.", "Edit Error", wxOK | wxICON_ERROR);
        return;
    }

    // Get the old contact details from the list control
    wxString oldName = contactList->GetItemText(selected, 0);
    wxString oldPhone = contactList->GetItemText(selected, 1);

    // Get the new (edited) contact details from the input fields
    wxString newName = nameInput->GetValue();
    wxString newPhone = phoneInput->GetValue();
    wxString newEmail = emailInput->GetValue();

    // Input validation for the new values
    if (newName.IsEmpty() || newPhone.IsEmpty()) {
        wxMessageBox("Name and Phone cannot be empty.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
    if (!newPhone.IsNumber()) {
        wxMessageBox("Phone must contain only numbers.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }
    if (newPhone.Length() < 11) {
        wxMessageBox("Phone number must be at least 11 digits.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }

     if(!newEmail.IsEmpty()){if (!newEmail.Contains("@") || !newEmail.Contains(".")) {
        wxMessageBox("Invalid email! It must contain both '@' and '.' characters.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }}

     int response = wxMessageBox("Are you sure you want to update this contact?", "Confirm Update", wxOK | wxCANCEL | wxICON_QUESTION);
    if (response == wxCANCEL) {
        return; // User clicked Cancel
    }

    // Create a new Contact object with the updated details
    Contact updatedContact(newName, newPhone, newEmail);

    // Delegate the update operation to the core logic
    if (coreLogic->EditContact(oldName, oldPhone, updatedContact)) {
        wxMessageBox("Contact updated successfully.", "Success", wxOK | wxICON_INFORMATION);
        // Refresh the list to show the updated contact
        RefreshList();
    } else {
        wxMessageBox("Failed to update contact! See logs for details.", "Error", wxOK | wxICON_ERROR);
    }
    nameInput->Clear();
        phoneInput->Clear();
        emailInput->Clear();
        searchInput->Clear();
}
