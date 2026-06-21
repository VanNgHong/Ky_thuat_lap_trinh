# XÂY DỰNG CHƯƠNG TRÌNH HỖ TRỢ THI TRẮC NGHIỆM ĐƠN GIẢN

Chương trình mô phỏng một hệ thống thi trắc nghiệm chạy trên Terminal, hỗ trợ quản lý ngân hàng câu hỏi theo môn học/độ khó, sinh đề tự động theo tỉ lệ độ khó cố định, làm bài có đếm giờ thời gian thực và lưu trữ lịch sử thi.

> Xem chi tiết phân tích, thiết kế và kiểm thử đầy đủ trong báo cáo (`main.tex` / `main.pdf`).

---

## Chức năng chính

### Sinh viên

- Đăng nhập hệ thống (tài khoản tự do, mật khẩu theo quy ước `sv` + username)
- Chọn **môn học** muốn thi trong số các môn hiện có trong ngân hàng câu hỏi (ví dụ: `TA10`, `TA11`, `TA12`)
- Nhập số lượng câu hỏi muốn thi — giới hạn trong khoảng `[1, N_max]`, với `N_max` được hệ thống tự tính dựa trên số câu Easy/Medium/Hard hiện có của môn đã chọn (xem mục *Tỉ lệ độ khó* bên dưới)
- Nhập thời gian làm bài (giây), trong khoảng `[10, 36000]`
- Xác nhận (Y/N) trước khi đề thi được sinh ra
- Làm bài tuần tự với câu hỏi và đáp án đã được xáo trộn ngẫu nhiên (Fisher–Yates)
- Nộp bài sớm bất kỳ lúc nào bằng cách nhập `S`, hoặc tự động bị thu bài khi hết giờ
- Xem kết quả chi tiết ngay sau khi nộp bài: từng câu **[DUNG]/[SAI]/[CHUA TRA LOI]**, tổng số câu đúng, điểm theo thang 10
- Xem lại lịch sử các lần thi của riêng bản thân (lọc theo tên đăng nhập)

### Quản trị viên

- Đăng nhập với tài khoản cố định `admin` / `admin123`
- Xem danh sách câu hỏi: liệt kê theo **một môn học cụ thể** (hệ thống gợi ý sẵn danh sách các môn hiện có) hoặc nhập `ALL` để xem toàn bộ
- Thêm câu hỏi mới: nhập ID (kiểm tra trùng), môn học, độ khó (`Easy`/`Medium`/`Hard`), nội dung, 4 đáp án và đáp án đúng — dữ liệu được ghi đè ngay xuống `questions.txt` sau khi thêm thành công
- Xóa câu hỏi theo ID — xử lý đầy đủ các trường hợp: danh sách rỗng, xóa node đầu, xóa giữa/cuối, không tìm thấy ID — đồng bộ lại file ngay sau khi xóa
- Xem lịch sử thi của **toàn bộ** sinh viên (không lọc theo người dùng)

> *Lưu ý:* phiên bản hiện tại **chưa hỗ trợ sửa câu hỏi đã có** — để cập nhật nội dung, Admin cần xóa rồi thêm lại với cùng ID. Đây là hạn chế đã ghi nhận, được đề xuất bổ sung ở phần hướng phát triển trong báo cáo.

---

## Sinh đề theo tỉ lệ độ khó 40-40-20

Thay vì bốc ngẫu nhiên hoàn toàn, mỗi đề thi được sinh đúng theo tỉ lệ cố định:

| Mức độ | Tỉ lệ | Công thức số câu |
|---|---|---|
| Easy | 40% | `N × 40 / 100` (làm tròn xuống) |
| Medium | 40% | `N × 40 / 100` (làm tròn xuống) |
| Hard | 20% | phần còn lại: `N - easyNeed - mediumNeed` |

Quy trình sinh đề (`Exam::generateExamFromBank`):

1. Trích các câu hỏi của môn đã chọn theo từng mức độ từ `QuestionBank`.
2. Nếu ngân hàng không đủ số câu cần ở **bất kỳ** mức độ nào → từ chối sinh đề, báo lỗi rõ ràng, **không** sinh đề thiếu câu hay sai tỉ lệ.
3. Xáo trộn (Fisher–Yates) riêng từng nhóm Easy/Medium/Hard trước khi cắt lấy đúng số câu cần ở mỗi nhóm — đảm bảo các lần thi khác nhau (cùng tỉ lệ) vẫn có tổ hợp câu hỏi khác nhau nếu ngân hàng dư câu.
4. Trộn tiếp toàn bộ `N` câu đã ghép để tránh đề luôn hiển thị theo thứ tự Easy → Medium → Hard.
5. Với mỗi câu hỏi, xáo trộn lại 4 đáp án và cập nhật lại vị trí đáp án đúng theo nội dung (không theo chỉ số cũ).

