# HỆ THỐNG THI TRẮC NGHIỆM C++

## Cấu trúc thư mục

```text
KY_THUAT_LAP_TRINH/
├── include/        ← Header files (interface)
│   ├── QuestionBank.h
│   ├── Exam.h
│   ├── History.h
│   └── Menu.h
├── src/            ← Source files (implementation)
│   ├── QuestionBank.cpp 
│   ├── Exam.cpp          
│   ├── History.cpp       
│   ├── Menu.cpp          
│   └── main.cpp          
├── data/
│   ├── questions.txt
│   └── history.txt
└── README.md
```

## Biên dịch (MinGW / g++)

Chạy từ thư mục gốc `KY_THUAT_LAP_TRINH/`:

```bash
g++ -std=c++17 -Wall -Wextra -I./include \
    src/main.cpp src/Menu.cpp src/QuestionBank.cpp \
    src/Exam.cpp src/History.cpp \
    -o KY_THUAT_LAP_TRINH
```

Chạy chương trình:

```bash
./KY_THUAT_LAP_TRINH        # Linux/Mac
KY_THUAT_LAP_TRINH.exe      # Windows
```

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
Username|Subject|SoCauDung|TongSoCau|Diem|ThoiGianLam|Timestamp
```

## Phân công

| Module | File | Thành viên |
| -------- | ------ | ----------- |
| Giao diện & Main | Menu.h/cpp, main.cpp | **Thành** |
| Thuật toán thi | Exam.h/cpp | **Hùng** |
| Dữ liệu & File I/O | QuestionBank.h/cpp, History.h/cpp | **Vân** |

## Lưu ý kỹ thuật

- Tất cả dữ liệu truyền qua tham số hàm, **không dùng biến toàn cục**
- Mọi `new` phải có `delete` tương ứng trong Destructor
- Dùng `#pragma once` ở đầu tất cả file `.h`
- Chỉ push code khi đã compile **0 error** trên máy cá nhân
