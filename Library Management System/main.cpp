#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const string BOOK_FILE = "books.txt";
const string MEMBER_FILE = "members.txt";
const string ISSUE_FILE = "issued.txt";

// =====================================================
// Book Class
// =====================================================
class Book {
private:
    int bookId;
    string title;
    string author;
    bool issued;

public:
    Book() {
        bookId = 0;
        title = "";
        author = "";
        issued = false;
    }

    Book(int id, string t, string a, bool status = false) {
        bookId = id;
        title = t;
        author = a;
        issued = status;
    }

    int getBookId() const {
        return bookId;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool isIssued() const {
        return issued;
    }

    void setIssued(bool status) {
        issued = status;
    }

    void saveToFile(ofstream &file) const {
        file << bookId << '\n';
        file << title << '\n';
        file << author << '\n';
        file << issued << '\n';
    }

    bool loadFromFile(ifstream &file) {
        if (!(file >> bookId))
            return false;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(file, title);
        getline(file, author);

        file >> issued;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        return true;
    }

    void display() const {
        cout << left
             << setw(10) << bookId
             << setw(30) << title
             << setw(25) << author
             << setw(15) << (issued ? "Issued" : "Available")
             << endl;
    }
};

// =====================================================
// Member Class
// =====================================================
class Member {
private:
    int memberId;
    string name;
    string phone;

public:
    Member() {
        memberId = 0;
        name = "";
        phone = "";
    }

    Member(int id, string n, string p) {
        memberId = id;
        name = n;
        phone = p;
    }

    int getMemberId() const {
        return memberId;
    }

    string getName() const {
        return name;
    }

    void saveToFile(ofstream &file) const {
        file << memberId << '\n';
        file << name << '\n';
        file << phone << '\n';
    }

    bool loadFromFile(ifstream &file) {
        if (!(file >> memberId))
            return false;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(file, name);
        getline(file, phone);

        return true;
    }

