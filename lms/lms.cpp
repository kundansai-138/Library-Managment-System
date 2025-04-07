#include <bits/stdc++.h>
using namespace std;

//inter conversion between string and time

time_t stringToTime(string& timeStr) {
    tm tm = {};
    istringstream ss(timeStr);
    ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return mktime(&tm);
}

string timeToString(time_t time) {
    ostringstream oss;
    oss << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}


class Library;

//User Class

class User {
protected:
    int ID;
    string username;
    string password;
    static int next_user_ID;
public:
    // Constructor for loading
    User(int id, const string& uname, const string& pass) 
        : ID(id), username(uname), password(pass) {
        if(id >= next_user_ID)
            next_user_ID = id + 1;
    }
    // Constructor for user creation 
    User(const string& uname, const string& pass) 
        : ID(next_user_ID++), username(uname), password(pass) {}
    //getter function
    int get_ID() const { return ID; }
    string get_username() const { return username; }
    string get_password() const { return password; }
    //setter function
    void set_username(const string& new_username){ username=new_username;}
    void set_password(const string& new_password){ password = new_password;}
    //load and store for next_user_ID for unique id generation for all users
    static void load_next_user_ID() {
        ifstream infile("user_next_ID.txt");
        if(infile) infile >> next_user_ID;
        infile.close();
    }
    static void save_next_user_ID() {
        ofstream outfile("user_next_ID.txt");
        outfile << next_user_ID;
        outfile.close();
    }
};

// Book Class

class Book {
private:
    int ID;
    bool Available;
    string Name;
    string Author;
    string Publisher;
    string ISBN;
    static int next_book_ID;
    int reserver_id;
public:
    // Constructor for new books
    Book(const string& name, const string& author, const string& publisher, const string& isbn)
        : ID(next_book_ID++), Available(true), Name(name), Author(author), Publisher(publisher), ISBN(isbn) , reserver_id(0) {}
    // Constructor for loading
    Book(int id, const string& name, const string& author, const string& publisher, const string& isbn, bool available, int reserver)
        : ID(id), Available(available), Name(name), Author(author), Publisher(publisher), ISBN(isbn), reserver_id(reserver)  {
            if(id >= next_book_ID)
                next_book_ID = id + 1;
        }
    // getter function
    string get_name() const { return Name; }
    string get_author() const { return Author; }
    string get_publisher() const { return Publisher; }
    string get_ISBN() const { return ISBN; }
    bool get_availability() const { return Available; }
    int get_ID() const { return ID; }
    int get_reserver_id() const { return reserver_id; }
    // setter function
    void set_availability(bool availability) { Available = availability;}; 
    void set_name(const string& name){Name=name;}
    void set_author(const string& author){Author=author;}
    void set_publisher(const string& publisher){Publisher=publisher;}
    void set_ISBN(const string& isbn){ISBN=isbn;}
    void set_reserver_id(int x) {reserver_id = x;}
    // load and store function for load_next_book_ID for unique id generation
    static void load_next_book_ID() {
        ifstream infile("book_next_ID.txt");
        if(infile) infile >> next_book_ID;
        infile.close();
    }
    static void save_next_book_ID() {
        ofstream outfile("book_next_ID.txt");
        outfile << next_book_ID;
        outfile.close();
    }
};

//borrow record Class (borrowed book)

class borrow_record {
public:
    Book* book;
    string time_stamp;
    //constructor
    borrow_record(Book* b, string time) : book(b), time_stamp(time) {}
};

//Account Class

class Account {
private:
    vector<borrow_record> record;
    int fine;
    vector<Book*> history;
public:
    //conmstructor
    Account() : fine(0) {}
    //getter
    vector<borrow_record>& get_record() { return record; }
    vector<Book*>& get_history() { return history; }
    //adding book into account
    void borrow_book(Book* book) {
        time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timeStr = timeToString(now);
        record.push_back(borrow_record(book, timeStr));
        history.push_back(book);
    }
    //load from file
    void load_borrow_record(Book* book, const string& timestamp) {
        record.push_back(borrow_record(book, timestamp));
    }
    void load_history_record(Book* book) {
        history.push_back(book);
    }
    //fine caluclation for students
    int calculate_fine() {
        time_t current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        int new_fine = fine;
        for (auto& rec : record) {
            string ts = rec.time_stamp;
            time_t borrowed_time = stringToTime(ts);
            double days = difftime(current_time, borrowed_time) / (60*60*24);
            if (days > 15)
                new_fine += static_cast<int>(10 *  (int) (days - 15));
        }
        return new_fine;
    }
    //fine payment for students
    void pay_fine() {
        fine = 0;
        time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        string timeStr = timeToString(now);
        for (auto& rec : record)
            rec.time_stamp = timeStr;
    }
    //faculty will not be imposed by fine but will get into overdue if u borrow a book for more the 60 days
    bool faculty_overdue() {
         time_t current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        for (auto& rec : record) {
            string ts = rec.time_stamp;
            time_t borrowed_time = stringToTime(ts);
            double days = difftime(current_time, borrowed_time) / (60*60*24);
            if (days > 60){
                return true;
            }
        }
        return false;
    }
};

//Student Class 

class Student : public User {
private:
    int borrowed_books;
public:
    Account my_account;
    //constructor for loading
    Student(int id, const string& uname, const string& pass) 
        : User(id, uname, pass), borrowed_books(0) {}
    //constructor    
    Student(const string& uname, const string& pass)
        : User(uname, pass), borrowed_books(0) {}

    int get_borrowed_books() { return borrowed_books; }
    void increment_borrowed() { borrowed_books++; }
    void decrement_borrowed() { if(borrowed_books > 0) borrowed_books--; }

