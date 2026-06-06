#pragma once
#include <string>

// ============================================================
//  QuestionBank.h  –  Khai báo cấu trúc câu hỏi & ngân hàng
//  Tác giả: VÂN  |  Role: Kỹ sư Dữ liệu
//  Thành phụ thuộc vào file này – KHÔNG được thay đổi
//  tên field mà không báo trước cho Thành và Hùng!
// ============================================================

// Cấu trúc 1 câu hỏi – ĐÂY LÀ HỢP ĐỒNG CHUNG CỦA NHÓM
struct Question {
    int         id;
    std::string content;
    std::string optA;
    std::string optB;
    std::string optC;
    std::string optD;
    char        correctAns;  // 'A', 'B', 'C', hoặc 'D'
};

// Node của Danh sách liên kết đơn
struct QuestionNode {
    Question     data;
    QuestionNode* next;
};

// Class quản lý Ngân hàng câu hỏi (Singly Linked List)
class QuestionBank {
public:
    QuestionBank();
    ~QuestionBank();

    // File I/O
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // CRUD
    void addQuestion(const Question& q);
    bool removeQuestion(int id);
    Question* findById(int id);

    // Tiện ích
    int  getCount() const;
    int  getNextId() const;
    void printAll() const;

    // Cho ExamEngine dùng – lấy mảng câu hỏi ngẫu nhiên
    // Caller chịu trách nhiệm delete[] sau khi dùng
    Question* getRandomSubset(int count) const;

private:
    QuestionNode* head;
    int           size;

    // Cấm copy (tránh shallow copy con trỏ)
    QuestionBank(const QuestionBank&) = delete;
    QuestionBank& operator=(const QuestionBank&) = delete;
};
