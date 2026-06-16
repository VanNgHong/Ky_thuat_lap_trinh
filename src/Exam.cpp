#include "Exam.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <cctype>
using namespace std;

// CÁC HÀM CƠ BẢN CỦA CLASS EXAM
// Constructor: Cấp phát động mảng Question[N]
Exam::Exam(int n, int t) {
    numberOfQuestions = n;
    timeLimitSec = t;
    questionList = new Question[n];
}

// Destructor: Giải phóng bộ nhớ động
Exam::~Exam() {
    delete[] questionList;
}

// Hàm nạp câu hỏi từ QuestionBank vào Exam 
void Exam::generateExamFromBank(QuestionBank& bank) {
    int bankCount = bank.getQuestionCount();
    if (bankCount < numberOfQuestions) {
        std::cout << "Loi: Ngan hang khong du " << numberOfQuestions << " cau hoi!\n";
        return;
    }

    int* indices = new int[bankCount];
    for (int i = 0; i < bankCount; ++i) indices[i] = i;

    for (int i = bankCount - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    for (int i = 0; i < numberOfQuestions; ++i) {
        Question* bankQuestion = bank.getQuestionAt(indices[i]);
        if (bankQuestion != nullptr) {
            questionList[i] = *bankQuestion;
        }
    }
    delete[] indices;
}

// Xáo trộn mảng chuỗi (Áp dụng cho mảng answers[4])
// Tương ứng: PROCEDURE Shuffle(arr[], n) dành cho đáp án
void Exam::shuffleAnswers(string arr[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        string temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Xáo trộn mảng câu hỏi (Áp dụng cho mảng questionList)
// Tương ứng: PROCEDURE Shuffle(arr[], n) dành cho câu hỏi

void Exam::shuffleQuestions(Question arr[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        Question temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

//mới thêm 16/06
void Exam::loadFromBank(Question* questions) {
    for (int i = 0; i < numberOfQuestions; i++)
        questionList[i] = questions[i];
}
// Hàm chính áp dụng logic trộn đề thi
// Tương ứng: PROCEDURE ShuffleExam(questions[], qCount)
void Exam::shuffleExam() {
    
    // 1. Xáo thứ tự câu hỏi: Shuffle(questions, qCount)
    shuffleQuestions(questionList, numberOfQuestions);

    // 2. FOR EACH q IN questions DO
    for (int i = 0; i < numberOfQuestions; ++i) {
        Question& q = questionList[i]; // Lấy câu hỏi hiện tại
// Vì struct Question dùng ký tự 'A','B','C','D' để đánh dấu đáp án đúng, trước khi xáo ta cần biết nội dung chuỗi (text) chính xác của đáp án đúng là gì.
        int originalCorrectIndex = q.correct - 'A'; 
        string correctText = q.answers[originalCorrectIndex];

        // Xáo đáp án riêng từng câu: Shuffle(q.answers, q.answerCount)
        // Với câu hỏi trắc nghiệm cố định là 4 đáp án
        shuffleAnswers(q.answers, 4);

        // Cập nhật lại correctIndex sau khi xáo
        for (int k = 0; k < 4; ++k) {
            if (q.answers[k] == correctText) {
                q.correct = static_cast<char>('A' + k);
                break; 
            }
        }
    }
}

// Hàm in đề thi để kiểm tra
void Exam::printExam() const {
    for (int i = 0; i < numberOfQuestions; ++i) {
        std::cout << "Cau hoi " << i + 1 << " (ID: " << questionList[i].id << "): " << questionList[i].content << "\n";
        std::cout << "  A. " << questionList[i].answers[0] << "\n";
        std::cout << "  B. " << questionList[i].answers[1] << "\n";
        std::cout << "  C. " << questionList[i].answers[2] << "\n";
        std::cout << "  D. " << questionList[i].answers[3] << "\n";
        std::cout << "=> Dap an dung: " << questionList[i].correct << "\n\n";
    }
}

// Chạy toàn bộ bài thi: hiển thị từng câu, nhận đáp án, đếm giờ ngược,
// tự động thu bài khi hết giờ và tính điểm theo thang 10.
TestRecord Exam::startExam(const std::string& username, int timeLimitMin)
{
    cout << "\n=== BAT DAU LAM BAI ===\n";
    cout << "Thoi gian: " << timeLimitMin << " giay\n"; 
    cout << "Nhan 'S' de nop bai som.\n\n";

    TestRecord record;
    record.studentName = username;
    record.correctCount = 0;
    record.totalCount = numberOfQuestions;

    char* userAnswers = new char[numberOfQuestions];
    bool* result = new bool[numberOfQuestions];
    for (int i = 0; i < numberOfQuestions; i++) {
        userAnswers[i] = '-';
        result[i] = false;
    }

    // Ghi lại thời điểm bắt đầu
    time_t startTime = time(nullptr);
    bool timeUp = false;

    for (int i = 0; i < numberOfQuestions; i++) {
        // 1. Kiểm tra thời gian TRƯỚC KHI hiển thị câu hỏi tiếp theo
        time_t now = time(nullptr);
        long elapsed = (long)difftime(now, startTime);
        long remaining = timeLimitMin - elapsed;

        if (remaining <= 0) {
            cout << "\n\n*** HET GIO! He thong tu dung thu bai. ***\n";
            timeUp = true;
            break;
        }

        // In thời gian còn lại cho người dùng biết trước khi làm câu này
        cout << "\n[Thoi gian con lai: " << remaining << " giay]\n";

        Question& q = questionList[i];
        cout << "----------------------------------------\n";
        cout << "Cau " << (i + 1) << "/" << numberOfQuestions << "\n";
        cout << q.content << "\n";
        cout << "A. " << q.answers[0] << "\n";
        cout << "B. " << q.answers[1] << "\n";
        cout << "C. " << q.answers[2] << "\n";
        cout << "D. " << q.answers[3] << "\n";
        cout << "(Nhap S de nop bai ngay)\n";

        char chosen = '-';

        while (true) {
            cout << "Chon dap an (A/B/C/D hoac S=nop bai): ";
            string input;
            if (!(cin >> input)) {
                break;
            }

            if (input.empty()) continue;
            char c = toupper(input[0]);

            if (c == 'S') {
                cout << "\nBan da chon nop bai som.\n";
                goto DONE;
            }

            if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
                chosen = c;
                break;
            }
            cout << "Nhap khong hop le! Vui long nhap A/B/C/D.\n";
        }

        // 2. Kiểm tra thời gian SAU KHI người dùng nhập xong
        now = time(nullptr);
        elapsed = (long)difftime(now, startTime);
        if (timeLimitMin - elapsed <= 0) {
            cout << "\n\n*** HET GIO TRONG LUC LAM CAU NAY! He thong tu dong thu bai. ***\n";
            timeUp = true;
            break;
        }

        userAnswers[i] = chosen;
        if (chosen != '-' && chosen == q.correct) {
            result[i] = true;
            record.correctCount++;
        }
    }

DONE:
    // Tính thời gian thực tế đã dùng
    time_t endTime = time(nullptr);
    long usedSec = (long)difftime(endTime, startTime);
    if (usedSec > timeLimitMin) usedSec = timeLimitMin; 
    
    cout << "\nThoi gian lam bai: " << usedSec << " giay\n";

    // --- Phần tính điểm giữ nguyên ---
    if (record.totalCount > 0)
        record.score = (double)record.correctCount / record.totalCount * 10.0;
    else
        record.score = 0.0;

    time_t nowTime = time(nullptr);
    tm* localTm = localtime(&nowTime);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTm);
    record.datetime = buf;

    // =====================================
    // HIỂN THỊ ĐÁP ÁN SAU KHI THI (ĐÃ ĐƯỢC ĐƯA VÀO ĐÚNG VỊ TRÍ)
    // =====================================
    cout << "\n\n========================================\n";
    cout << "CHI TIET KET QUA BAI THI\n";
    cout << "========================================\n";

    for (int i = 0; i < numberOfQuestions; i++) {
        cout << "\nCau " << (i + 1) << ":\n";
        cout << questionList[i].content << "\n";

        cout << "Ban chon   : " << userAnswers[i];
        
        if (userAnswers[i] >= 'A' && userAnswers[i] <= 'D') {
            int idx = userAnswers[i] - 'A';
            cout << ". " << questionList[i].answers[idx];
        }
        cout << "\n";
        
        cout << "Dap an dung: " << questionList[i].correct << ". "
             << questionList[i].answers[questionList[i].correct - 'A'] << "\n";

        if (userAnswers[i] == '-') {
            cout << "[CHUA TRA LOI]\n";
        }
        else if (result[i]) {
            cout << "[DUNG]\n";
        }
        else {
            cout << "[SAI]\n";
        }
    }

    cout << "\n========================================\n";
    cout << "Tong so cau dung: " << record.correctCount << "/" << record.totalCount << "\n";
    cout << "Diem so: " << record.score << "/10\n";
    cout << "========================================\n";

    cout << "\nDEBUG 1\n";

    // Giải phóng bộ nhớ động ở CUỐI HÀM, sau khi đã dùng xong để in kết quả
    delete[] userAnswers;
    delete[] result;

    cout << "\nDEBUG 2\n";
    return record;
} // Đóng hàm startExam hợp lệ ở đây

