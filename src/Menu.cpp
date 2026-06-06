// ============================================================
//  Menu.cpp  –  Xử lý I/O màn hình, bẫy lỗi nhập liệu
//  Tác giả: THÀNH  |  Role: System Architect / UI / QA
// ============================================================

#include "Menu.h"
#include "QuestionBank.h"
#include "Exam.h"
#include "History.h"

#include <iostream>
#include <string>
#include <limits>
#include <sstream>

// ────────────────────────────────────────────────────────────
//  Hàm tiện ích Terminal
// ────────────────────────────────────────────────────────────

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\n" << Color::YELLOW
              << "  Nhấn Enter để tiếp tục..." << Color::RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printLine(char c, int width) {
    std::cout << "  ";
    for (int i = 0; i < width; ++i) std::cout << c;
    std::cout << "\n";
}

void printCentered(const std::string& text, int width) {
    int padding = (width - static_cast<int>(text.size())) / 2;
    if (padding < 0) padding = 0;
    std::cout << "  ";
    for (int i = 0; i < padding; ++i) std::cout << ' ';
    std::cout << text << "\n";
}

// ────────────────────────────────────────────────────────────
//  Banner chào mừng
// ────────────────────────────────────────────────────────────

void showWelcomeBanner() {
    clearScreen();
    std::cout << Color::CYAN << Color::BOLD;
    printLine('=');
    printCentered("HE THONG THI TRAC NGHIEM");
    printCentered("Mon Tieng Anh");
    printLine('=');
    std::cout << Color::RESET << "\n";
}

// ────────────────────────────────────────────────────────────
//  Đăng nhập
// ────────────────────────────────────────────────────────────

LoginResult showLoginScreen(LoginSession& session) {
    showWelcomeBanner();

    // Tài khoản admin cứng (trong thực tế nên đọc từ file có hash)
    const std::string ADMIN_USER = "admin";
    const std::string ADMIN_PASS = "admin123";

    std::cout << Color::YELLOW << "  Chon chuc nang:\n" << Color::RESET;
    std::cout << "  [1] Dang nhap\n";
    std::cout << "  [0] Thoat chuong trinh\n\n";

    int choice = getValidInt("  > Lua chon", 0, 1);
    if (choice == 0) return LoginResult::Exit;

    std::string username = getValidString("\n  Ten dang nhap", 30);
    std::cout << "  Mat khau: ";
    std::string password;
    std::getline(std::cin, password);

    // Kiểm tra admin
    if (username == ADMIN_USER && password == ADMIN_PASS) {
        session.username = username;
        session.isAdmin  = true;
        std::cout << Color::GREEN << "\n  Dang nhap thanh cong! Xin chao, "
                  << username << " (Admin)\n" << Color::RESET;
        pauseScreen();
        return LoginResult::Success;
    }

    // Kiểm tra sinh viên (username bất kỳ, mật khẩu = "sv" + username)
    // Đây là logic đơn giản – thực tế có thể đọc từ file users.txt
    std::string expectedPass = "sv" + username;
    if (!username.empty() && password == expectedPass) {
        session.username = username;
        session.isAdmin  = false;
        std::cout << Color::GREEN << "\n  Dang nhap thanh cong! Xin chao, "
                  << username << "\n" << Color::RESET;
        pauseScreen();
        return LoginResult::Success;
    }

    std::cout << Color::RED
              << "\n  [LOI] Ten dang nhap hoac mat khau khong dung!\n"
              << Color::RESET;
    pauseScreen();
    return LoginResult::InvalidCredentials;
}

// ────────────────────────────────────────────────────────────
//  Menu Admin
// ────────────────────────────────────────────────────────────

