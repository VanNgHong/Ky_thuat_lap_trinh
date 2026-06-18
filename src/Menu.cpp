// ============================================================
//  Menu.cpp  –  Triển khai giao diện Menu và điều hướng
//  Tác giả: THÀNH  |  Role: System Architect / UI / QA
// ============================================================

#include "../include/Menu.h"
#include "../include/QuestionBank.h"
#include "../include/Exam.h"
#include "../include/History.h"

#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <cctype>

using namespace std;

// ============================================================
//  HÀM TIỆN ÍCH TERMINAL
// ============================================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printLine(char c, int width) {
    cout << string(width, c) << "\n";
}

void printCentered(const string& text, int width) {
    int len = static_cast<int>(text.length());
    if (len >= width) {
        cout << text << "\n";
        return;
    }
    int padding = (width - len) / 2;
    cout << string(padding, ' ') << text << "\n";
}

// ============================================================
//  INPUT VALIDATION
// ============================================================

int getValidInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << Color::RED << "Gia tri phai trong khoang ["
                 << minVal << ", " << maxVal << "]!\n" << Color::RESET;
        } else {
            if (cin.eof()) {
                cout << "\n[He thong] Het du lieu dau vao. Thoat chuong trinh.\n";
                exit(0);
            }
            cout << Color::RED << "Vui long nhap mot so nguyen hop le!\n" << Color::RESET;
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getValidString(const string& prompt, int maxLen) {
    string input;
    while (true) {
        cout << prompt;
        if (!getline(cin, input)) {
            cout << "\n[He thong] Het du lieu dau vao. Thoat chuong trinh.\n";
            exit(0);
        }

        if (!input.empty() && static_cast<int>(input.length()) <= maxLen) {
            return input;
        }

        if (input.empty()) {
            cout << Color::RED << "Chuoi khong duoc de trong!\n" << Color::RESET;
        } else {
            cout << Color::RED << "Chuoi qua dai (toi da " << maxLen << " ky tu)!\n" << Color::RESET;
        }
    }
}

bool getYesNo(const string& prompt) {
    while (true) {
        cout << prompt << " (Y/N): ";
        string input;
        if (!getline(cin, input)) {
            cout << "\n[He thong] Het du lieu dau vao. Thoat chuong trinh.\n";
            exit(0);
        }

        if (!input.empty()) {
            char c = static_cast<char>(toupper(static_cast<unsigned char>(input[0])));
            if (c == 'Y') return true;
            if (c == 'N') return false;
        }
        cout << Color::RED << "Vui long nhap Y hoac N!\n" << Color::RESET;
    }
}

// Tính so cau hoi toi da co the sinh de duoc, dua tren dung cong thuc chia ti le 40% Easy - 40% Medium - 20% Hard
int getMaxNumberQuest(int nEasy, int nMedium, int nHard)
{
    int total = nEasy + nMedium + nHard;

    // Thu N tu lon -> nho. N hop le dau tien tim duoc chinh la so cau toi da.
    for (int n = total; n >= 1; n--) {
        int easyNeed   = n * 40 / 100;
        int mediumNeed = n * 40 / 100;
        int hardNeed   = n - easyNeed - mediumNeed;

        if (easyNeed <= nEasy && mediumNeed <= nMedium && hardNeed <= nHard) {
            return n;
        }
    }

    return 0;   // Khong co N nao hop le (vi du thieu han 1 muc do)
}

// ============================================================
//  MÀN HÌNH CHÀO MỪNG
// ============================================================

void showWelcomeBanner() {
    clearScreen();
    cout << Color::CYAN << Color::BOLD;
    printLine('=');
    printCentered("HE THONG THI TRAC NGHIEM MON TIENG ANH");
    printLine('=');
    cout << Color::RESET;
    pauseScreen();
}

// ============================================================
//  ĐĂNG NHẬP
// ============================================================

LoginResult showLoginScreen(LoginSession& session) {
    clearScreen();
    cout << Color::CYAN << Color::BOLD;
    printLine('=');
    printCentered("DANG NHAP HE THONG");
    printLine('=');
    cout << Color::RESET << "\n";

    cout << "(Go 'exit' o ten dang nhap de thoat chuong trinh)\n\n";

    string username = getValidString("Ten dang nhap: ", 50);

    string lowerUsername = username;
    for (char& c : lowerUsername) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    if (lowerUsername == "exit") {
        return LoginResult::Exit;
    }

    string password = getValidString("Mat khau: ", 50);

    // Tai khoan Admin
    if (username == "admin" && password == "admin123") {
        session.username = username;
        session.isAdmin  = true;
        return LoginResult::Success;
    }

    // Tai khoan Sinh vien: password = "sv" + username
    if (password == ("sv" + username)) {
        session.username = username;
        session.isAdmin  = false;
        return LoginResult::Success;
    }

    cout << "\n" << Color::RED << "Sai ten dang nhap hoac mat khau!\n" << Color::RESET;
    pauseScreen();
    return LoginResult::InvalidCredentials;
}

// ============================================================
//  MENU ADMIN
// ============================================================

void showAdminMenu(LoginSession& session) {
    const string QUESTIONS_FILE = "data/questions.txt";
    const string HISTORY_FILE   = "data/history.txt";

    QuestionBank bank;
    bank.loadFromFile(QUESTIONS_FILE);

    bool running = true;
    while (running) {
        clearScreen();
        cout << Color::MAGENTA << Color::BOLD;
        printLine('=');
        printCentered("MENU QUAN TRI VIEN (" + session.username + ")");
        printLine('=');
        cout << Color::RESET;

        cout << "1. Xem danh sach cau hoi\n";
        cout << "2. Them cau hoi moi\n";
        cout << "3. Xoa cau hoi theo ID\n";
        cout << "4. Xem lich su thi cua tat ca sinh vien\n";
        cout << "5. Dang xuat\n";
        printLine();

        int choice = getValidInt("Chon chuc nang (1-5): ", 1, 5);

        switch (choice) {
            case 1: {
                clearScreen();
                cout << Color::BOLD;
                printCentered("DANH SACH CAU HOI");
                cout << Color::RESET;
                printLine();

                if (bank.getQuestionCount() == 0) {
                    cout << "Ngan hang cau hoi dang rong.\n";
                    pauseScreen();
                    break;
                }

                int subjectCount = 0;
                string* subjects = bank.getDistinctSubjects(subjectCount);

                cout << "Danh sach cac mon hoc:\n";
                for (int i = 0; i < subjectCount; i++) {
                    cout << "- " << subjects[i] << "\n";
                }

                string subject;
                while (true){
                    cout << "\nNhap mon hoc muon xem cau hoi (Nhap ALL neu muon in het tat ca): ";
                    getline(cin, subject);

                    if (subject == "all" || subject == "ALL" || subject == "All")
                    {
                        bank.printAll();
                        pauseScreen();
                        break;
                    }

                    bool found = false;
                    for (int i = 0; i < subjectCount; i++)
                    {
                        if (subjects[i] == subject)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (found)
                    {
                        bank.printBySubject(subject);
                        pauseScreen();
                        break;
                    }

                    cout << Color::RED
                        << "\n[LOI] Khong ton tai mon hoc \"" << subject << "\"!\n"
                        << "Vui long nhap lai.\n"
                        << Color::RESET;
                }
                break;
            }

            case 2: {
                clearScreen();
                cout << Color::BOLD;
                printCentered("THEM CAU HOI MOI");
                cout << Color::RESET;
                printLine();

                Question q;
                while (true) {
                    q.id = getValidInt("Nhap ID cau hoi (so nguyen duong): ", 1, 1000000);
                    if (bank.isIdExists(q.id)) {
                        cout << Color::RED << "[Loi] ID " << q.id
                            << " da ton tai! Vui long nhap mot ID khac.\n" << Color::RESET;
                    } else {
                    break;
                    }
                }

                q.subject = getValidString("Mon hoc: ", 50);

                while (true) {
                    string diff = getValidString("Muc do (Easy / Medium / Hard): ", 10);
                    if (!diff.empty()) {
                        diff[0] = toupper((unsigned char)diff[0]);
                        for (int i = 1; i < (int)diff.size(); i++)
                            diff[i] = tolower((unsigned char)diff[i]);
                    }
                    if (diff == "Easy" || diff == "Medium" || diff == "Hard") {
                        q.difficulty = diff;
                        break;
                    }
                    cout << Color::RED << "Chi chap nhan: Easy, Medium, hoac Hard!\n" << Color::RESET;
                }

                q.content    = getValidString("Noi dung cau hoi: ", 200);
                q.answers[0] = getValidString("Dap an A: ", 100);
                q.answers[1] = getValidString("Dap an B: ", 100);
                q.answers[2] = getValidString("Dap an C: ", 100);
                q.answers[3] = getValidString("Dap an D: ", 100);

                while (true) {
                    string cAns = getValidString("Dap an dung (A/B/C/D): ", 5);
                    char up = toupper((unsigned char)cAns[0]);
                    if (up == 'A' || up == 'B' || up == 'C' || up == 'D') {
                        q.correct = up;
                        break;
                    }
                    cout << Color::RED << "Vui long nhap A, B, C hoac D!\n" << Color::RESET;
                }

                if (bank.addQuestion(q)) {
                    bank.saveToFile(QUESTIONS_FILE);
                    cout << Color::GREEN << "\n[OK] Da them cau hoi thanh cong!\n" << Color::RESET;
                } else {
                    cout << Color::RED << "\n[Loi] Khong the them cau hoi (ID bi trung).\n" << Color::RESET;
                }
                pauseScreen();
                break;
            }

            case 3: {
                clearScreen();
                cout << Color::BOLD;
                printCentered("XOA CAU HOI");
                cout << Color::RESET;
                printLine();

                if (bank.getQuestionCount() == 0) {
                    cout << "Ngan hang cau hoi dang rong.\n";
                } else {
                    bank.printAll();
                    printLine();
                    int id = getValidInt("Nhap ID cau hoi can xoa: ", 1, 1000000);
                    bank.removeQuestion(id);
                    bank.saveToFile(QUESTIONS_FILE);
                }
                pauseScreen();
                break;
            }

            case 4: {
                HistoryManager history;
                history.loadFromFile(HISTORY_FILE);

                clearScreen();
                cout << Color::BOLD;
                printCentered("LICH SU THI CUA SINH VIEN");
                cout << Color::RESET;
                printLine();
                history.printAll();
                pauseScreen();
                break;
            }

            case 5:
                running = false;
                break;
        }
    }
}

// ============================================================
//  MENU SINH VIÊN
// ============================================================

void showStudentMenu(LoginSession& session) {
    const string QUESTIONS_FILE = "data/questions.txt";
    const string HISTORY_FILE   = "data/history.txt";

    bool running = true;
    while (running) {
        clearScreen();
        cout << Color::BLUE << Color::BOLD;
        printLine('=');
        printCentered("MENU SINH VIEN (" + session.username + ")");
        printLine('=');
        cout << Color::RESET;

        cout << "1. Bat dau lam bai thi\n";
        cout << "2. Xem lich su thi cua ban\n";
        cout << "3. Dang xuat\n";
        printLine();

        int choice = getValidInt("Chon chuc nang (1-3): ", 1, 3);

        switch (choice) {
            case 1: {
                QuestionBank bank;
                bank.loadFromFile(QUESTIONS_FILE);

                if (bank.getQuestionCount() == 0) {
                    cout << Color::RED << "\nNgan hang cau hoi rong!\n" << Color::RESET;
                    pauseScreen();
                    break;
                }

                // ── Cho sinh vien chon mon hoc ───────────────────────────
                int subjectCount = 0;
                string* subjects = bank.getDistinctSubjects(subjectCount);

                clearScreen();
                cout << Color::BOLD;
                printCentered("CHON MON HOC DE THI");
                cout << Color::RESET;
                printLine();

                for (int i = 0; i < subjectCount; i++) {
                    cout << (i + 1) << ". " << subjects[i] << "\n";
                }
                printLine();

                int subjectChoice = getValidInt("Chon mon hoc (1-" + to_string(subjectCount) + "): ", 1, subjectCount);

                string selectedSubject = subjects[subjectChoice - 1];
                delete[] subjects;   

                int nEasy   = bank.countBySubjectAndDifficulty(selectedSubject, "Easy");
                int nMedium = bank.countBySubjectAndDifficulty(selectedSubject, "Medium");
                int nHard   = bank.countBySubjectAndDifficulty(selectedSubject, "Hard");

                clearScreen();
                cout << Color::BOLD;
                printCentered("BAT DAU LAM BAI THI - MON " + selectedSubject);
                cout << Color::RESET;
                printLine();

                int maxQ = getMaxNumberQuest(nEasy, nMedium, nHard);

                if (maxQ < 1) {
                    cout << Color::RED
                        << "[Loi] Mon " << selectedSubject
                        << " chua du cau hoi theo ca 3 muc do de sinh de!\n"
                        << Color::RESET;
                    pauseScreen();
                    break;
                }

                int numQ = getValidInt("Nhap so cau hoi muon thi (1-" + to_string(maxQ) + "): ", 1, maxQ);
                int timeLimit = getValidInt("Nhap thoi gian lam bai (giay, toi thieu 10): ", 10, 36000);
                if (!getYesNo("\nBat dau lam bai?")) break;

                // ── Sinh de CHI TU mon hoc da chon ───────────────────────
                Exam exam(numQ, timeLimit);
                if (!exam.generateExamFromBank(bank, selectedSubject)) {
                    cout << Color::RED
                        << "[Loi] Khong the sinh de thi (ngan hang khong du theo ti le 40-40-20).\n"
                        << Color::RESET;
                    pauseScreen();
                    break;
                }
                exam.shuffleExam();

                clearScreen();
                TestRecord record = exam.startExam(session.username, exam.timeLimitSec);
                record.subject = selectedSubject;

                clearScreen();
                cout << Color::GREEN << Color::BOLD;
                printLine('=');
                printCentered("KET QUA BAI THI");
                printLine('=');
                cout << Color::RESET;

                cout << "Sinh vien  : " << record.studentName  << "\n";
                cout << "Mon hoc    : " << record.subject       << "\n";  
                cout << "So cau dung: " << record.correctCount << "/" << record.totalCount << "\n";
                cout << fixed << setprecision(2);
                cout << "Diem so    : " << record.score << " / 10\n";
                cout << "Thoi gian  : " << record.datetime << "\n";

                HistoryManager history;
                history.addRecord(record);
                history.saveToFile(HISTORY_FILE);

                pauseScreen();
                break;
            }

            case 2: {
                HistoryManager history;
                history.loadFromFile(HISTORY_FILE);

                clearScreen();
                cout << Color::BOLD;
                printCentered("LICH SU LAM BAI THI CUA BAN");
                cout << Color::RESET;
                printLine();
                cout << fixed << setprecision(2);
                history.printByUser(session.username);
                pauseScreen();
                break;
            }

            case 3:
                running = false;
                break;
        }
    }
}
