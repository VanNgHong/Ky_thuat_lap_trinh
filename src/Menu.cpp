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
                } else {
                    bank.printAll();
                }
                pauseScreen();
                break;
            }

            case 2: {
                clearScreen();
                cout << Color::BOLD;
                printCentered("THEM CAU HOI MOI");
                cout << Color::RESET;
                printLine();

                Question q;
                q.id = getValidInt("Nhap ID cau hoi (so nguyen duong): ", 1, 1000000);
                q.content   = getValidString("Noi dung cau hoi: ", 200);
                q.answers[0] = getValidString("Dap an A: ", 100);
                q.answers[1] = getValidString("Dap an B: ", 100);
                q.answers[2] = getValidString("Dap an C: ", 100);
                q.answers[3] = getValidString("Dap an D: ", 100);

                while (true) {
                    string cAns = getValidString("Dap an dung (A/B/C/D): ", 5);
                    char up = static_cast<char>(toupper(static_cast<unsigned char>(cAns[0])));
                    if (up == 'A' || up == 'B' || up == 'C' || up == 'D') {
                        q.correct = up;
                        break;
                    }
                    cout << Color::RED << "Vui long nhap A, B, C hoac D!\n" << Color::RESET;
                }

                bank.addQuestion(q);
                bank.saveToFile(QUESTIONS_FILE);

                cout << Color::GREEN << "\n[OK] Da them cau hoi thanh cong!\n" << Color::RESET;
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

                int total = bank.getQuestionCount();
                if (total == 0) {
                    cout << Color::RED << "\nNgan hang cau hoi rong, khong the tao de thi!\n" << Color::RESET;
                    pauseScreen();
                    break;
                }

                clearScreen();
                cout << Color::BOLD;
                printCentered("BAT DAU LAM BAI THI");
                cout << Color::RESET;
                printLine();
                cout << "Ngan hang hien co " << total << " cau hoi.\n\n";

                int numQ = getValidInt(
                    "Nhap so cau hoi muon thi (1-" + to_string(total) + "): ", 1, total);
                int timeLimit = getValidInt(
                    "Nhap thoi gian lam bai (giay, toi thieu 10): ", 10, 36000);

                if (!getYesNo("\nBat dau lam bai?")) {
                    break;
                }

                Question* selected = bank.generateRandomSet(numQ);
                if (selected == nullptr){
                    cout << "Khong tao duoc de thi!\n";
                    pauseScreen();
                    break;
                }


                Exam exam(numQ, timeLimit);
                exam.loadFromBank(selected);
                delete[] selected;       
                // giải phóng sau khi đã load vào Exam
                exam.shuffleExam();      
                // hoặc exam.shuffle() nếu bạn đổi tên
                clearScreen();
                TestRecord record = exam.startExam(session.username, exam.timeLimitSec);
                cout << "\nRETURNED FROM startExam()\n";
                pauseScreen(); 
                clearScreen();
                
                cout << Color::GREEN << Color::BOLD;
                cout << Color::GREEN << Color::BOLD;
                printLine('=');
                printCentered("KET QUA BAI THI");
                printLine('=');
                cout << Color::RESET;

                cout << "Sinh vien : " << record.studentName << "\n";
                cout << "So cau dung: " << record.correctCount << "/" << record.totalCount << "\n";
                cout << fixed << setprecision(2);
                cout << "Diem so   : " << record.score << " / 10\n";
                cout << "Thoi gian : " << record.datetime << "\n";

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
