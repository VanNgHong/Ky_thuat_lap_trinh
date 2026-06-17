// ============================================================
//  Exam.cpp  –  Triển khai logic sinh đề, xáo trộn, bộ đếm giờ
//  Tác giả: HÙNG  |  Role: Kỹ sư Logic / Thuật toán
// ============================================================

#include "Exam.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <cctype>
using namespace std;

// ============================================================
//  CONSTRUCTOR / DESTRUCTOR
// ============================================================

// Constructor: Cấp phát động mảng Question[N]
Exam::Exam(int n, int t)
{
    numberOfQuestions = n;
    timeLimitSec = t;

    questionList = new Question[numberOfQuestions];
    userAnswers  = new char[numberOfQuestions];

    for (int i = 0; i < numberOfQuestions; i++)
        userAnswers[i] = '-';
}

// Destructor: Giải phóng bộ nhớ động
Exam::~Exam() {
    delete[] questionList;
    delete[] userAnswers;
}

// ============================================================
//  NẠP CÂU HỎI VÀO ĐỀ THI
// ============================================================

// Nạp từ mảng Question[] (dùng sau generateRandomSet)
void Exam::loadFromBank(Question* questions) {
    for (int i = 0; i < numberOfQuestions; i++)
        questionList[i] = questions[i];
}