    void display() const {
        cout << "Member ID : " << memberId << endl;
        cout << "Name      : " << name << endl;
        cout << "Phone     : " << phone << endl;
        cout << "-----------------------------\n";
    }
};

// =====================================================
// Utility Functions
// =====================================================
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool bookExists(int id) {
    ifstream file(BOOK_FILE);

    Book book;

    while (book.loadFromFile(file)) {
        if (book.getBookId() == id) {
            return true;
        }
    }

    return false;
}

bool memberExists(int id) {
    ifstream file(MEMBER_FILE);

    Member member;

    while (member.loadFromFile(file)) {
        if (member.getMemberId() == id) {
            return true;
        }
    }

    return false;
}

// =====================================================
// Add Book
// =====================================================
void addBook() {

    int id;
    string title;
    string author;

    cout << "\n====================================\n";
    cout << "             ADD BOOK\n";
    cout << "====================================\n";

    cout << "Enter Book ID: ";

    while (!(cin >> id)) {
        cout << "Invalid input. Enter Book ID: ";
        clearInput();
    }

    if (bookExists(id)) {
        cout << "\nBook ID already exists!\n";
        return;
    }

    cin.ignore();

    cout << "Enter Book Title: ";
    getline(cin, title);

    cout << "Enter Author Name: ";
    getline(cin, author);

    Book book(id, title, author);

    ofstream file(BOOK_FILE, ios::app);

    if (!file) {
        cout << "Error opening book file!\n";
        return;
    }

    book.saveToFile(file);

    file.close();

    cout << "\nBook added successfully!\n";
}

// =====================================================
// Display Books
// =====================================================
void displayBooks() {

    ifstream file(BOOK_FILE);

    if (!file) {
        cout << "\nNo books found!\n";
        return;
    }

    Book book;
    bool found = false;

    cout << "\n============================================================\n";
    cout << "                    ALL BOOKS\n";
    cout << "============================================================\n";

    cout << left
         << setw(10) << "Book ID"
         << setw(30) << "Title"
         << setw(25) << "Author"
         << setw(15) << "Status"
         << endl;

    cout << string(80, '-') << endl;

    while (book.loadFromFile(file)) {
        book.display();
        found = true;
    }

    if (!found) {
        cout << "No books available.\n";
    }

    file.close();
}

// =====================================================
// Search Book
// =====================================================
void searchBook() {

    int choice;
    string keyword;

    cout << "\n====================================\n";
    cout << "             SEARCH BOOK\n";
    cout << "====================================\n";
    cout << "1. Search by Title\n";
    cout << "2. Search by Author\n";
    cout << "Enter choice: ";

    while (!(cin >> choice)) {
        cout << "Invalid choice: ";
        clearInput();
    }

    cin.ignore();

    cout << "Enter search keyword: ";
    getline(cin, keyword);

    ifstream file(BOOK_FILE);

    if (!file) {
        cout << "\nNo books found!\n";
        return;
    }

    Book book;
    bool found = false;

    while (book.loadFromFile(file)) {

        bool match = false;

        if (choice == 1) {
            if (book.getTitle().find(keyword) != string::npos) {
                match = true;
            }
        }
        else if (choice == 2) {
            if (book.getAuthor().find(keyword) != string::npos) {
                match = true;
            }
        }
        else {
            cout << "\nInvalid choice!\n";
            return;
        }

        if (match) {

            if (!found) {
                cout << "\nSearch Results\n";
                cout << string(80, '-') << endl;

                cout << left
                     << setw(10) << "Book ID"
                     << setw(30) << "Title"
                     << setw(25) << "Author"
                     << setw(15) << "Status"
                     << endl;

                cout << string(80, '-') << endl;
            }

            book.display();
            found = true;
        }
    }

    file.close();

    if (!found) {
        cout << "\nNo matching book found.\n";
    }
}

// =====================================================
// Add Member
// =====================================================
void addMember() {

    int id;
    string name;
    string phone;

    cout << "\n====================================\n";
    cout << "            ADD MEMBER\n";
    cout << "====================================\n";

    cout << "Enter Member ID: ";

    while (!(cin >> id)) {
        cout << "Invalid input. Enter Member ID: ";
        clearInput();
    }

    if (memberExists(id)) {
        cout << "\nMember ID already exists!\n";
        return;
    }

    cin.ignore();

    cout << "Enter Member Name: ";
    getline(cin, name);

    cout << "Enter Phone Number: ";
    getline(cin, phone);

    Member member(id, name, phone);

    ofstream file(MEMBER_FILE, ios::app);

    if (!file) {
        cout << "Error opening member file!\n";
        return;
    }

    member.saveToFile(file);

    file.close();

    cout << "\nMember added successfully!\n";
}

// =====================================================
// Display Members
// =====================================================
void displayMembers() {

    ifstream file(MEMBER_FILE);

    if (!file) {
        cout << "\nNo members found!\n";
        return;
    }

    Member member;
    bool found = false;

    cout << "\n====================================\n";
    cout << "           ALL MEMBERS\n";
    cout << "====================================\n";

    while (member.loadFromFile(file)) {
        member.display();
        found = true;
    }

    if (!found) {
        cout << "No members available.\n";
    }

    file.close();
}

// =====================================================
// Issue Book
// =====================================================
void issueBook() {

    int bookId;
    int memberId;

    cout << "\n====================================\n";
    cout << "             ISSUE BOOK\n";
    cout << "====================================\n";

    cout << "Enter Book ID: ";

    while (!(cin >> bookId)) {
        cout << "Invalid Book ID: ";
        clearInput();
    }

    ifstream file(BOOK_FILE);

    if (!file) {
        cout << "\nNo books found!\n";
        return;
    }

    Book book;
    bool found = false;

    while (book.loadFromFile(file)) {

        if (book.getBookId() == bookId) {

            found = true;

            if (book.isIssued()) {
                cout << "\nBook is already issued!\n";
                file.close();
                return;
            }

            break;
        }
    }

    file.close();

    if (!found) {
        cout << "\nBook not found!\n";
        return;
    }

    cout << "Enter Member ID: ";

    while (!(cin >> memberId)) {
        cout << "Invalid Member ID: ";
        clearInput();
    }

    if (!memberExists(memberId)) {
        cout << "\nMember not found!\n";
        return;
    }

    // Update book status
    ifstream inputFile(BOOK_FILE);
    ofstream tempFile("temp_books.txt");

    Book currentBook;

    while (currentBook.loadFromFile(inputFile)) {

        if (currentBook.getBookId() == bookId) {
            currentBook.setIssued(true);
        }

        currentBook.saveToFile(tempFile);
    }

    inputFile.close();
    tempFile.close();

    remove(BOOK_FILE.c_str());
    rename("temp_books.txt", BOOK_FILE.c_str());

    // Save issue record
    ofstream issueFile(ISSUE_FILE, ios::app);

    issueFile << bookId << '\n';
    issueFile << memberId << '\n';

    issueFile.close();

    cout << "\nBook issued successfully!\n";
}

// =====================================================
// Return Book
// =====================================================
void returnBook() {

    int bookId;

    cout << "\n====================================\n";
    cout << "            RETURN BOOK\n";
    cout << "====================================\n";

    cout << "Enter Book ID: ";

    while (!(cin >> bookId)) {
        cout << "Invalid Book ID: ";
        clearInput();
    }

    ifstream file(BOOK_FILE);

    if (!file) {
        cout << "\nNo books found!\n";
        return;
    }

    Book book;
    bool found = false;

    while (book.loadFromFile(file)) {

        if (book.getBookId() == bookId) {

            found = true;

            if (!book.isIssued()) {
                cout << "\nThis book is not currently issued.\n";
                file.close();
                return;
            }

            break;
        }
    }

    file.close();

    if (!found) {
        cout << "\nBook not found!\n";
        return;
    }

    // Update book status
    ifstream inputFile(BOOK_FILE);
    ofstream tempFile("temp_books.txt");

    Book currentBook;

    while (currentBook.loadFromFile(inputFile)) {

        if (currentBook.getBookId() == bookId) {
            currentBook.setIssued(false);
        }

        currentBook.saveToFile(tempFile);
    }

    inputFile.close();
    tempFile.close();

    remove(BOOK_FILE.c_str());
    rename("temp_books.txt", BOOK_FILE.c_str());

    // Remove issue record
    ifstream issueInput(ISSUE_FILE);
    ofstream issueTemp("temp_issued.txt");

    int issuedBookId;
    int memberId;

    while (issueInput >> issuedBookId >> memberId) {

        if (issuedBookId != bookId) {
            issueTemp << issuedBookId << '\n';
            issueTemp << memberId << '\n';
        }
    }

    issueInput.close();
    issueTemp.close();

    remove(ISSUE_FILE.c_str());
    rename("temp_issued.txt", ISSUE_FILE.c_str());

    cout << "\nBook returned successfully!\n";
}

// =====================================================
// Main Menu
// =====================================================
int main() {

    int choice;

    do {

        cout << "\n\n";
        cout << "============================================\n";
        cout << "          LIBRARY MANAGEMENT SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Add Member\n";
        cout << "5. Display Members\n";
        cout << "6. Issue Book\n";
        cout << "7. Return Book\n";
        cout << "8. Exit\n";
        cout << "============================================\n";

        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid choice. Enter a number: ";
            clearInput();
        }

        switch (choice) {

            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                addMember();
                break;

            case 5:
                displayMembers();
                break;

            case 6:
                issueBook();
                break;

            case 7:
                returnBook();
                break;

            case 8:
                cout << "\nThank you for using Library Management System!\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}