Hàm `getMaxNumberQuest(nEasy, nMedium, nHard)` (trong `Menu.cpp`) duyệt `N` giảm dần để tìm số câu tối đa hợp lệ thỏa tỉ lệ trên, dùng để giới hạn lựa chọn số câu của sinh viên.

---

## Đếm giờ và thu bài tự động

- Dùng đồng hồ hệ thống (`time(nullptr)`, `difftime`) thay vì đếm vòng lặp, để không bị lệch khi người dùng dừng lại suy nghĩ lâu.
- Thời gian còn lại được kiểm tra **hai lần** mỗi câu: ngay trước khi hiển thị câu hỏi, và ngay sau khi người dùng nhập xong đáp án — tránh tình huống "lách" giới hạn thời gian ở câu cuối cùng.
- Câu hỏi chưa kịp trả lời khi hết giờ/nộp sớm được ghi nhận ở trạng thái riêng `[CHUA TRA LOI]`, tách biệt với `[SAI]`.

---

## Thuật toán và cấu trúc dữ liệu

- **Danh sách liên kết đơn** (tự cài đặt bằng con trỏ, không dùng STL) để quản lý ngân hàng câu hỏi (`QuestionBank`) và lịch sử thi (`HistoryManager`), dùng cặp con trỏ `head`/`tail` để thêm vào cuối với độ phức tạp `O(1)`.
- **Mảng động** (`new[]` / `delete[]`) để lưu danh sách câu hỏi của một đề thi cụ thể (`Question* questionList`) và đáp án người dùng đã chọn (`char* userAnswers`) trong class `Exam`.
- **Thuật toán Fisher–Yates Shuffle** dùng ở hai cấp độ: xáo trộn thứ tự câu hỏi trong đề, và xáo trộn thứ tự 4 đáp án của từng câu (kèm cập nhật lại đáp án đúng).
- **File I/O** (`<fstream>`, `<sstream>`) đọc/ghi dữ liệu dạng văn bản phẳng, phân tách bằng ký tự `|`, có bẫy lỗi theo từng dòng (dòng hỏng bị bỏ qua, không làm crash chương trình).
- **Quản lý bộ nhớ động** triệt để bằng Constructor/Destructor (nguyên tắc RAII) — đã kiểm tra bằng Valgrind, không phát hiện rò rỉ bộ nhớ (`definitely lost: 0 bytes`).

---

## Lưu ý kỹ thuật

- Toàn bộ chương trình dùng chuẩn **C++17**, **không sử dụng container STL** (`std::vector`, `std::list`,...) — tự cài đặt danh sách liên kết đơn bằng con trỏ theo đúng yêu cầu môn học.
- Tất cả dữ liệu truyền qua tham số hàm hoặc thành viên class, **không dùng biến toàn cục**.
- Mọi `new`/`new[]` đều có `delete`/`delete[]` tương ứng, được giải phóng trong Destructor hoặc ngay sau khi sử dụng (kể cả ở các nhánh lỗi/early-return của `generateExamFromBank`).
- Dùng `#pragma once` hoặc include guard (`#ifndef`) ở đầu tất cả file `.h`.
- Các hàm nhập liệu (`getValidInt`, `getValidString`, `getYesNo`) bẫy lỗi hoàn toàn (`cin.clear()` + `cin.ignore()`), không bao giờ làm crash chương trình do người dùng nhập sai, chỉ thoát có kiểm soát khi gặp EOF.
- Chỉ push code khi đã compile **0 lỗi** trên máy cá nhân.

---

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
├── report/
│   └── main.tex            (báo cáo LaTeX đầy đủ)
├── .vscode/
│   └── tasks.json
├── quiz_program.exe        ← được tạo sau khi biên dịch
└── README.md
```

---

## Biên dịch và chạy chương trình

### Biên dịch thủ công bằng g++ (MSYS2 MinGW UCRT64)

Chạy từ thư mục gốc `KY_THUAT_LAP_TRINH/`:

```bash
g++ -std=c++17 -g -Wall -I./include ^
    src/main.cpp src/Menu.cpp src/Exam.cpp ^
    src/History.cpp src/QuestionBank.cpp ^
    -o quiz_program.exe
