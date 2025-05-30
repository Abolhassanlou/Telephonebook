Here’s an improved and updated version of your README, tailored to match your **wxWidgets-based, SQLite-backed** telephone book application:

---

# 📞 Phone Book Management System

A modern **C++ Phone Book Management System** using **wxWidgets** for GUI integration and **SQLite** for data persistence. This application allows users to add, edit, delete, and search for contacts with a clean and efficient logic layer and support for a GUI frontend.

---

## 🚀 Features

* **Add New Contact**
  Add contacts with a name, phone number, and email address. Duplicate phone numbers are prevented.

* **Edit Existing Contact**
  Modify any contact’s name, phone number, or email with validation.

* **Delete Contact**
  Remove contacts based on name and phone number.

* **Search Contacts**
  Search by name or phone number — supports case-insensitive partial matching.

* **View All Contacts**
  List all contacts, sorted by name.

* **Persistent Storage**
  Uses **SQLite** to persist all contact information. The database is opened on initialization and saved automatically.

* **wxWidgets Integration**
  The logic layer is designed to integrate with a **wxWidgets GUI**, but can be tested headlessly using a CLI test runner.

---

## 🛠 Requirements

* **C++17** or later
* **SQLite3**
* **wxWidgets 3.x** (if using GUI)
* **CMake** (recommended for building)

---

## 📁 Project Structure

```
.
├── Contact.hpp / Contact.cpp         # Contact class definition
├── TelephoneBookLogic.hpp / .cpp    # Core logic handling all DB operations
├── test.cpp                         # Console-based test harness
├── main.cpp / GUI code (optional)   # wxWidgets app entry point
└── test_phonebook.db                # SQLite database file (auto-created)
```

---

## 🧪 Running the Test (CLI)

A minimal CLI test is included to validate functionality without a GUI:

```bash
mkdir build
cd build
cmake ..
make -j8
or
g++ -std=c++17 test.cpp TelephoneBookLogic.cpp Contact.cpp `wx-config --cxxflags --libs` -lsqlite3 -o phonebook_test
./phonebook_test
```

You’ll see log output (via `wxLog`) showing contact operations.

---

## ✅ Example Output

```
TelephoneBookLogic constructor started for DB: test_phonebook.db
Opening database at: test_phonebook.db
Contacts loaded from database. Count: 0
Adding Alice: Success
Adding Bob: Success
Search results for 'alice': 1 contact(s) found.
Editing Alice: Success
Deleting Bob: Success
```

---

## 🔧 Notes

* The application uses `wxLogMessage` for logging — output appears in the console or wx log window.
* The test app initializes `wxWidgets` via a dummy app to enable `wxString` and `wxLog`.

---

## 📌 Planned Improvements

* GUI integration using `wxFrame` and `wxListCtrl`
* Export to CSV
* Support for contact images

---

## 📃 License

## License

This project is licensed under the [MIT License](LICENSE.md) — see the LICENSE file for details.


---

to see the test

1. Use the SQLite CLI Tool (sqlite3)

If you have SQLite installed on your system, you can open a terminal and run:

sqlite3 test_phonebook.db

Once inside the SQLite prompt, you can run SQL commands like:

.tables         -- shows tables in the database
.schema contacts -- shows the schema of the contacts table (replace "contacts" with your table name)
SELECT * FROM contacts;  -- shows all data in the contacts table

Type .exit or press Ctrl+D to quit.
