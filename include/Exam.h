#pragma once
#include "QuestionBank.h"
#include "History.h"
#include <string>
// ============================================================
//  Exam.h  –  Khai báo logic sinh đề, xáo trộn, bộ đếm giờ
// ============================================================

class Exam {
public:
    // Khởi tạo: cấp phát mảng Question[n]
Exam(int n, int timeLimitSec);
int timeLimitSec;
    // Destructor: giải phóng bộ nhớ động
    ~Exam();

    void loadFromBank(Question* questions);

    // Nạp ngẫu nhiên câu hỏi từ ngân hàng vào đề thi
    void generateExamFromBank(QuestionBank& bank);

    // Xáo trộn thứ tự câu hỏi và đáp án
    void shuffleExam();

    // In đề thi ra console để kiểm tra
    void printExam() const;

    TestRecord startExam(const std::string &username, int timeLimitMin);

private:
    // Số lượng câu hỏi trong đề
    int       numberOfQuestions;

    // Mảng động lưu danh sách câu hỏi
    Question* questionList;

    // Mảng động lưu đáp án người dùng chọn
    char*     userAnswers;

    // Xáo trộn mảng đáp án của 1 câu (Fisher-Yates)
    void shuffleAnswers(std::string arr[], int n);

    // Xáo trộn mảng câu hỏi (Fisher-Yates)
    void shuffleQuestions(Question arr[], int n);
    // Copy câu hỏi từ mảng vào exam
};
