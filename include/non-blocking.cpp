// timer_poc.cpp — Proof of Concept đếm giờ non-blocking
// Biên dịch Windows (MSVC/MinGW): g++ timer_poc.cpp -o timer_poc
#include <iostream>
#include <ctime>
#include <cstring>
#include <conio.h>   // kbhit(), getch() — Windows only
using namespace std;

int main() {
    const int TIME_LIMIT = 15;         // giây
    char  inputBuf[64] = {};
    int   len       = 0;
    bool  timedOut  = false;

    cout << "Cau hoi: Thu do cua Viet Nam la gi?\n";
    cout << "Tra loi: ";
    cout.flush();

    time_t startTime = time(0);     // <-- mốc thời gian bắt đầu

    while (true) {
        time_t elapsed   = time(0) - startTime;
        int    remaining = TIME_LIMIT - (int)elapsed;

        // Cập nhật dòng timer — ghi đè tại chỗ bằng ANSI
        cout << "\033[s";             // lưu vị trí con trỏ
        cout << "\033[1;60H";        // di chuyển đến cột 60 dòng 1
        cout << "[Con lai: " << remaining << "s]  ";
        cout << "\033[u";             // khôi phục vị trí con trỏ
        cout.flush();

        if (elapsed >= TIME_LIMIT) {
            timedOut = true;
            break;
        }

        if (kbhit()) {               // có phím được nhấn?
            char ch = (char)getch();
            if (ch == '\r' || ch == '\n') break;   // Enter
            if (ch == 8 && len > 0) {               // Backspace
                inputBuf[--len] = '\0';
                cout << "\b \b";
            } else if (len < 63) {inputBuf[len++] = ch;
                cout << ch;
            }
            cout.flush();
        }

        // Nghỉ ngắn để tránh busy-wait (tiêu tốn CPU)
        _sleep(100);                  // 100ms — ctime không có sleep
    }

    cout << "\n";
    if (timedOut)
        cout << "[HET GIO] Tu dong nop bai!\n";
    else
        cout << "Dap an: " << inputBuf << "\n";
  return 0;
}