    void book_borrow(int book_id, Library* lib);
    void book_return(int book_id);
    void reserve_a_book(int book_id,Library* lib);
};

class Faculty : public User {
private:
    int borrowed_books;
public:
    Account my_account;
    //constructor for loading
    Faculty(int id, const string& uname, const string& pass)
        : User(id, uname, pass), borrowed_books(0) {}
    //comnstructor    
    Faculty(const string& uname, const string& pass)
        : User(uname, pass), borrowed_books(0) {}

    int get_borrowed_books() { return borrowed_books; }
    void increment_borrowed() { borrowed_books++; }
    void decrement_borrowed() { if(borrowed_books > 0) borrowed_books--; }

    void book_borrow(int book_id, Library* lib);
    void book_return(int book_id);
    void reserve_a_book(int book_id,Library* lib);
};

class Librarian : public User {
public:
    //constructor for loading
    Librarian(int id, const string& uname, const string& pass)
        : User(id, uname, pass) {}
    //constructor    
    Librarian(const string& uname, const string& pass)
        : User(uname, pass) {}
};

//Library Class

class Library {
private:
    vector<Student*> student_users;
    vector<Faculty*> faculty_users;
    vector<Librarian*> librarian_users;
    vector<Book*> available_books;
public:
    // Add functions
    void add_librarian(Librarian* lib) { librarian_users.push_back(lib); }
    void add_student(Student* student) { student_users.push_back(student); }
    void add_faculty(Faculty* faculty) { faculty_users.push_back(faculty); }
    void add_book(Book* book) { available_books.push_back(book); }
    // empty
    bool BookEmpty() const {
        return available_books.empty();
    }
    bool StudentEmpty() const {
        return student_users.empty();
    } 
    bool FacultyEmpty() const {
        return faculty_users.empty();
    } 
    bool LibrarianEmpty() const {
        return librarian_users.empty();
    } 
    // Getter functions
    Book* get_book(int id) {
        for (Book* book : available_books)
            if (book->get_ID() == id)
                return book;
        return nullptr;
    }
    Student* get_student(int id) {
        for (Student* stu : student_users)
            if (stu->get_ID() == id)
                return stu;
        return nullptr;
    }
    Faculty* get_faculty(int id) {
        for (Faculty* fac : faculty_users)
            if (fac->get_ID() == id)
                return fac;
        return nullptr;
    }
    Librarian* get_librarian(int id) {
        for (Librarian* lib : librarian_users)
            if (lib->get_ID() == id)
                return lib;
        return nullptr;
    }
    // Remove 
    void remove_book_from_library(int id) {
        for (auto it = available_books.begin(); it != available_books.end(); ++it) {
            if ((*it)->get_ID() == id && (*it)->get_availability()) {
                available_books.erase(it);
                cout << "Successfully removed book ID " << id << endl;
                return;
            }
        }
        cout << "Book not found or currently borrowed" << endl;
    }
    void remove_student_user(int id) {
        for (auto it = student_users.begin(); it != student_users.end(); ++it) {
            if ((*it)->get_ID() == id) {
                student_users.erase(it);
                cout << "Successfully removed Student with ID " << id << endl;
                return;
            }
        }
        cout << "No student with ID: " << id << endl;
    }
    void remove_faculty_user(int id) {
        for (auto it = faculty_users.begin(); it != faculty_users.end(); ++it) {
            if ((*it)->get_ID() == id) {
                faculty_users.erase(it);
                cout << "Successfully removed Faculty with ID " << id << endl;
                return;
            }
        }
        cout << "No Faculty with ID: " << id << endl;
    }
    void remove_librarian(int id) {
        for (auto it = librarian_users.begin(); it != librarian_users.end(); ++it) {
            if ((*it)->get_ID() == id) {
                librarian_users.erase(it);
                cout << "Successfully removed Librarian with ID " << id << endl;
                return;
            }
        }
        cout << "No Librarian with ID: " << id << endl;
    }
    // Verification
    bool verify_student(int id, string p) {
        for (auto stu : student_users)
            if (stu->get_ID() == id && stu->get_password() == p)
                return true;
        cout << "Invalid Credentials" << endl;
        return false;
    }
    bool verify_faculty(int id, string p) {
        for (auto fac : faculty_users)
            if (fac->get_ID() == id && fac->get_password() == p)
                return true;
        cout << "Invalid Credentials" << endl;
        return false;
    }
    bool verify_librarian(int id, string p) {
        for (auto lib : librarian_users)
            if (lib->get_ID() == id && lib->get_password() == p)
                return true;
        cout << "Invalid Credentials" << endl;
        return false;
    }

//book

    //format: ID|Name|Author|Publisher|ISBN|Available|reserver_id
    void loadBooks() {
        ifstream file("books.txt");
        if (!file) {
            cout << "No books data found. Starting with an empty library." << endl;
            return;
        }
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 7) {
                int id = stoi(tokens[0]);
                string name = tokens[1];
                string author = tokens[2];
                string publisher = tokens[3];
                string isbn = tokens[4];
                bool available = (tokens[5] == "1");
                int reserver = stoi(tokens[6]);
                Book* book = new Book(id, name, author, publisher, isbn, available,reserver);
                available_books.push_back(book);
            }
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        for (Book* book : available_books) {
            file << book->get_ID() << "|" << book->get_name() << "|" 
                 << book->get_author() << "|" << book->get_publisher() << "|" 
                 << book->get_ISBN() << "|" << (book->get_availability() ? "1" : "0") << "|" << book->get_reserver_id() << "\n";
        }
        file.close();
    }

