#pragma once
#include <string>

// ============================================================
//  Menu.h  –  Khai báo các hàm vẽ giao diện và điều hướng
//  Tác giả: THÀNH  |  Role: System Architect / UI / QA
//  Quy tắc: camelCase cho hàm, PascalCase cho struct/class
// ============================================================

// ── Thông tin người dùng đang đăng nhập ─────────────────────
struct LoginSession {
    std::string username;
    bool        isAdmin;
};

// ── Kết quả đăng nhập ───────────────────────────────────────
enum class LoginResult {
    Success,
    InvalidCredentials,
    Exit
};

// ── Các mã màu ANSI cho Terminal ────────────────────────────
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
    const std::string BG_BLUE = "\033[44m";
}

// ── Hàm tiện ích Terminal ────────────────────────────────────
void clearScreen();
void pauseScreen();
void printLine(char c = '-', int width = 60);
void printCentered(const std::string& text, int width = 60);

// ── Màn hình chào mừng ──────────────────────────────────────
void showWelcomeBanner();

// ── Đăng nhập / Đăng xuất ───────────────────────────────────
LoginResult showLoginScreen(LoginSession& session);

// ── Menu chính Admin ────────────────────────────────────────
void showAdminMenu(LoginSession& session);

// ── Menu chính Sinh viên ────────────────────────────────────
void showStudentMenu(LoginSession& session);

// ── Input Validation ────────────────────────────────────────
// Đọc số nguyên trong đoạn [min, max], bẫy lỗi hoàn toàn
int  getValidInt(const std::string& prompt, int minVal, int maxVal);

// Đọc chuỗi không rỗng
std::string getValidString(const std::string& prompt, int maxLen = 50);

// Đọc lựa chọn Y/N
bool getYesNo(const std::string& prompt);