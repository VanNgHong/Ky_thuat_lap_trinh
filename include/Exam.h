#pragma once
#include "QuestionBank.h"
#include "History.h"
#include <string>

// ============================================================
//  Exam.h  –  Khai báo logic sinh đề, xáo trộn, bộ đếm giờ
//  Tác giả: HÙNG  |  Role: Kỹ sư Logic
// ============================================================

class ExamEngine {
public:
    // Khởi tạo: nhận ngân hàng câu hỏi, username, số câu, thời gian (giây)
    ExamEngine(const QuestionBank& bank,
               const std::string& username,
               int numQuestions,
               int timeLimitSec);
    
    // Destructor: Cực kỳ quan trọng để delete[] questionList giải phóng bộ nhớ động
    ~ExamEngine();

    // Chạy toàn bộ bài thi → trả về kết quả
    TestRecord run();

private:
    // ──── THUỘC TÍNH ĐƯỢC CUNG CẤP BAN ĐẦU ────
    const QuestionBank& bank;
    std::string         username;
    int                 numQuestions;
    int                 timeLimitSec;

    // ──── THUỘC TÍNH QUẢN LÝ MẢNG ĐỘNG (BỔ SUNG) ────
    Question* questionList; // Mảng động Question* được cấp phát bằng new Question[N]

    // ──── CÁC PHƯƠNG THỨC TRỢ GIÚP LOGIC TRỘN ĐỀ ────
    
    // Hàm nạp ngẫu nhiên câu hỏi từ ngân hàng câu hỏi vào questionList
    void generateExamFromBank();

    // Hàm áp dụng giải thuật hoán vị Fisher-Yates cho mảng chuỗi (đáp án)
    void shuffleAnswers(std::string arr[], int n);

    // Hàm áp dụng giải thuật hoán vị Fisher-Yates cho mảng Question (câu hỏi)
    void shuffleQuestions(Question arr[], int n);

    // Hàm tổng hợp: Gọi xáo trộn thứ tự câu hỏi và đáp án của từng câu
    void shuffleExam();
TestRecord startExam(const std::string& username, int timeLimitMin);
};