//student 

    // format: ID|Username|Password|BorrowedBooksCount
    void loadStudents() {
        ifstream file("students.txt");
        if (!file) {
            cout << "No student data found." << endl;
            return;
        }
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 4) {
                int id = stoi(tokens[0]);
                string uname = tokens[1];
                string pass = tokens[2];
                Student* stu = new Student(id, uname, pass);
                student_users.push_back(stu);
            }
        }
        file.close();
    }

    void saveStudents() {
        ofstream file("students.txt");
        for (Student* stu : student_users) {
            file << stu->get_ID() << "|" << stu->get_username() << "|" 
                 << stu->get_password() << "|" << stu->get_borrowed_books() << "\n";
        }
        file.close();
    }

    //format: StudentID|BookID|TimeStamp
    void loadStudentRecords() {
        ifstream file("student_records.txt");
        if (!file) return;
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 3) {
                int studentID = stoi(tokens[0]);
                int bookID = stoi(tokens[1]);
                string timestamp = tokens[2];
                Student* stu = get_student(studentID);
                Book* book = get_book(bookID);
                if(stu && book) {
                    stu->my_account.load_borrow_record(book, timestamp);
                    stu->increment_borrowed();
                }
            }
        }
        file.close();
    }

    void saveStudentRecords() {
        ofstream file("student_records.txt");
        for (Student* stu : student_users) {
            for (auto& rec : stu->my_account.get_record())
                file << stu->get_ID() << "|" << rec.book->get_ID() << "|" << rec.time_stamp << "\n";
        }
        file.close();
    }

    //format: StudentID|BookID
    void loadStudentHistory() {
        ifstream file("student_history.txt");
        if (!file) return;
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 2) {
                int studentID = stoi(tokens[0]);
                int bookID = stoi(tokens[1]);
                Student* stu = get_student(studentID);
                Book* book = get_book(bookID);
                if(stu && book)
                    stu->my_account.load_history_record(book);
            }
        }
        file.close();
    }

    void saveStudentHistory() {
        ofstream file("student_history.txt");
        for (Student* stu : student_users) {
            for (Book* book : stu->my_account.get_history())
                file << stu->get_ID() << "|" << book->get_ID() << "\n";
        }
        file.close();
    }

// faculty   

    //format: ID|Username|Password|BorrowedBooksCount
    void loadFaculty() {
        ifstream file("faculty.txt");
        if (!file) {
            cout << "No faculty data found." << endl;
            return;
        }
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 4) {
                int id = stoi(tokens[0]);
                string uname = tokens[1];
                string pass = tokens[2];
                Faculty* fac = new Faculty(id, uname, pass);
                faculty_users.push_back(fac);
            }
        }
        file.close();
    }

    void saveFaculty() {
        ofstream file("faculty.txt");
        for (Faculty* fac : faculty_users) {
            file << fac->get_ID() << "|" << fac->get_username() << "|" 
                 << fac->get_password() << "|" << fac->get_borrowed_books() << "\n";
        }
        file.close();
    }

    //format: FacultyID|BookID|TimeStamp
    void loadFacultyRecords() {
        ifstream file("faculty_records.txt");
        if (!file) return;
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 3) {
                int facultyID = stoi(tokens[0]);
                int bookID = stoi(tokens[1]);
                string timestamp = tokens[2];
                Faculty* fac = get_faculty(facultyID);
                Book* book = get_book(bookID);
                if(fac && book) {
                    fac->my_account.load_borrow_record(book, timestamp);
                    fac->increment_borrowed();
                }
            }
        }
        file.close();
    }

    void saveFacultyRecords() {
        ofstream file("faculty_records.txt");
        for (Faculty* fac : faculty_users) {
            for (auto& rec : fac->my_account.get_record())
                file << fac->get_ID() << "|" << rec.book->get_ID() << "|" << rec.time_stamp << "\n";
        }
        file.close();
    }

    //format: FacultyID|BookID
    void loadFacultyHistory() {
        ifstream file("faculty_history.txt");
        if (!file) return;
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 2) {
                int facultyID = stoi(tokens[0]);
                int bookID = stoi(tokens[1]);
                Faculty* fac = get_faculty(facultyID);
                Book* book = get_book(bookID);
                if(fac && book)
                    fac->my_account.load_history_record(book);
            }
        }
        file.close();
    }

    void saveFacultyHistory() {
        ofstream file("faculty_history.txt");
        for (Faculty* fac : faculty_users) {
            for (Book* book : fac->my_account.get_history())
                file << fac->get_ID() << "|" << book->get_ID() << "\n";
        }
        file.close();
    }