void showAdminMenu(LoginSession& session) {
    QuestionBank qBank;
    qBank.loadFromFile("data/questions.txt");

    HistoryManager histMgr;

    bool running = true;
    while (running) {
        clearScreen();
        std::cout << Color::CYAN << Color::BOLD;
        printLine('=');
        printCentered("MENU QUAN TRI VIEN  |  " + session.username);
        printLine('=');
        std::cout << Color::RESET << "\n";

        std::cout << "  " << Color::YELLOW << "[1]" << Color::RESET
                  << " Xem danh sach cau hoi\n";
        std::cout << "  " << Color::YELLOW << "[2]" << Color::RESET
                  << " Them cau hoi moi\n";
        std::cout << "  " << Color::YELLOW << "[3]" << Color::RESET
                  << " Xoa cau hoi\n";
        std::cout << "  " << Color::YELLOW << "[4]" << Color::RESET
                  << " Xem lich su tat ca bai thi\n";
        std::cout << "  " << Color::YELLOW << "[5]" << Color::RESET
                  << " Thong ke ket qua\n";
        std::cout << "  " << Color::RED    << "[0]" << Color::RESET
                  << " Dang xuat\n\n";

        int choice = getValidInt("  > Lua chon", 0, 5);

        switch (choice) {
            case 1: {
                clearScreen();
                std::cout << Color::BOLD << "\n  DANH SACH CAU HOI:\n"
                          << Color::RESET;
                printLine('-');
                qBank.printAll();
                printLine('-');
                pauseScreen();
                break;
            }
            case 2: {
                clearScreen();
                std::cout << Color::BOLD << "\n  THEM CAU HOI MOI:\n"
                          << Color::RESET;
                printLine('-');

                Question q;
                q.id      = qBank.getNextId();
                q.content = getValidString("  Noi dung cau hoi", 200);
                q.optA    = getValidString("  Dap an A", 100);
                q.optB    = getValidString("  Dap an B", 100);
                q.optC    = getValidString("  Dap an C", 100);
                q.optD    = getValidString("  Dap an D", 100);

                std::cout << "  Dap an dung (A/B/C/D): ";
                std::string ans;
                std::getline(std::cin, ans);
                // Validate
                while (ans != "A" && ans != "B" && ans != "C" && ans != "D" &&
                       ans != "a" && ans != "b" && ans != "c" && ans != "d") {
                    std::cout << Color::RED
                              << "  [LOI] Chi nhap A, B, C hoac D: "
                              << Color::RESET;
                    std::getline(std::cin, ans);
                }
                q.correctAns = ans[0];
                if (q.correctAns >= 'a') q.correctAns -= 32; // uppercase

                qBank.addQuestion(q);
                qBank.saveToFile("data/questions.txt");

                std::cout << Color::GREEN
                          << "\n  [OK] Da them cau hoi ID=" << q.id << " thanh cong!\n"
                          << Color::RESET;
                pauseScreen();
                break;
            }
            case 3: {
                clearScreen();
                std::cout << Color::BOLD << "\n  XOA CAU HOI:\n" << Color::RESET;
                printLine('-');
                qBank.printAll();
                printLine('-');

                int delId = getValidInt("  Nhap ID cau hoi can xoa (0 = huy)", 0, 9999);
                if (delId != 0) {
                    if (getYesNo("  Ban chac chan muon xoa?")) {
                        if (qBank.removeQuestion(delId)) {
                            qBank.saveToFile("data/questions.txt");
                            std::cout << Color::GREEN
                                      << "  [OK] Da xoa cau hoi ID=" << delId << "\n"
                                      << Color::RESET;
                        } else {
                            std::cout << Color::RED
                                      << "  [LOI] Khong tim thay ID=" << delId << "\n"
                                      << Color::RESET;
                        }
                    } else {
                        std::cout << "  Da huy.\n";
                    }
                }
                pauseScreen();
                break;
            }
            case 4: {
                clearScreen();
                std::cout << Color::BOLD << "\n  LICH SU TAT CA BAI THI:\n"
                          << Color::RESET;
                printLine('-');
                histMgr.loadFromFile("data/history.txt");
                histMgr.printAll();
                printLine('-');
                pauseScreen();
                break;
            }
            case 5: {
                clearScreen();
                std::cout << Color::BOLD << "\n  THONG KE KET QUA:\n"
                          << Color::RESET;
                printLine('-');
                histMgr.loadFromFile("data/history.txt");
                histMgr.printStats();
                printLine('-');
                pauseScreen();
                break;
            }
            case 0:
                running = false;
                break;
        }
    }
}

// ────────────────────────────────────────────────────────────
//  Menu Sinh viên
// ────────────────────────────────────────────────────────────

