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
    ~ExamEngine();

    // Chạy toàn bộ bài thi → trả về kết quả
    TestRecord run();

private:
    const QuestionBank& bank;
    std::string         username;
    int                 numQuestions;
    int                 timeLimitSec;
};
