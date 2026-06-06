// ============================================================
//  main.cpp  –  Entry point  |  Nối các module lại với nhau
//  Tác giả: THÀNH  |  Role: System Architect / QA
//
//  LUỒNG CHÍNH:
//    main()
//      └─ showWelcomeBanner()
//      └─ showLoginScreen()   ← lặp cho đến khi đăng nhập OK
//          └─ [Admin]   → showAdminMenu()
//          └─ [Student] → showStudentMenu()
// ============================================================

#include "Menu.h"
#include <iostream>

int main() {
    // Vòng lặp chính: cho phép đăng nhập lại nếu sai thông tin
    bool appRunning = true;
    while (appRunning) {
        LoginSession session;
        LoginResult result = showLoginScreen(session);

        switch (result) {
            case LoginResult::Success:
                if (session.isAdmin) {
                    showAdminMenu(session);
                } else {
                    showStudentMenu(session);
                }
                // Sau khi đăng xuất → quay lại màn hình đăng nhập
                break;

            case LoginResult::InvalidCredentials:
                // showLoginScreen đã in thông báo lỗi, tiếp tục vòng lặp
                break;

            case LoginResult::Exit:
                appRunning = false;
                break;
        }
    }

    // Màn hình tạm biệt
    clearScreen();
    std::cout << Color::CYAN << Color::BOLD;
    printLine('=');
    printCentered("Chuc mung ban hoan thanh bai kiem tra!");
    printLine('=');
    std::cout << Color::RESET << "\n";

    return 0;
}
