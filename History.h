#pragma once
#include <string>

// ============================================================
//  History.h  –  Khai báo struct TestRecord & HistoryManager
//  Tác giả: VÂN  |  Role: Kỹ sư Dữ liệu
// ============================================================

// Cấu trúc 1 bản ghi kết quả thi – HỢP ĐỒNG CHUNG
struct TestRecord {
    std::string username;
    std::string subject;
    int         correctCount;
    int         totalQuestions;
    double      score;          // 0.0 – 10.0
    int         timeTakenSec;   // thời gian thực tế làm bài (giây)
    std::string timestamp;      // "YYYY-MM-DD HH:MM"
};

// Node danh sách liên kết lịch sử thi
struct HistoryNode {
    TestRecord  data;
    HistoryNode* next;
};

// Class quản lý lịch sử thi
class HistoryManager {
public:
    HistoryManager();
    ~HistoryManager();

    // File I/O
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // Thêm 1 bản ghi
    void addRecord(const TestRecord& record);

    // In toàn bộ / theo user
    void printAll() const;
    void printByUser(const std::string& username) const;

    // Thống kê
    void printStats() const;

private:
    HistoryNode* head;
    int          size;

    HistoryManager(const HistoryManager&) = delete;
    HistoryManager& operator=(const HistoryManager&) = delete;
};