void showStudentMenu(LoginSession& session) {
    QuestionBank qBank;
    qBank.loadFromFile("data/questions.txt");

    HistoryManager histMgr;

    bool running = true;
    while (running) {
        clearScreen();
        std::cout << Color::CYAN << Color::BOLD;
        printLine('=');
        printCentered("MENU SINH VIEN  |  " + session.username);
        printLine('=');
        std::cout << Color::RESET << "\n";

        std::cout << "  " << Color::YELLOW << "[1]" << Color::RESET
                  << " Bat dau lam bai thi\n";
        std::cout << "  " << Color::YELLOW << "[2]" << Color::RESET
                  << " Xem ket qua cua toi\n";
        std::cout << "  " << Color::RED    << "[0]" << Color::RESET
                  << " Dang xuat\n\n";

        int choice = getValidInt("  > Lua chon", 0, 2);

        switch (choice) {
            case 1: {
                // Kiểm tra đủ câu hỏi chưa
                if (qBank.getCount() < 5) {
                    std::cout << Color::RED
                              << "\n  [LOI] Ngan hang cau hoi chua du (can it nhat 5 cau)!\n"
                              << Color::RESET;
                    pauseScreen();
                    break;
                }

                clearScreen();
                std::cout << Color::BOLD << "\n  THIET LAP BAI THI:\n"
                          << Color::RESET;
                printLine('-');

                int numQ = getValidInt(
                    "  So cau hoi (toi da " + std::to_string(qBank.getCount()) + ")",
                    1, qBank.getCount()
                );
                int timeSec = getValidInt("  Thoi gian lam bai (phut)", 1, 120) * 60;

                std::cout << Color::YELLOW
                          << "\n  San sang lam bai? (" << numQ << " cau hoi, thoi gian "
                          << timeSec / 60 << " phut).\n" << Color::RESET;

                if (getYesNo("  Bat dau lam bai?")) {
                    // Khởi tạo engine và chạy bài thi
                    ExamEngine engine(qBank, session.username, numQ, timeSec);
                    TestRecord record = engine.run();

                    // Lưu kết quả
                    histMgr.loadFromFile("data/history.txt");
                    histMgr.addRecord(record);
                    histMgr.saveToFile("data/history.txt");

                    // Hiển thị kết quả
                    clearScreen();
                    std::cout << Color::BOLD << "\n  KET QUA BAI THI:\n"
                              << Color::RESET;
                    printLine('=');
                    std::cout << "  Thi sinh  : " << record.username << "\n";
                    std::cout << "  So cau    : " << record.totalQuestions << "\n";
                    std::cout << "  So dung   : " << Color::GREEN
                              << record.correctCount << Color::RESET << "\n";
                    std::cout << "  So sai    : " << Color::RED
                              << (record.totalQuestions - record.correctCount)
                              << Color::RESET << "\n";

                    double score = (record.totalQuestions > 0)
                        ? (10.0 * record.correctCount / record.totalQuestions)
                        : 0.0;
                    std::cout << "  Diem so   : " << Color::CYAN
                              << Color::BOLD;
                    std::cout.precision(1);
                    std::cout << std::fixed << score;
                    std::cout << " / 10" << Color::RESET << "\n";
                    printLine('=');
                }
                pauseScreen();
                break;
            }
            case 2: {
                clearScreen();
                std::cout << Color::BOLD << "\n  KET QUA BAI LAM  |  " 
                          << session.username << ":\n" << Color::RESET;
                printLine('-');
                histMgr.loadFromFile("data/history.txt");
                histMgr.printByUser(session.username);
                printLine('-');
                pauseScreen();
                break;
            }
            case 0:
                running = false;
                break;
        }
    }
}

// ────────────────────────────────────────────────────────────
//  Input Validation
// ────────────────────────────────────────────────────────────

int getValidInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    std::string raw;
    while (true) {
        std::cout << prompt << " [" << minVal << "-" << maxVal << "]: ";
        std::getline(std::cin, raw);

        // Bắt chuỗi rỗng
        if (raw.empty()) {
            std::cout << Color::RED
                      << "  [LOI] Khong duoc bo trong. Vui long nhap lai.\n"
                      << Color::RESET;
            continue;
        }

        // Bắt ký tự không phải số (cho phép dấu trừ ở đầu)
        bool valid = true;
        for (size_t i = 0; i < raw.size(); ++i) {
            if (i == 0 && raw[0] == '-') continue;
            if (raw[i] < '0' || raw[i] > '9') { valid = false; break; }
        }

        if (!valid) {
            std::cout << Color::RED
                      << "  [LOI] Vui long chi nhap so nguyen.\n"
                      << Color::RESET;
            continue;
        }

        // Bắt tràn số (stoi ném exception)
        try {
            value = std::stoi(raw);
        } catch (...) {
            std::cout << Color::RED
                      << "  [LOI] Gia tri vuot qua gioi han cho phep.\n"
                      << Color::RESET;
            continue;
        }

        if (value < minVal || value > maxVal) {
            std::cout << Color::RED << "  [LOI] Vui long nhap trong khoang ["
                      << minVal << ", " << maxVal << "].\n" << Color::RESET;
            continue;
        }

        return value;
    }
}

std::string getValidString(const std::string& prompt, int maxLen) {
    std::string input;
    while (true) {
        std::cout << prompt << ": ";
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << Color::RED
                      << "  [LOI] Khong duoc de trong.\n" << Color::RESET;
            continue;
        }

        if (static_cast<int>(input.size()) > maxLen) {
            std::cout << Color::RED << "  [LOI] Toi da " << maxLen
                      << " ky tu.\n" << Color::RESET;
            continue;
        }

        return input;
    }
}

bool getYesNo(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (Y/N): ";
        std::getline(std::cin, input);
        if (input == "Y" || input == "y") return true;
        if (input == "N" || input == "n") return false;
        std::cout << Color::RED
                  << "  [LOI] Chi nhap Y hoac N.\n" << Color::RESET;
    }
}
