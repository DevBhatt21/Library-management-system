#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

using namespace std;

const int MAX_BOOKS = 100; // max limit of books 


//classes private and public
class Book {
private:
    string isbn, title, author, edition, publication, category, review;
    bool isBorrowed;
    float rating;
    time_t addedDate;
    time_t lastBorrowedDate;
    string borrowerName;


public:
    // Constructor
    Book() : isBorrowed(false), rating(0.0f) {
        addedDate = time(0); // store current time
        lastBorrowedDate = 0;
    }



    // Setters
    void setIsbn(const string& val) { isbn = val; }
    void setTitle(const string& val) { title = val; }
    void setAuthor(const string& val) { author = val; }
    void setEdition(const string& val) { edition = val; }
    void setPublication(const string& val) { publication = val; }
    void setCategory(const string& val) { category = val; }
    void setReview(const string& val) { review = val; }
    void setIsBorrowed(bool val) { isBorrowed = val; }
    void setRating(float val) { rating = val; }
    void setLastBorrowedDate(time_t val) { lastBorrowedDate = val; }
    void setBorrowerName(const string& name) { borrowerName = name; }




    // Getters
    string getIsbn() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getEdition() const { return edition; }
    string getPublication() const { return publication; }
    string getCategory() const { return category; }
    string getReview() const { return review; }
    bool getIsBorrowed() const { return isBorrowed; }
    float getRating() const { return rating; }
    time_t getAddedDate() const { return addedDate; }
    time_t getLastBorrowedDate() const { return lastBorrowedDate; }
    string getBorrowerName() const { return borrowerName; }




    // Converts date to readable format(to check when the book was borrowed and returned)
    string getFormattedDate(time_t date) const {
        char buffer[80];
        struct tm timeInfo;
        localtime_s(&timeInfo, &date); // safer localtime
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
        return string(buffer);
    }

    // Convert book data to string for saving to file(to store data locally)
    string toFileString() const {
        return isbn + ";" + title + ";" + author + ";" + edition + ";" + publication + ";" + category + ";" +
            (isBorrowed ? "1" : "0") + ";" + to_string(rating) + ";" + review + ";" +
            getFormattedDate(addedDate) + ";" + getFormattedDate(lastBorrowedDate) + ";" + borrowerName +"\n";
    }


    // Convert date string back to time_t
    static time_t parseDate(const string& str) {
        struct tm timeInfo = {};
        int parsedItems = sscanf_s(str.c_str(), "%d-%d-%d %d:%d:%d",
            &timeInfo.tm_year, &timeInfo.tm_mon, &timeInfo.tm_mday,
            &timeInfo.tm_hour, &timeInfo.tm_min, &timeInfo.tm_sec);

        if (parsedItems != 6) return 0;

        timeInfo.tm_year -= 1900;
        timeInfo.tm_mon -= 1;

        return mktime(&timeInfo);
    }



    // Convert line from file to Book object(to check if there is any previous data)
    static Book fromFileString(const string& line) {
        Book b;
        stringstream ss(line);
        string data;

        getline(ss, data, ';'); b.setIsbn(data);
        getline(ss, data, ';'); b.setTitle(data);
        getline(ss, data, ';'); b.setAuthor(data);
        getline(ss, data, ';'); b.setEdition(data);
        getline(ss, data, ';'); b.setPublication(data);
        getline(ss, data, ';'); b.setCategory(data);
        getline(ss, data, ';'); b.setIsBorrowed(data == "1");
        getline(ss, data, ';'); b.setRating(stof(data));
        getline(ss, data, ';'); b.setReview(data);
        getline(ss, data, ';'); b.addedDate = parseDate(data);
        getline(ss, data, ';'); b.lastBorrowedDate = parseDate(data);
        getline(ss, data, ';'); b.borrowerName = data;

        return b;
    }
};

Book books[MAX_BOOKS]; // array of books
int counter = 0; // to track number of books

