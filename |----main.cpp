#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

/* =========================
   STUDENT CLASS
========================= */
class Student {
private:
    string indexNumber;
    string name;

public:
    Student() {}

    Student(string index, string n) {
        indexNumber = index;
        name = n;
    }

    string getIndex() const {
        return indexNumber;
    }

    string getName() const {
        return name;
    }

    string toFileString() const {
        return indexNumber + "," + name;
    }

    static Student fromFileString(string line) {
        int pos = line.find(",");
        string index = line.substr(0, pos);
        string name = line.substr(pos + 1);
        return Student(index, name);
    }
};

/* =========================
   GLOBAL VARIABLES
========================= */
vector<Student> students;
string studentFile = "students.txt";

/* =========================
   INPUT VALIDATION
========================= */
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedInt() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input. Enter a number: ";
        clearInput();
    }
    return value;
}

bool studentExists(string index) {
    for (const Student& s : students) {
        if (s.getIndex() == index)
            return true;
    }
    return false;
}

string getValidatedStatus() {
    string status;

    while (true) {
        cout << "Enter Status (P = Present, L = Late, A = Absent): ";
        cin >> status;

        transform(status.begin(), status.end(), status.begin(), ::toupper);

        if (status == "P") return "Present";
        if (status == "L") return "Late";
        if (status == "A") return "Absent";

        cout << "Invalid choice. Try again.\n";
    }
}

/* =========================
   FILE HANDLING
========================= */
void loadStudents() {
    students.clear();
    ifstream file(studentFile);

    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        students.push_back(Student::fromFileString(line));
    }

    file.close();
}

void saveStudents() {
    ofstream file(studentFile);

    for (const Student& s : students) {
        file << s.toFileString() << endl;
    }

    file.close();
}

/* =========================
   STUDENT MANAGEMENT
========================= */
void registerStudent() {
    string index, name;

    cout << "Enter Index Number: ";
    cin >> index;

    if (studentExists(index)) {
        cout << "Error: Student with this index already exists.\n";
        return;
    }

    cin.ignore();
    cout << "Enter Full Name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Name cannot be empty.\n";
        return;
    }

    students.push_back(Student(index, name));
    saveStudents();

    cout << "Student Registered Successfully!\n";
}

void viewStudents() {
    cout << "\n--------------------------------------------\n";
    cout << left << setw(15) << "Index Number"
         << setw(25) << "Name" << endl;
    cout << "--------------------------------------------\n";

    if (students.empty()) {
        cout << "No students registered.\n";
        return;
    }

    for (const Student& s : students) {
        cout << left << setw(15) << s.getIndex()
             << setw(25) << s.getName() << endl;
    }

    cout << "--------------------------------------------\n";
}

void searchStudent() {
    string index;
    cout << "Enter Index Number to Search: ";
    cin >> index;

    for (const Student& s : students) {
        if (s.getIndex() == index) {
            cout << "\nStudent Found:\n";
            cout << left << setw(15) << "Index:"
                 << s.getIndex() << endl;
            cout << left << setw(15) << "Name:"
                 << s.getName() << endl;
            return;
        }
    }

    cout << "Student Not Found.\n";
}

/* =========================
   ATTENDANCE SESSION
========================= */
void createSession() {
    string courseCode, date, startTime;
    int duration;

    cout << "Enter Course Code: ";
    cin >> courseCode;

    cout << "Enter Date (YYYY_MM_DD): ";
    cin >> date;

    cout << "Enter Start Time (HH:MM): ";
    cin >> startTime;

    cout << "Enter Duration (minutes): ";
    duration = getValidatedInt();

    string filename = "session_" + courseCode + "_" + date + ".txt";

    ofstream file(filename);

    file << "Course: " << courseCode << endl;
    file << "Date: " << date << endl;
    file << "Start Time: " << startTime << endl;
    file << "Duration: " << duration << " minutes\n";
    file << "-----------------------------------\n";

    for (const Student& s : students) {
        file << s.getIndex() << "," << s.getName() << ",Absent\n";
    }

    file.close();

    cout << "Session Created Successfully!\n";
}

/* =========================
   MARK ATTENDANCE
========================= */
void markAttendance() {
    string filename;
    cout << "Enter Session File Name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Session file not found.\n";
        return;
    }

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    cout << "\nMark Attendance\n";

    for (int i = 5; i < lines.size(); i++) {
        cout << "\nStudent Record: " << lines[i] << endl;

        string newStatus = getValidatedStatus();

        int lastComma = lines[i].rfind(",");
        lines[i] = lines[i].substr(0, lastComma + 1) + newStatus;
    }

    ofstream outFile(filename);
    for (const string& l : lines) {
        outFile << l << endl;
    }

    outFile.close();

    cout << "Attendance Updated Successfully!\n";
}

/* =========================
   REPORT SUMMARY
========================= */
void displayReport() {
    string filename;
    cout << "Enter Session File Name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File not found.\n";
        return;
    }

    string line;
    int present = 0, absent = 0, late = 0;

    while (getline(file, line)) {
        if (line.find(",Present") != string::npos)
            present++;
        else if (line.find(",Absent") != string::npos)
            absent++;
        else if (line.find(",Late") != string::npos)
            late++;
    }

    file.close();

    cout << "\n--------------------------------\n";
    cout << left << setw(15) << "Status"
         << setw(10) << "Count" << endl;
    cout << "--------------------------------\n";

    cout << left << setw(15) << "Present" << setw(10) << present << endl;
    cout << left << setw(15) << "Late" << setw(10) << late << endl;
    cout << left << setw(15) << "Absent" << setw(10) << absent << endl;

    cout << "--------------------------------\n";
}

/* =========================
   MAIN MENU
========================= */
void menu() {
    int choice;

    do {
        cout << "\n===== DIGITAL ATTENDANCE SYSTEM =====\n";
        cout << "1. Register Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Create Attendance Session\n";
        cout << "5. Mark Attendance\n";
        cout << "6. Display Attendance Report\n";
        cout << "0. Exit\n";
        cout << "Enter Choice: ";

        choice = getValidatedInt();

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: createSession(); break;
            case 5: markAttendance(); break;
            case 6: displayReport(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

/* =========================
   MAIN FUNCTION
========================= */
int main() {
    loadStudents();
    menu();
    return 0;
}