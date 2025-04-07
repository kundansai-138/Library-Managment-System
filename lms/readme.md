# Library Management System - README

Welcome to the Library Management System, a comprehensive C++ application designed to manage library operations efficiently. This system supports three types of users: **Students**, **Faculty**, and **Librarians**, each with distinct privileges and functionalities.

---

## Features

### **User Authentication**
- **Students**, **Faculty**, and **Librarians** log in with unique credentials.
- Secure password handling for all user types.

### **Book Management**
- Add, remove, and view books.
- Track availability, reservations, and borrowing history.
- Book details include:
  - ID, Name, Author, Publisher, ISBN.
  - Availability status and reserver ID (if reserved).

### **User-Specific Functionalities**
#### **Students**:
- Borrow up to **3 books** with a **15-day limit** (fines: ₹10/day after due).
- Reserve unavailable books.
- View borrowing history and pay fines.
- Edit profile (username/password).

#### **Faculty**:
- Borrow up to **5 books** with a **60-day limit** (no fines, but overdue status).
- Reserve books.
- View borrowing history.
- Edit profile (username/password).

#### **Librarians**:
- Add/remove books and users (students, faculty, librarians).
- View all borrowed books and user histories.
- Manage library data persistence.

---

## User Guide

### **Login Options**
1. **Student**: Enter ID and password to borrow books, check fines, and view history.
2. **Faculty**: Access extended borrowing limits and manage reservations.
3. **Librarian**: Full administrative control over books and users.

---

## Classes Overview
1. **User**: Base class for all users (ID, username, password).
2. **Book**: Manages book details (ID, name, author, ISBN, availability).
3. **borrow_record**: Tracks borrowed books and timestamps.
4. **Account**: Handles borrowing history, fines (students), and overdue status (faculty).
5. **Student/Faculty/Librarian**: Inherit from `User` with role-specific features.
6. **Library**: Central class for operations, menus, and data storage.

---

## Data Storage
All data is saved in text files for persistence:
- **Books**: `books.txt`
- **Users**: `students.txt`, `faculty.txt`, `librarians.txt`
- **Borrow Records**: `student_records.txt`, `faculty_records.txt`
- **History**: `student_history.txt`, `faculty_history.txt`
- **Next IDs**: `user_next_ID.txt`, `book_next_ID.txt`

---
# Library Management System (LMS)

## Overview

The **Library Management System (LMS)** is a C++ program designed to manage library operations. It allows users to borrow, return, and reserve books, while also providing functionalities for librarians to manage books and users. The system supports three types of users: **Students**, **Faculty**, and **Librarians**, each with different privileges.

---

## Features

### User Types and Their Capabilities

1. **Student**:
   - Borrow up to 3 books.
   - Return borrowed books.
   - Reserve books that are currently unavailable.
   - View borrowing history.
   - Check and pay fines (if any).
   - Edit personal details (username and password).

2. **Faculty**:
   - Borrow up to 5 books.
   - Return borrowed books.
   - Reserve books that are currently unavailable.
   - View borrowing history.
   - Check overdue status (no fines, but books must be returned within 60 days).
   - Edit personal details (username and password).

3. **Librarian**:
   - Add, remove, and view books in the library.
   - Add or remove users (Students, Faculty, and Librarians).
   - View current borrowed books by users.
   - View borrowing history of users.
   - Edit personal details (username and password).

---

### Book Management

- Books can be added, removed, and viewed by librarians.
- Each book has a unique ID, name, author, publisher, ISBN, and availability status.
- Books can be reserved by users if they are currently unavailable.

---

### Fine Calculation

- **Students** are charged a fine of **10 Rupees per day** for each book that is overdue (more than 15 days).
  - Fine calculation is done **day by day** (not continuous).
- **Faculty** do not incur fines but are marked as overdue if they keep a book for more than 60 days.

---

### Data Persistence

- The system saves all data (books, users, borrowing records, etc.) to text files, ensuring that data is retained between program runs.
- Data is loaded automatically when the program starts.

