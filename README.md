# XÂY DỰNG CHƯƠNG TRÌNH HỖ TRỢ THI TRẮC NGHIỆM ĐƠN GIẢN

## Chức năng chính

### Sinh viên

- Đăng nhập hệ thống
- Làm bài thi trắc nghiệm
- Chọn số lượng câu hỏi và thời gian làm bài
- Xáo trộn ngẫu nhiên câu hỏi và đáp án
- Nộp bài sớm hoặc tự động nộp khi hết giờ
- Xem lịch sử các lần thi của bản thân

### Quản trị viên

- Đăng nhập với quyền Admin
- Xem danh sách câu hỏi
- Thêm câu hỏi mới
- Xóa câu hỏi theo ID
- Xem lịch sử thi của tất cả sinh viên

## Thuật toán và cấu trúc dữ liệu

- Danh sách liên kết đơn để quản lý ngân hàng câu hỏi
- Mảng động để lưu đề thi và đáp án người dùng
- Thuật toán Fisher–Yates Shuffle để:
  - Xáo trộn câu hỏi
  - Xáo trộn đáp án
- Đọc/Ghi dữ liệu bằng File I/O (fstream)
- Quản lý bộ nhớ động bằng Constructor và Destructor

## Lưu ý kỹ thuật

- Tất cả dữ liệu truyền qua tham số hàm, **không dùng biến toàn cục**
- Mọi `new` phải có `delete` tương ứng trong Destructor
- Dùng `#pragma once` ở đầu tất cả file `.h`
- Chỉ push code khi đã compile **0 error** trên máy cá nhân

## Cấu trúc thư mục

```text
KY_THUAT_LAP_TRINH/
├── include/
│   ├── QuestionBank.h
│   ├── Exam.h
│   ├── History.h
│   └── Menu.h
├── src/
│   ├── main.cpp
│   ├── Menu.cpp
│   ├── Exam.cpp
│   ├── History.cpp
│   └── QuestionBank.cpp
├── data/
│   ├── questions.txt
│   └── history.txt
├── .vscode/
│   └── tasks.json
├── quiz_program.exe ← được tạo sau khi biên dịch
└── README.md
```

## Biên dịch và chạy chương trình

### Biên dịch thủ công bằng g++ (MSYS2 MinGW UCRT64)

Chạy từ thư mục gốc `KY_THUAT_LAP_TRINH/`:

```bash
C:/msys64/ucrt64/bin/g++.exe ^
-std=c++17 ^
-g ^
-Wall ^
-I./include ^
src/main.cpp ^
src/Menu.cpp ^
src/Exam.cpp ^
src/History.cpp ^
src/QuestionBank.cpp ^
-o quiz_program.exe
```

### Chạy chương trình

```powershell
.\quiz_program.exe
```

### Sử dụng VS Code

Dự án đã được cấu hình sẵn trong `.vscode/tasks.json`.

- **Ctrl + Shift + B** → Build Quiz Program
- **Run Task** → Run Quiz Program

Sau khi biên dịch thành công, file thực thi `quiz_program.exe` sẽ được tạo tại thư mục gốc của dự án.

## Tài khoản mặc định

| Vai trò    | Username  | Password      |
|------------|-----------|---------------|
| Admin      | admin     | admin123      |
| Sinh viên  | (bất kỳ)  | sv + username |

**Ví dụ:** username = `hung`, password = `svhung`

## Định dạng file dữ liệu

**questions.txt:**

```text
ID|NoiDung|DapAnA|DapAnB|DapAnC|DapAnD|DapAnDung
```

**history.txt:**

```text
Username|SoCauDung|TongSoCau|Diem|Timestamp
```

## Phân công

| Module | File | Thành viên |
| -------- | ------ | ----------- |
| Giao diện & Main | Menu.h/cpp, main.cpp | **Thành** |
| Thuật toán thi | Exam.h/cpp | **Hùng** |
| Dữ liệu & File I/O | QuestionBank.h/cpp, History.h/cpp | **Vân** |
