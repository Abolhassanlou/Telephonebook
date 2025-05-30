#ifndef TELEPHONEBOOK_HPP
#define TELEPHONEBOOK_HPP

#include <wx/wx.h>         // Core wxWidgets classes
#include <wx/listctrl.h>   // For wxListCtrl
#include "Contact.hpp"     // Definition of the Contact class

// Forward declaration of TelephoneBookLogic.
// We only need to know that this class exists here,
// as TelephoneBook will hold a pointer to it.
// The full definition is needed in TelephoneBook.cpp.
class TelephoneBookLogic;

class TelephoneBook : public wxFrame {
public:
    // Constructor: Initializes the GUI and creates the logic instance.
    TelephoneBook(const wxString& title);

    // Destructor: Cleans up resources, including the core logic object.
    ~TelephoneBook();

private:
    // GUI Widgets (remain directly in TelephoneBook as it's the main UI class)
    wxTextCtrl* nameInput;
    wxTextCtrl* phoneInput;
    wxTextCtrl* emailInput;
    wxTextCtrl* searchInput;
    wxButton* addButton;
    wxButton* searchButton;
    wxButton* sortButton;
    wxButton* deleteButton;
    wxButton* editButton;
    wxListCtrl* contactList;

    // Pointer to the core logic handler.
    // This object manages data (contacts vector) and database interactions.
    TelephoneBookLogic* coreLogic;

    // Helper method to refresh the contact list display.
    // This method interacts with the GUI (contactList) and the logic (coreLogic).
    void RefreshList();

    // Event Handlers for UI actions (remain in TelephoneBook)
    void OnAddContact(wxCommandEvent& event);
    void OnSearchContact(wxCommandEvent& event);
    void OnSortContact(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnContactSelected(wxListEvent& event);
    void OnDeleteContact(wxCommandEvent& event);
    void OnEditContact(wxCommandEvent& event);
};

#endif // TELEPHONEBOOK_HPP