// Function prototypes
void clearScreen();
void loadBooks();
void saveBooks();
void addBook();
void deleteBook();
void editBook();
void quit();

// extra features
void searchBook();
void viewAllBooks();
void sortBooks();
void borrowBook();
void returnBook();
void addReview();
void backupBooks();
void restoreBooks();
void viewRatings();

//main
int main() {
    loadBooks(); // load from file when program starts(if any available - usually not as the data is locally)
    string choice;

    while (true) {
        clearScreen();
        cout << "Hello Admin, what would you like to do today?";
        cout << "===============================\n";
        cout << "    LIBRARY MANAGEMENT SYSTEM\n";
        cout << "===============================\n\n";
        cout << "[1] ADD BOOK\n";
        cout << "[2] DELETE BOOK\n";
        cout << "[3] EDIT BOOK\n";
        cout << "[4] SEARCH BOOK\n";
        cout << "[5] VIEW ALL BOOKS\n";
        cout << "[6] SORT BOOKS\n";
        cout << "[7] BORROW BOOK\n";
        cout << "[8] RETURN BOOK\n";
        cout << "[9] ADD REVIEW TO BOOK\n";
        cout << "[10] BACKUP BOOKS\n";
        cout << "[11] RESTORE BOOKS\n";
        cout << "[12] VIEW BOOK RATINGS\n\n";
        cout << "[13] QUIT\n";
        cout << "ENTER CHOICE: ";
        getline(cin, choice);
        clearScreen();

        if (choice == "1") addBook();
        else if (choice == "2") deleteBook();
        else if (choice == "3") editBook();
        else if (choice == "4") searchBook();
        else if (choice == "5") viewAllBooks();
        else if (choice == "6") sortBooks();
        else if (choice == "7") borrowBook();
        else if (choice == "8") returnBook();
        else if (choice == "9") addReview();
        else if (choice == "10") backupBooks();
        else if (choice == "11") restoreBooks();
        else if (choice == "12") viewRatings();
        else if (choice == "13"){
            quit();
            break;
        } else {
            cout << "INVALID CHOICE! Press Enter to try again...";
            cin.get();
        }
    }

    return 0;
}

// clear screen
void clearScreen() {
    for (int i = 0; i < 30; ++i) cout << endl;
}


// Load book list from file(if any usually not as data is stored locally)
void loadBooks() {
    ifstream file("books.txt");
    string line;
    while (getline(file, line) && counter < MAX_BOOKS) {
        books[counter++] = Book::fromFileString(line);
    }
    file.close();
}


// Save book list to file(saves file locally on desktop or laptop-extra feature)
void saveBooks() {
    ofstream file("books.txt");
    for (int i = 0; i < counter; i++) {
        file << books[i].toFileString();
    }
    file.close();
}

// Add a new book to the library
void addBook() {
    if (counter >= MAX_BOOKS) {
        cout << "MAXIMUM NUMBER OF BOOKS REACHED!\nPress Enter to continue...";
        cin.get();
        return;
    }

    string isbn, title, author, edition, publication, category;
    cout << "ADD BOOK\n\n";
    cout << "Enter ISBN: "; getline(cin, isbn);
    cout << "Enter Title: "; getline(cin, title);
    cout << "Enter Author: "; getline(cin, author);
    cout << "Enter Edition: "; getline(cin, edition);
    cout << "Enter Publication: "; getline(cin, publication);
    cout << "Enter Category: "; getline(cin, category);

    Book& b = books[counter++];
    b.setIsbn(isbn);
    b.setTitle(title);
    b.setAuthor(author);
    b.setEdition(edition);
    b.setPublication(publication);
    b.setCategory(category);

    saveBooks();
    cout << "\nBOOK ADDED SUCCESSFULLY!\nPress Enter to continue...";
    cin.get();
}