//librarian

    //format: ID|Username|Password
    void loadLibrarians() {
        ifstream file("librarians.txt");
        if (!file) {
            cout << "No librarian data found." << endl;
            return;
        }
        string line;
        while(getline(file, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while(getline(ss, token, '|'))
                tokens.push_back(token);
            if(tokens.size() == 3) {
                int id = stoi(tokens[0]);
                string uname = tokens[1];
                string pass = tokens[2];
                Librarian* lib = new Librarian(id, uname, pass);
                librarian_users.push_back(lib);
            }
        }
        file.close();
    }

    void saveLibrarians() {
        ofstream file("librarians.txt");
        for (Librarian* lib : librarian_users)
            file << lib->get_ID() << "|" << lib->get_username() << "|" << lib->get_password() << "\n";
        file.close();
    }

    //load and save functions
    void load_library_data() {
        Book::load_next_book_ID();
        User::load_next_user_ID();
        loadBooks();
        loadStudents();
        loadStudentRecords();
        loadStudentHistory();
        loadFaculty();
        loadFacultyRecords();
        loadFacultyHistory();
        loadLibrarians();
    }

    void save_library_data() {
        saveBooks();
        saveStudents();
        saveStudentRecords();
        saveStudentHistory();
        saveFaculty();
        saveFacultyRecords();
        saveFacultyHistory();
        saveLibrarians();
        Book::save_next_book_ID();
        User::save_next_user_ID();
    }

//MENU

//student menu

    void student_menu(Student* student) {
        while (true) {
             cout<<"\nWELCOME STUDENT: "<<student->get_username()<<endl;
        cout<<endl;
            cout<<"\nSTUDENT MENU"<<endl;
            cout<<"1. View Books"<<endl;
            cout<<"2. Borrow a book"<<endl;
            cout<<"3. Return a book"<<endl;
            cout<<"4. Reserve a book"<<endl;
            cout<<"5. Check fines"<<endl;
            cout<<"6. pay fines"<<endl;
            cout<<"7. View borrowing history"<<endl;
            cout<<"8. Edit details"<<endl;
            cout<<"9. Logout"<<endl;
            int choice;
            cout<<"Enter your choice: ";
            cin>>choice;
            if(choice ==1){
    cout << "\nBOOK MENU\n";
    cout << "------------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "ID" 
         << setw(25) << "Name" 
         << setw(20) << "Author" 
         << setw(20) << "Publisher" 
         << setw(18) << "ISBN" 
         << setw(10) << "Available" << endl;
    cout << "------------------------------------------------------------------------------------------------------------\n";
    for (Book* book : available_books) {
        if (book) {
            cout << left << setw(10) << book->get_ID() 
                 << setw(25) << book->get_name().substr(0, 24)  // Limit length for alignment
                 << setw(20) << book->get_author().substr(0, 19) 
                 << setw(20) << book->get_publisher().substr(0, 19) 
                 << setw(18) << book->get_ISBN() 
                 << setw(10) << (book->get_availability() ? "Yes" : "No") << endl;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------\n";
            }
            else if(choice == 2){
                int book_id;
                cout << "Enter book ID to borrow: ";
                cin >> book_id;
                student->book_borrow(book_id, this);
            } 
            else if(choice == 3){
                int book_id;
                cout << "Enter book ID to return: ";
                cin >> book_id;
                student->book_return(book_id);
            }
            else if(choice == 4){
                int book_id;
                cout<<"Enter book ID to reserve: ";
                cin>>book_id;
                student->reserve_a_book(book_id,this);
            }
            else if(choice == 5){
                int fine = student->my_account.calculate_fine();
                cout << "Your current fine is: " << fine <<" Rupees"<<endl;
            }
            else if(choice == 6){
                int fine = student->my_account.calculate_fine();
                (student->my_account).pay_fine();
                cout << "Paid Fine of "<<fine<<" Rupees"<<endl;
            }
            else if(choice == 7){
                cout << "Borrowing history:" << endl;
                for (Book* book : student->my_account.get_history()) {
                    cout << "Book: " << book->get_name() << " (ID: " << book->get_ID() << ")" << endl;
                }
            }
            else if(choice == 8){
                cout<<"Edit Details"<<endl;
                cout<<"1. username"<<endl;
                cout<<"2. password"<<endl;
                int choice1;
                cout<<"Enter your choice : "<<endl;
                cin>>choice1;
                if(choice1==1){
                string new_username;
                cout<<"Enter new username:";
                cin>>new_username;
                string old_username=student->get_username();
                student->set_username(new_username);
                cout<<"student user name has been changed from "<<old_username<<" to "<<new_username<<endl; 
                }
                else if(choice1==2){
                     string new_password,check_new_password;
                cout<<"Enter new password:";
                cin>>new_password;
                cout<<"Re enter new password:";
                cin>>check_new_password;
                if(new_password==check_new_password){
                    student->set_password(new_password);
                }
                else{
                    cout<<"Both entered passwords weren't same. Try again"<<endl;
                }
                }
                else{
                    cout<<"Invalid choice,try again"<<endl;
                }
            }
            else if (choice == 9) {
                cout << "Logging out..." << endl;
                save_library_data(); // Save before logout
                break;
            } 
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

//faculty menu

    void faculty_menu(Faculty* faculty) {
        while (true) {
        cout<<"\nWELCOME FACULTY: "<<faculty->get_username()<<endl;
        cout<<endl;
            cout<<"\nFACULTY MENU"<<endl;
            cout<<"1. View Books"<<endl;
            cout<<"2. Borrow a book"<<endl;
            cout<<"3. Return a book"<<endl;
            cout<<"4. Reserve a book"<<endl;
            cout<<"5. Check Overdue"<<endl;
            cout<<"6. View borrowing history"<<endl;
            cout<<"7. Edit Details"<<endl;
            cout<<"8. Logout"<<endl;
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;
            if(choice == 1){          
        cout << "------------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "ID" 
         << setw(25) << "Name" 
         << setw(20) << "Author" 
         << setw(20) << "Publisher" 
         << setw(18) << "ISBN" 
         << setw(10) << "Available" << endl;
    cout << "------------------------------------------------------------------------------------------------------------\n";
    for (Book* book : available_books) {
        if (book) {
            cout << left << setw(10) << book->get_ID() 
                 << setw(25) << book->get_name().substr(0, 24)  // Limit length for alignment
                 << setw(20) << book->get_author().substr(0, 19) 
                 << setw(20) << book->get_publisher().substr(0, 19) 
                 << setw(18) << book->get_ISBN() 
                 << setw(10) << (book->get_availability() ? "Yes" : "No") << endl;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------\n";
            }
            else if(choice == 2) {
                int book_id;
                cout<<"Enter book ID to borrow: ";
                cin>>book_id;
                faculty->book_borrow(book_id, this);
            }
            else if (choice == 3) {
                int book_id;
                cout<<"Enter book ID to return: ";
                cin>>book_id;
                faculty->book_return(book_id);
            }
            else if (choice == 4) {
                int book_id;
                cout<<"Enter book ID to reserve: ";
                cin>>book_id;
                faculty->reserve_a_book(book_id,this);
            }
            else if (choice == 5) {
                bool fine = faculty->my_account.faculty_overdue();
                //60 days 
                if(fine){
                    cout<<"overdue"<<endl;
                }
                else{
                    cout<<"NO overdue"<<endl;
                }                
            } 
            else if (choice == 6) {
                cout << "Borrowing history:" << endl;
                for (Book* book : faculty->my_account.get_history()) {
                    cout << "Book: " << book->get_name() << " (ID: " << book->get_ID() << ")" << endl;
                }
            } 
            else if (choice == 7) {
                cout<<"Edit Details"<<endl;
                cout<<"1. username"<<endl;
                cout<<"2. password"<<endl;
                int choice1;
                cout<<"Enter your choice : "<<endl;
                cin>>choice1;
                if(choice1==1){
                string new_username;
                cout<<"Enter new username:";
                cin>>new_username;
                string old_username=faculty->get_username();
                faculty->set_username(new_username);
                cout<<"student user name has been changed from "<<old_username<<" to "<<new_username<<endl; 
                }
                else if(choice1==2){
                     string new_password,check_new_password;
                cout<<"Enter new password:";
                cin>>new_password;
                cout<<"Re enter new password:";
                cin>>check_new_password;
                if(new_password==check_new_password){
                    faculty->set_password(new_password);
                }
                else{
                    cout<<"Both entered passwords weren't same. Try again"<<endl;
                }
                }
                else{
                    cout<<"Invalid choice,try again"<<endl;
                }
            }
            else if (choice == 8) {
                cout << "Logging out..." << endl;
                break;
            }  
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void librarian_menu(Librarian* librarian) {
        while (true) {
             cout<<"\nWELCOME FACULTY: "<<librarian->get_username()<<endl;
        cout<<endl;
            cout<<"\nLIBRARIAN MENU"<<endl;
            cout<<"1. View Books"<<endl;
            cout<<"2. Add a book"<<endl;
            cout<<"3. Remove a book"<<endl;
            cout<<"4. Add User"<<endl;
            cout<<"5. Remove User"<<endl;
            cout<<"6. View Current Borrowed Books"<<endl;
            cout<<"7. View History"<<endl;
            cout<<"8. Logout" << endl;
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;
            if(choice ==1) {
             cout << "------------------------------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "ID" 
         << setw(25) << "Name" 
         << setw(20) << "Author" 
         << setw(20) << "Publisher" 
         << setw(18) << "ISBN" 
         << setw(10) << "Available" << endl;
    cout << "------------------------------------------------------------------------------------------------------------\n";
    for (Book* book : available_books) {
        if (book) {
            cout << left << setw(10) << book->get_ID() 
                 << setw(25) << book->get_name().substr(0, 24)  // Limit length for alignment
                 << setw(20) << book->get_author().substr(0, 19) 
                 << setw(20) << book->get_publisher().substr(0, 19) 
                 << setw(18) << book->get_ISBN() 
                 << setw(10) << (book->get_availability() ? "Yes" : "No") << endl;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------\n";
            }
            else if (choice == 2) {
                string name, author, publisher, isbn;
                cout << "Enter book name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter publisher: ";
                getline(cin, publisher);
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                Book* book = new Book(name, author, publisher, isbn);
                add_book(book);
                cout << "Book added successfully!" << endl;
            } 
            else if (choice == 3) {
                int book_id;
                cout << "Enter book ID to remove: ";
                cin >> book_id;
                remove_book_from_library(book_id);
            } 
            else if (choice == 4) {
                cout<<endl;
                cout<<"User Type"<<endl;
                cout<<"1. Student"<<endl;
                cout<<"2. Faculty"<<endl;
                cout<<"3. Librarian"<<endl;
                int add_user_choice;
                cout << "Enter your choice: ";
                cin>>add_user_choice;
              if(add_user_choice==1){
                  string username,password;
                  cout << "Enter username: ";
                  cin>>username;
                  cout << "Enter password: ";
                  cin>>password;
                  Student* new_student = new Student(username,password);
                  student_users.push_back(new_student);
                  cout<<"new student user of ID: "<< new_student->get_ID()<< " has been created."<<endl;
                 }
               else if(add_user_choice==2){
                  string username,password;
                 cout << "Enter username: ";
                 cin>>username;
                 cout << "Enter password: ";
                 cin>>password;
                 Faculty* new_faculty = new Faculty(username,password);
                 faculty_users.push_back(new_faculty);
                 cout<<"new faculty user of ID: "<< new_faculty->get_ID()<< " has been created."<<endl;
               }  
              else if(add_user_choice==3){
                string username,password;
                 cout << "Enter username: ";
                 cin>>username;
                 cout << "Enter password: ";
                 cin>>password;
                 Librarian* new_librarian = new Librarian(username,password);
                 librarian_users.push_back(new_librarian);
                 cout<<"new librarian user of ID: "<< new_librarian->get_ID()<< " has been created."<<endl;
              }
              else{
                 cout<<"Invalid Choice try again"<<endl;
              } 
            } 
            else if (choice == 5) {
                cout<<endl;
                cout<<"User Type"<<endl;
                cout<<"1. Student"<<endl;
                cout<<"2. Faculty"<<endl;
                cout<<"3. Librarian"<<endl;
                int remove_user_choice;
                cout << "Enter your choice: ";
                cin>>remove_user_choice;
                if(remove_user_choice==1){
                int student_id;
                cout << "Enter student ID to remove: ";
                cin >> student_id;
                Student* removal_student = get_student(student_id);
                if(removal_student){
                if(removal_student->get_borrowed_books()!=0){
                    cout<<"Student of ID:"<<student_id<<" has borrowed books cannot be removed"<<endl;
                }
                else{
                    if((removal_student->my_account).calculate_fine()!=0){
                        cout<<"Student of ID:"<<student_id<<" has fine cannot be removed"<<endl;
                    }
                    else{
                        remove_student_user(student_id);
                        cout<<"Student of ID:"<<student_id<<" is no more an user of this library";
                    }
                }
                }
                else{
                    cout<<"Student of ID:"<<student_id<<" doesn't exist"<<endl;
                }
                }
                else if(remove_user_choice==2){
                      int faculty_id;
                cout << "Enter faculty ID to remove: ";
                cin >> faculty_id;
                Faculty* removal_faculty = get_faculty(faculty_id);
                if(removal_faculty){
                if(removal_faculty->get_borrowed_books()!=0){
                    cout<<"Faculty of ID:"<<faculty_id<<" has borrowed books cannot be removed"<<endl;
                }
                else{
                        remove_faculty_user(faculty_id);
                        cout<<"Faulty of ID:"<<faculty_id<<" is no more an user of this library";
                }
                }
                else{
                    cout<<"Faculty of ID:"<<faculty_id<<" doesn't exist"<<endl;
                }
                }
                else if(remove_user_choice==3){
                     int librarian_id;
                cout << "Enter librarian ID to remove: ";
                cin >> librarian_id;
                Librarian* removal_librarian = get_librarian(librarian_id);
                if(removal_librarian){
                    if(removal_librarian->get_ID()==librarian->get_ID()){
                        cout<<"Are u sure to delete yourself as a librarian"<<endl;
                        int remove;
                        cout<<"enter 1 to continue ";
                        cin>>remove;
                        if(remove==1){
                            cout<<"U have removed yourself from librarian user sucessfully"<<endl;
                            remove_librarian(librarian_id);
                        }
                        else{
                            cout<<"Thank you"<<endl;
                        }
                    }
                    else{
                        remove_librarian(librarian_id);
                    }
                }
                else{
                    cout<<"Librarian of ID:"<<librarian_id<<" doesn't exist"<<endl;
                }   

                }
                else{
                    cout<<"Invalid Choice try again"<<endl;
                }
            }
            else if (choice == 6) {
                cout<<endl;
                cout<<"User Type"<<endl;
                cout<<"1. Student"<<endl;
                cout<<"2. Faculty"<<endl;
                int view_borrowed_choice;
                cin>>view_borrowed_choice;
                if(view_borrowed_choice==1){
                    int id;
                    cout<<"Enter ID: ";
                    cin>>id;
                    Student* student = get_student(id);
                    if(student){
                        if(student->get_borrowed_books()==0){
                            cout<<"Currently  no books are Borrowed by Student ID: "<<id<<endl;
                        } 
                        else{
                            int x=1;
                           for (const borrow_record& rec : (student->my_account).get_record()) {  
                                Book* book_borrowed = rec.book;  
                                   if (book_borrowed) {  // Check for null pointer  
                                        cout << x << ". Name: " << book_borrowed->get_name() << " (Book ID: " << book_borrowed->get_ID() << ") "  << "Borrow Time: " << rec.time_stamp << endl;  
                                             x++;  
                                             }  
                          }

                        }
                    } 
                    else{
                        cout<<"No student exist with ID: "<<id<<endl;
                    }
                }
                else if(view_borrowed_choice==2){
                    int id;
                    cout<<"Enter ID: ";
                    cin>>id;
                    Faculty* faculty = get_faculty(id);
                    if(faculty){
                        if(faculty->get_borrowed_books()==0){
                            cout<<"Currently  no books are Borrowed by Faculty ID: "<<id<<endl;
                        } 
                        else{
                            int x=1;
                           for (const borrow_record& rec : (faculty->my_account).get_record()) {  
                                Book* book_borrowed = rec.book;  
                                   if (book_borrowed) {  // Check for null pointer  
                                        cout << x << ". Name: " << book_borrowed->get_name() << " (Book ID: " << book_borrowed->get_ID() << ") "  << "Borrow Time: " << rec.time_stamp << endl;  
                                             x++;  
                                             }  
                          }

                        }
                    } 
                    else{
                        cout<<"No faculty exist with ID: "<<id<<endl;
                    }
                }
                else{
                    cout<<"Invalid choice, Try again"<<endl;
                }
            }
            else if (choice == 7){
                 cout<<"USER"<<endl;
                 cout<<"1. student"<<endl;
                 cout<<"2. faculty"<<endl;
                 int choice1;
                 cout<<"Enter your choice :";
                 cin>>choice1;
                 if(choice1==1){
                    int id;
                    cout<<"Enter Student ID: ";
                    cin>>id;
                    Student* stu = get_student(id);
                    if(stu){
                    if(((stu->my_account).get_history()).size()==0){
                        cout<<"NO history"<<endl;
                    }
                    else{
                    for(const Book* book: ((stu->my_account).get_history())){
                        cout<< book->get_name() <<" ( ID: "<<book->get_ID()<<" )"<<endl;
                    } 
                   }
                    }
                    else{
                        cout<<"No student with ID: "<<id<<endl;
                    }
                 }
                 else if(choice1==2){
                    int id;
                    cout<<"Enter Faculty ID: ";
                    cin>>id;
                    Faculty* fac = get_faculty(id);
                    if(fac){
                    if(((fac->my_account).get_history()).size()==0){
                        cout<<"NO history"<<endl;
                    }
                    else{
                    for(const Book* book: ((fac->my_account).get_history())){
                        cout<< book->get_name() <<" ( ID: "<<book->get_ID()<<" )"<<endl;
                    } 
                    }
                    }
                    else{
                        cout<<"No faculty with ID: "<<id<<endl;
                    }
                 }
            }
            else if (choice == 8) {
                cout << "Logging out..." << endl;
                save_library_data(); // Save before logout
                break;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void home() {
        cout << "\nWELCOME TO LIBRARY" << endl;
        while (true) {
            cout << "\nHome Menu" << endl;
            cout << "1. Login as Student" << endl;
            cout << "2. Login as Faculty" << endl;
            cout << "3. Login as Librarian" << endl;
            cout << "4. Exit" << endl;
            int x;
            cout << "Enter your choice: ";
            cin >> x;
            if (x == 1) {
                cout << "\nSTUDENT LOGIN" << endl;
                int student_id;
                cout << "Enter ID: ";
                cin >> student_id;
                string password;
                cout << "Enter Password: ";
                cin >> password;
                if (verify_student(student_id, password)) {
                    cout << "Login successful!" << endl;
                    Student* stu = get_student(student_id);
                    if (stu)
                        student_menu(stu);
                } else {
                    cout << "-------------------------------------" << endl;
                }
            } 
            else if (x == 2) {
                cout << "\nFACULTY LOGIN" << endl;
                int faculty_id;
                cout << "Enter ID: ";
                cin >> faculty_id;
                string password;
                cout << "Enter Password: ";
                cin >> password;
                if (verify_faculty(faculty_id, password)) {
                    cout << "Login successful!" << endl;
                    Faculty* fac = get_faculty(faculty_id);
                    if (fac)
                        faculty_menu(fac);
                } else {
                    cout << "-------------------------------------" << endl;
                }
            } 
            else if (x == 3) {
                cout << "\nLIBRARIAN LOGIN" << endl;
                int librarian_id;
                cout << "Enter ID: ";
                cin >> librarian_id;
                string password;
                cout << "Enter Password: ";
                cin >> password;
                if (verify_librarian(librarian_id, password)) {
                    cout << "Login successful!" << endl;
                    Librarian* lib = get_librarian(librarian_id);
                    if (lib)
                        librarian_menu(lib);
                } else {
                    cout << "-------------------------------------" << endl;
                }
            } 
            else if (x == 4) {
                cout << "Exiting..." << endl;
                break;
            } 
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

//borrow and return

void Student::book_borrow(int book_id, Library* lib) {
    int fine = my_account.calculate_fine();
    if (fine > 0) {
        cout << "Fine Over Due" << endl;
        return;
    }
    if (get_borrowed_books() >= 3) {
        cout << "Book Borrowing Limit Reached" << endl;
        cout << "Return a book to borrow new book" <<endl;
        return;
    }
    Book* book = lib->get_book(book_id);
    if (!book) {
        cout << "Book not found!" << endl;
        return;
    }
    else{
        if(!book->get_availability()){
            cout<<"Book is already borrowed by else"<<endl;
            if(book->get_reserver_id()==0){
                cout<<"Book can be reserved"<<endl;
                return;
            }
            else{
                cout<<"Book has already been reserved also"<<endl;
                return;
            }
        }
        else{
            if(book->get_reserver_id()!=0){
                if(book->get_reserver_id()==this->get_ID()){
                book->set_availability(false);
                my_account.borrow_book(book);
                increment_borrowed();
                book->set_reserver_id(0);
                cout << "Book borrowed successfully!" << endl;
                return;
                }
                else{
                    cout<<"Book is available but cannot be borrowed"<<endl;
                    cout<<"Book is reserved by User ID:"<<book->get_reserver_id()<<endl;
                    return;
                }
            }
            else{
                book->set_availability(false);
                my_account.borrow_book(book);
                increment_borrowed();
                cout << "Book borrowed successfully!" << endl;
                return;
            }
        }
    }
}

void Student::book_return(int book_id) {
    int fine = my_account.calculate_fine();
    if (fine > 0) {
        cout << "Fine Over Due. Pay Fine to return a book." << endl;
        return;
    }
    vector<borrow_record>& records = my_account.get_record();
    bool found = false;
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->book->get_ID() == book_id) {
            it->book->set_availability(true);
            records.erase(it);
            decrement_borrowed();
            cout << "Book returned successfully!" << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "No such book borrowed" << endl;
}

void Student::reserve_a_book(int book_id,Library* lib) {
     int fine = my_account.calculate_fine();
    if (fine > 0) {
        cout << "Fine Over Due" << endl;
        cout << "Book cannot be reserved" <<endl;
        return;
    }
    else{
        Book* book = lib->get_book(book_id);
        if(book){
           if(book->get_availability()){
              if(book->get_reserver_id()==0){
                   cout<<"Book can be directly borrowed"<<endl;
              }
              else{
                if(book->get_reserver_id()==this->get_ID()){
                    cout<<"book has be reserved by yourself, available to borrow now"<<endl;
                }
                else{
                    cout<<"book is reserved by someone else"<<endl;
                }
              }
           }
           else{
             if(book->get_reserver_id()==0){
                   book->set_reserver_id(this->get_ID());
                   cout<<"Book ID: "<<book_id<<" has been reserved to user ID: "<<this->get_ID()<<endl;
              }
              else{
                if(book->get_reserver_id()==this->get_ID()){
                    cout<<"book has be reserved by yourself already,but not available to borrow now"<<endl;
                }
                else{
                    cout<<"book is reserved by someone else"<<endl;
                }
              }
           }
        }
        else{
            cout << "No Book exist with ID: "<<book_id<<endl;
            return;
        }
    }
}

void Faculty::book_borrow(int book_id, Library* lib) {
    bool fine = my_account.faculty_overdue();
    if (fine) {
        cout << "Fine Over Due" << endl;
        return;
    }
    if (get_borrowed_books() >= 5) {
        cout << "Book Borrowing Limit Reached" << endl;
        cout << "Return a book to borrow new book" << endl;
        return;
    }
    Book* book = lib->get_book(book_id);
    if (!book) {
        cout << "Book not found!" << endl;
        return;
    }
    if (!book->get_availability()) {
        cout << "Book is already borrowed!" << endl;
        if(book->get_reserver_id()==0){
            cout << "Book can be reserved" << endl;
        }
        else{
            cout << "Book is already reserved by ID: " << book->get_reserver_id() << endl;
        }
        return;
    }
    else{
      if((book->get_reserver_id()==0)||(book->get_reserver_id()==this->get_ID())){       
    book->set_availability(false);
    my_account.borrow_book(book);
    increment_borrowed();
    cout << "Book borrowed successfully!" << endl;
    book->set_reserver_id(0);
      }
      else{
        cout<<"Book is already reserved by ID: "<<book->get_reserver_id()<<endl;
      }
    }
}

void Faculty::book_return(int book_id) {
    vector<borrow_record>& records = my_account.get_record();
    bool found = false;
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->book->get_ID() == book_id) {
            it->book->set_availability(true);
            records.erase(it);
            decrement_borrowed();
            cout << "Book returned successfully!" << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "No such book borrowed" << endl;
}

void Faculty::reserve_a_book(int book_id,Library* lib) {
     bool fine = my_account.faculty_overdue();
    if (fine) {
        cout << "Fine Over Due" << endl;
        cout << "Book cannot be reserved" <<endl;
        return;
    }
    else{
        Book* book = lib->get_book(book_id);
        if(book){
           if(book->get_availability()){
              if(book->get_reserver_id()==0){
                   cout<<"Book can be directly borrowed"<<endl;
              }
              else{
                if(book->get_reserver_id()==this->get_ID()){
                    cout<<"book has be reserved by yourself, available to borrow now"<<endl;
                }
                else{
                    cout<<"book is reserved by someone else"<<endl;
                }
              }
           }
           else{
             if(book->get_reserver_id()==0){
                   book->set_reserver_id(this->get_ID());
                   cout<<"Book ID: "<<book_id<<" has been reserved to user ID: "<<this->get_ID()<<endl;
              }
              else{
                if(book->get_reserver_id()==this->get_ID()){
                    cout<<"book has be reserved by yourself already,but not available to borrow now"<<endl;
                }
                else{
                    cout<<"book is reserved by someone else"<<endl;
                }
              }
           }
        }
        else{
            cout << "No Book exist with ID: "<<book_id<<endl;
            return;
        }
    }
}


int User::next_user_ID = 1;
int Book::next_book_ID = 1;
//====================== Main Function ======================

int main() {
    Library lib;
    // Load all library data from separate files
    lib.load_library_data();
    // If the library is empty, auto-add test data
      
    if(lib.BookEmpty()&&lib.StudentEmpty()&&lib.FacultyEmpty()&&lib.LibrarianEmpty()){
       cout<<"No data found"<<endl;
       lib.add_book(new Book("Book1", "Author1", "Publisher1", "ISBN1"));
       lib.add_book(new Book("Book2", "Author2", "Publisher2", "ISBN2"));
       lib.add_book(new Book("Book3", "Author3", "Publisher3", "ISBN3"));
       lib.add_book(new Book("Book4", "Author4", "Publisher4", "ISBN4"));
       lib.add_book(new Book("Book5", "Author5", "Publisher5", "ISBN5"));
       lib.add_book(new Book("Book6", "Author6", "Publisher6", "ISBN6"));
       lib.add_book(new Book("Book7", "Author7", "Publisher7", "ISBN7"));
       lib.add_book(new Book("Book8", "Author8", "Publisher8", "ISBN8"));
       lib.add_book(new Book("Book9", "Author9", "Publisher9", "ISBN9"));
       lib.add_book(new Book("Book10", "Author10", "Publisher10", "ISBN10"));

            lib.add_student(new Student("student1", "password1"));
            lib.add_student(new Student("student2", "password2"));
            lib.add_student(new Student("student3", "password3"));
            lib.add_student(new Student("student4", "password4"));
            lib.add_student(new Student("student5", "password5"));

            lib.add_faculty(new Faculty("faculty1", "password1"));
            lib.add_faculty(new Faculty("faculty2", "password2"));
            lib.add_faculty(new Faculty("faculty3", "password3"));

            lib.add_librarian(new Librarian("librarian1", "password1"));
            lib.save_library_data();
            cout<<" Default Data loaded successfully"<<endl;
    }
    else{
        cout<<"Data loaded successfully"<<endl;
    }
    if(lib.BookEmpty()){
        
    }
    // Display the home menu for user login/interaction
    lib.home();
        
    // Save all data before exit
    lib.save_library_data();
    return 0;
}