```

(Trên Linux/macOS, dùng dấu `\` thay cho `^` để nối dòng.)

### Chạy chương trình

```powershell
.\quiz_program.exe
```

> Chương trình cần được chạy **từ đúng thư mục gốc** của dự án để đường dẫn tương đối tới `data/questions.txt` và `data/history.txt` hoạt động chính xác.

### Sử dụng VS Code

Dự án đã được cấu hình sẵn trong `.vscode/tasks.json` (2 task: `Build Quiz Program` và `Run Quiz Program`).

- **Ctrl + Shift + B** → Build Quiz Program
- Chạy task **Run Quiz Program** để build (nếu cần) rồi chạy chương trình ngay sau đó.

---

## Tài khoản mặc định

| Vai trò    | Username  | Password        |
|------------|-----------|-----------------|
| Admin      | `admin`   | `admin123`      |
| Sinh viên  | (bất kỳ)  | `sv` + username |

**Ví dụ:** username = `hung`, password = `svhung`.

Tại màn hình đăng nhập, gõ `exit` vào ô tên đăng nhập để thoát chương trình một cách an toàn.

> Cơ chế xác thực hiện tại không mã hóa mật khẩu, chỉ phục vụ mục đích minh họa học thuật trong phạm vi bài tập lớn.

---

## Định dạng file dữ liệu

### `questions.txt` — 9 trường, phân tách bằng `|`

```text
id|subject|difficulty|content|DapAnA|DapAnB|DapAnC|DapAnD|DapAnDung
```

| Thứ tự | Trường | Ý nghĩa |
|---|---|---|
| 1 | `id` | Số nguyên dương, duy nhất trong toàn ngân hàng |
| 2 | `subject` | Tên môn học (ví dụ `TA10`, `TA11`, `TA12`) |
| 3 | `difficulty` | `Easy` / `Medium` / `Hard` |
| 4 | `content` | Nội dung câu hỏi |
| 5–8 | `answers[0..3]` | Bốn đáp án tương ứng A, B, C, D |
| 9 | `correct` | Ký tự đáp án đúng (`A`/`B`/`C`/`D`) |

Ví dụ một dòng hợp lệ:

```text
12|TA10|Medium|Choose the correct form: She ___ to school.|go|goes|going|went|B
```

Dòng trống hoặc dòng bắt đầu bằng `#` được bỏ qua an toàn khi đọc file. Khi ghi (`saveToFile`), file được **ghi đè toàn bộ** để phản ánh đúng trạng thái ngân hàng hiện tại.

### `history.txt` — 6 trường, phân tách bằng `|`

```text
studentName|subject|correctCount|totalCount|score|datetime
```

| Thứ tự | Trường | Ý nghĩa |
|---|---|---|
| 1 | `studentName` | Tên đăng nhập của sinh viên |
| 2 | `subject` | Môn học đã thi |
| 3 | `correctCount` | Số câu trả lời đúng |
| 4 | `totalCount` | Tổng số câu của đề thi |
| 5 | `score` | Điểm trên thang 10 (`double`) |
| 6 | `datetime` | Thời điểm nộp bài, dạng `YYYY-MM-DD HH:MM:SS` |

Ví dụ một dòng hợp lệ:

```text
hung|TA10|7|10|7|2026-06-15 21:40:12
```

Khi ghi (`saveToFile`), file được mở ở chế độ **append** (`ios::app`) — kết quả mỗi lần thi được **nối thêm**, không ghi đè lịch sử cũ của các sinh viên khác.

---

## Phân công

| Module | File | Thành viên | Vai trò |
| -------- | ------ | ----------- | ----------- |
| Giao diện & Main | `Menu.h/cpp`, `main.cpp` | **Nguyễn Đức Thành** | System Architect / UI / QA |
| Thuật toán thi | `Exam.h/cpp` | **Nguyễn Mạnh Hùng** | Algorithm Engineer |
| Dữ liệu & File I/O | `QuestionBank.h/cpp`, `History.h/cpp` | **Nguyễn Hồng Vân** | Data Engineer |

Phối hợp qua **GitHub** (quản lý mã nguồn theo nhánh) và **ClickUp** (theo dõi tiến độ). Mỗi thành viên chỉ đẩy code khi đã biên dịch thành công với **0 lỗi** trên máy cá nhân.