// Delete a book using ISBN number of the book
void deleteBook() {
    if (counter == 0) {
        cout << "NO BOOK TO DELETE!\nPress Enter to continue...";
        cin.get();
        return;
    }

    string isbn;
    cout << "DELETE BOOK\n\nEnter ISBN: ";
    getline(cin, isbn);

    for (int i = 0; i < counter; i++) {
        if (books[i].getIsbn() == isbn) {
            cout << "\nBOOK FOUND\nDelete it?\n[1] Yes\n[2] No\nEnter choice: ";
            int confirm;
            cin >> confirm;
            cin.ignore();
            if (confirm == 1) {
                for (int j = i; j < counter - 1; j++) books[j] = books[j + 1];
                counter--;
                saveBooks();
                cout << "\nBOOK DELETED!\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }
    }

    cout << "\nBOOK NOT FOUND!\nPress Enter to continue...";
    cin.get();
}

// Edit book details in the library
void editBook() {
    string editIsbn, isbn, title, author, edition, publication, category;
    cout << "EDIT BOOK\n\n";

    if (counter == 0) {
        cout << "NO BOOK TO EDIT!\n\nPress Enter to continue...";
        cin.get();
        return;
    }

    cout << "Enter ISBN: ";
    getline(cin, editIsbn);

    for (int i = 0; i < counter; i++) {
        if (books[i].getIsbn() == editIsbn) {
            cout << "\nBOOK FOUND\n\n";
            cout << "Enter new ISBN: ";
            getline(cin, isbn);
            cout << "Enter new Title: ";
            getline(cin, title);
            cout << "Enter new Author: ";
            getline(cin, author);
            cout << "Enter new Edition: ";
            getline(cin, edition);
            cout << "Enter new Publication: ";
            getline(cin, publication);
            cout << "Enter new Category: ";
            getline(cin, category);

            books[i].setIsbn(isbn);
            books[i].setTitle(title);
            books[i].setAuthor(author);
            books[i].setEdition(edition);
            books[i].setPublication(publication);
            books[i].setCategory(category);

            saveBooks();
            cout << "\nBOOK UPDATED!\n\nPress Enter to continue...";
            cin.get();
            return;
        }
    }

    cout << "\nBOOK NOT FOUND!\n\nPress Enter to continue...";
    cin.get();
}


//searches for books in library using ISBN or Title 
void searchBook() {
    string keyword;
    cout << "SEARCH BOOK\n\nEnter ISBN or Title: ";
    getline(cin, keyword);

    bool found = false;
    for (int i = 0; i < counter; ++i) {
        if (books[i].getIsbn() == keyword || books[i].getTitle().find(keyword) != string::npos) {
            cout << "\nTitle: " << books[i].getTitle()
                 << "\nAuthor: " << books[i].getAuthor()
                 << "\nISBN: " << books[i].getIsbn()
                 << "\nRating: " << books[i].getRating()
                 << "\nBorrowed: " << (books[i].getIsBorrowed() ? "Yes" : "No") << "\n";
            found = true;
        }
    }
    if (!found) cout << "\nNO BOOK FOUND!\n";
    cout << "\nPress Enter to continue...";
    cin.get();
}

//helps to look at available books currently or helps to look at who borrowed the book
void viewAllBooks() {
    cout << "ALL BOOKS:\n\n";
    if (counter == 0) {
        cout << "NO BOOKS AVAILABLE.\n";
    } else {
        for (int i = 0; i < counter; ++i) {
            cout << i + 1 << ". " << books[i].getTitle()
                 << " [ISBN: " << books[i].getIsbn() << "] "
                 << (books[i].getIsBorrowed() ? "[BORROWED by " + books[i].getBorrowerName() + "]" : "[AVAILABLE]") << "\n";

        }
    }
    cout << "\nPress Enter to continue...";
    cin.get();
}

//helps sorts books by title!!
void sortBooks() {
    for (int i = 0; i < counter - 1; ++i) {
        for (int j = i + 1; j < counter; ++j) {
            if (books[i].getTitle() > books[j].getTitle()) {
                swap(books[i], books[j]);
            }
        }
    }
    cout << "BOOKS SORTED BY TITLE!\nPress Enter to continue...";
    cin.get();
}

//helps admin to log who has borrowed the book 
void borrowBook() {
    string isbn;
    cout << "BORROW BOOK\n\nEnter ISBN: ";
    getline(cin, isbn);

    for (int i = 0; i < counter; ++i) {
        if (books[i].getIsbn() == isbn) {
            if (books[i].getIsBorrowed()) {
                cout << "This book is already borrowed.\n";
            } else {
                books[i].setIsBorrowed(true);
books[i].setLastBorrowedDate(time(0));

string borrower;
cout << "Enter your name: ";
getline(cin, borrower);
books[i].setBorrowerName(borrower);

saveBooks();
cout << "Book borrowed successfully by " << borrower << "!\n";

            }
            cout << "Press Enter to continue...";
            cin.get();
            return;
        }
    }
    cout << "Book not found!\nPress Enter to continue...";
    cin.get();
}

//helps admin log the book back in 
void returnBook() {
    string isbn;
    cout << "RETURN BOOK\n\nEnter ISBN: ";
    getline(cin, isbn);

    for (int i = 0; i < counter; ++i) {
        if (books[i].getIsbn() == isbn) {
            if (!books[i].getIsBorrowed()) {
                cout << "This book is not currently borrowed.\n";
            } else {
                books[i].setIsBorrowed(false);
                saveBooks();
                cout << "Book returned successfully!\n";
            }
            cout << "Press Enter to continue...";
            cin.get();
            return;
        }
    }
    cout << "Book not found!\nPress Enter to continue...";
    cin.get();
}


//helps in adding a review to the book
void addReview() {
    string isbn, review;
    float rating;

    cout << "ADD REVIEW\n\nEnter ISBN: ";
    getline(cin, isbn);

    for (int i = 0; i < counter; ++i) {
        if (books[i].getIsbn() == isbn) {
            cout << "Enter your review: ";
            getline(cin, review);
            do {
                cout << "Enter rating (0.0 - 5.0): ";
                cin >> rating;
                if (rating < 0.0f || rating > 5.0f) {
                    cout << "Invalid rating! Please enter between 0.0 and 5.0.\n";
                }
            } while (rating < 0.0f || rating > 5.0f);
            cin.ignore();
            
            books[i].setReview(review);
            books[i].setRating(rating);
            saveBooks();
            cout << "Review added!\nPress Enter to continue...";
            cin.get();
            return;
        }
    }

    cout << "Book not found!\nPress Enter to continue...";
    cin.get();
}

//helps backup book locally on a desktop
void backupBooks() {
    ofstream file("books_backup.txt");
    for (int i = 0; i < counter; ++i) {
        file << books[i].toFileString();
    }
    file.close();
    cout << "Books backed up to books_backup.txt!\nPress Enter to continue...";
    cin.get();
}

//helps restore a book locally using desktop
void restoreBooks() {
    ifstream file("books_backup.txt");
    string line;
    counter = 0;

    while (getline(file, line) && counter < MAX_BOOKS) {
        books[counter++] = Book::fromFileString(line);
    }

    file.close();
    saveBooks();
    cout << "Books restored from backup!\nPress Enter to continue...";
    cin.get();
}

//helps in viewing the rating of books together
void viewRatings() {
    cout << "BOOK RATINGS:\n\n";

    if (counter == 0) {
        cout << "NO BOOKS AVAILABLE.\n";
    } else {
        for (int i = 0; i < counter; ++i) {
            cout << i + 1 << ". " << books[i].getTitle()
                 << " [ISBN: " << books[i].getIsbn() << "] - ";

            float rating = books[i].getRating();    
            if (rating > 0.0f) {
                cout << "Rating: " << rating << "/5.0\n";
            } else {
                cout << "No rating yet\n";
            }
        }
    }

    cout << "\nPress Enter to continue...";
    cin.get();
}

//helps exiting program 
void quit() {
    cout << "Goodbye! Come back to NTU libary!!!\n";
}