// Nạp câu hỏi từ QuestionBank vào Exam 
void Exam::generateExamFromBank(QuestionBank& bank) {
    int bankCount = bank.getQuestionCount();
    if (bankCount < numberOfQuestions) {
        std::cout << "Loi: Ngan hang khong du " << numberOfQuestions << " cau hoi!\n";
        return;
    }

    int* indices = new int[bankCount];
    for (int i = 0; i < bankCount; ++i) indices[i] = i;

    // Fisher-Yates shuffle trên mảng index
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

// ============================================================
//  THUẬT TOÁN XÁO TRỘN (Fisher-Yates)
// ============================================================

// Xáo trộn mảng chuỗi (Áp dụng cho mảng answers[4])
void Exam::shuffleAnswers(string arr[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        string temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Xáo trộn thứ tự các câu hỏi trong đề
void Exam::shuffleQuestions(Question arr[], int n)
{
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        Question temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Áp dụng toàn bộ: xáo câu hỏi + xáo đáp án từng câu,
// đồng thời cập nhật lại trường 'correct' sau khi xáo đáp án.
void Exam::shuffleExam() {

    // 1. Xáo thứ tự câu hỏi: Shuffle(questions, qCount)
    shuffleQuestions(questionList, numberOfQuestions);

    // 2. Với mỗi câu, xáo đáp án và cập nhật đáp án đúng
    for (int i = 0; i < numberOfQuestions; ++i) {
        Question& q = questionList[i]; // Lấy câu hỏi hiện tại

        // Ghi nhớ nội dung đáp án đúng trước khi xáo
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

// ============================================================
//  IN ĐỀ THI (dùng để debug / kiểm tra)
// ============================================================

void Exam::printExam() const {
    for (int i = 0; i < numberOfQuestions; ++i) {
        cout << "Cau hoi " << (i + 1)
             << " (ID: " << questionList[i].id << "): "
             << questionList[i].content << "\n";
        cout << "  A. " << questionList[i].answers[0] << "\n";
        cout << "  B. " << questionList[i].answers[1] << "\n";
        cout << "  C. " << questionList[i].answers[2] << "\n";
        cout << "  D. " << questionList[i].answers[3] << "\n";
        cout << "=> Dap an dung: " << questionList[i].correct << "\n\n";
    }
}

// ============================================================
//  CHẠY BÀI THI
// ============================================================

// Hiển thị từng câu hỏi, nhận đáp án, đếm giờ ngược,
// tự động thu bài khi hết giờ, tính điểm thang 10.
TestRecord Exam::startExam(const std::string& username, int timeLimitSec_) {
    cout << "\n=== BAT DAU LAM BAI ===\n";
    cout << "Thoi gian: " << timeLimitSec_ << " giay\n";
    cout << "Nhan 'S' de nop bai som.\n\n";

    TestRecord record;
    record.studentName  = username;
    record.correctCount = 0;
    record.totalCount   = numberOfQuestions;

    // Khởi tạo mảng kết quả từng câu
    bool* result = new bool[numberOfQuestions];
    for (int i = 0; i < numberOfQuestions; i++) {
        userAnswers[i] = '-';
        result[i]      = false;
    }

    time_t startTime = time(nullptr);
    bool   submitted = false;   // true khi nộp sớm hoặc hết giờ

    for (int i = 0; i < numberOfQuestions && !submitted; i++) {

        // ── Kiểm tra giờ TRƯỚC khi hiển thị câu hỏi ──────────
        long elapsed   = (long)difftime(time(nullptr), startTime);
        long remaining = timeLimitSec_ - elapsed;

        if (remaining <= 0) {
            cout << "\n\n*** HET GIO! He thong tu dong thu bai. ***\n";
            submitted = true;
            break;
        }

        cout << "\n[Thoi gian con lai: " << remaining << " giay]\n";

        // ── Hiển thị câu hỏi ──────────────────────────────────
        Question& q = questionList[i];
        cout << "----------------------------------------\n";
        cout << "Cau " << (i + 1) << "/" << numberOfQuestions << "\n";
        cout << q.content << "\n";
        cout << "A. " << q.answers[0] << "\n";
        cout << "B. " << q.answers[1] << "\n";
        cout << "C. " << q.answers[2] << "\n";
        cout << "D. " << q.answers[3] << "\n";
        cout << "(Nhap S de nop bai ngay)\n";

        // ── Nhận đáp án từ người dùng ─────────────────────────
        char chosen = '-';
        while (true) {
            cout << "Chon dap an (A/B/C/D hoac S=nop bai): ";
            string input;
            if (!(cin >> input)) break;
            if (input.empty()) continue;

            char c = static_cast<char>(toupper(static_cast<unsigned char>(input[0])));

            if (c == 'S') {
                cout << "\nBan da chon nop bai som.\n";
                submitted = true;
                break;
            }
            if (c >= 'A' && c <= 'D') {
                chosen = c;
                break;
            }
            cout << "Nhap khong hop le! Vui long nhap A/B/C/D hoac S.\n";
        }

        if (submitted) break;

        // ── Kiểm tra giờ SAU KHI nhập xong ───────────────────
        elapsed   = (long)difftime(time(nullptr), startTime);
        remaining = timeLimitSec_ - elapsed;
        if (remaining <= 0) {
            cout << "\n\n*** HET GIO TRONG LUC LAM CAU NAY! He thong tu dong thu bai. ***\n";
            submitted = true;
        }

        // ── Ghi nhận đáp án và chấm điểm câu này ─────────────
        userAnswers[i] = chosen;
        if (chosen != '-' && chosen == q.correct) {
            result[i] = true;
            record.correctCount++;
        }
    }

    // ── Tính thời gian thực tế đã dùng ────────────────────────
    long usedSec = (long)difftime(time(nullptr), startTime);
    if (usedSec > timeLimitSec_) usedSec = timeLimitSec_;
    cout << "\nThoi gian lam bai: " << usedSec << " giay\n";

    // ── Tính điểm ─────────────────────────────────────────────
    record.score = (record.totalCount > 0)
        ? (double)record.correctCount / record.totalCount * 10.0
        : 0.0;

    // ── Ghi timestamp ─────────────────────────────────────────
    time_t nowTime = time(nullptr);
    tm*    localTm = localtime(&nowTime);
    char   buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTm);
    record.datetime = buf;

    // ── Hiển thị chi tiết kết quả từng câu ───────────────────
    cout << "\n\n========================================\n";
    cout << "CHI TIET KET QUA BAI THI\n";
    cout << "========================================\n";

    for (int i = 0; i < numberOfQuestions; i++) {
        cout << "\nCau " << (i + 1) << ":\n";
        cout << questionList[i].content << "\n";

        cout << "Ban chon   : " << userAnswers[i];
        if (userAnswers[i] >= 'A' && userAnswers[i] <= 'D')
            cout << ". " << questionList[i].answers[userAnswers[i] - 'A'];
        cout << "\n";

        cout << "Dap an dung: " << questionList[i].correct << ". "
             << questionList[i].answers[questionList[i].correct - 'A'] << "\n";

        if (userAnswers[i] == '-')
            cout << "[CHUA TRA LOI]\n";
        else if (result[i])
            cout << "[DUNG]\n";
        else
            cout << "[SAI]\n";
    }

    cout << "\n========================================\n";
    cout << "Tong so cau dung: " << record.correctCount
         << "/" << record.totalCount << "\n";
    cout << "Diem so: " << record.score << "/10\n";
    cout << "========================================\n";

    delete[] result;
    return record;
}
