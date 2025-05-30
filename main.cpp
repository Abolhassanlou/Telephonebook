#include <wx/wx.h>
#include "TelephoneBook.hpp"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxLogStderr* logger = new wxLogStderr(); // Create a logger that outputs to stderr (usually the terminal)
        wxLog::SetActiveTarget(logger);
        TelephoneBook* frame = new TelephoneBook("Telephone Book");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