---

## File Structure

The program uses the following files to store data:

1. **books.txt**: Stores book information.
   - Format: `ID|Name|Author|Publisher|ISBN|Available|reserver_id`

2. **students.txt**: Stores student user information.
   - Format: `ID|Username|Password|BorrowedBooksCount`

3. **student_records.txt**: Stores borrowing records for students.
   - Format: `StudentID|BookID|TimeStamp`

4. **student_history.txt**: Stores borrowing history for students.
   - Format: `StudentID|BookID`

5. **faculty.txt**: Stores faculty user information.
   - Format: `ID|Username|Password|BorrowedBooksCount`

6. **faculty_records.txt**: Stores borrowing records for faculty.
   - Format: `FacultyID|BookID|TimeStamp`

7. **faculty_history.txt**: Stores borrowing history for faculty.
   - Format: `FacultyID|BookID`

8. **librarians.txt**: Stores librarian user information.
   - Format: `ID|Username|Password`

9. **user_next_ID.txt**: Stores the next available user ID.
10. **book_next_ID.txt**: Stores the next available book ID.

---

## How to Use

1. **Compilation**:
   - Ensure you have a C++ compiler installed (e.g., `g++`).
   - Compile the program using the following command:
     ```bash
     g++ -o lms lms.cpp
     ```

2. **Execution**:
   - Run the compiled program:
     ```bash
     ./lms
     ```

3. **Default Data**:
   - If no data files are found, the system will automatically load default data (10 books, 5 students, 3 faculty members, and 1 librarian).

4. **Login**:
   - Upon running the program, you will be presented with a home menu where you can log in as a **Student**, **Faculty**, or **Librarian**.
   - Use the default credentials or create new users to log in.

5. **Navigation**:
   - After logging in, follow the on-screen menus to perform various operations such as borrowing, returning, or reserving books.

---

## Assumptions

1. **Input Choices**:
   - All menu choices are assumed to be **integers**. Invalid inputs (e.g., strings or floats) may cause unexpected behavior.

2. **Fine Calculation**:
   - Fines are calculated **day by day** (not continuously). For example, if a book is overdue by 16 days, the fine will be 10 Rupees (for 1 day over the 15-day limit).

3. **File Storage**:
   - All data files are stored in the **same directory** as the executable. If the program is moved, the files will be created in the new directory.

4. **Overdue for Faculty**:
   - Faculty members are marked as overdue if they keep a book for more than 60 days, but no fines are imposed.
5. **Reservation**
   - reserver_id = 0, no reservation has been made, any other number mean that particular user id made the reservation.
6. **Initial Inputs**
   - in case of already ran file the input will be as follows
   - Students : student1-student5 (ID: 1-5) password: password1-password5
   - Faculty  : faculty1-faculty3 (ID: 6-8) password: password1-password3
   - Librarian: librarian1 (ID: 9) password:password1
   - Books : book1-book10 (ID 1-10) similarly,other attributes will add id at the end of it
7. **ID**
   - in this system, i am auto generating id's of the users and books and id would be used as unique representation of an user and a book.
---

## Example Usage

### Student Login
1. Log in as a student.
2. Borrow a book by entering the book ID.
3. Return a book by entering the book ID.
4. Reserve a book that is currently unavailable.
5. Check and pay fines if any books are overdue.

### Faculty Login
1. Log in as a faculty member.
2. Borrow a book by entering the book ID.
3. Return a book by entering the book ID.
4. Reserve a book that is currently unavailable.
5. Check overdue status.

### Librarian Login
1. Log in as a librarian.
2. Add or remove books from the library.
3. Add or remove users (students, faculty, or librarians).
4. View current borrowed books and borrowing history of users.

---

## Notes

- The program is designed to be simple and easy to use, with all data stored in text files for easy access and modification.
- Ensure that the program has **read/write permissions** for the directory where the data files are stored.

---

## Author

- **C. kundan sai**
- **ckundans23@iitk.ac.in** (if applicable)

---