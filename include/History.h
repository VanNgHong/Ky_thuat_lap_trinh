#ifndef HISTORY_H
#define HISTORY_H

#include <string>
using namespace std;

// Struct lưu thông tin một lần thi của thí sinh
struct TestRecord {
    string studentName;  
    int correctCount;         
    int totalCount;           
    double score;             
    string datetime;     // Ngày giờ thi (dạng chuỗi, ví dụ: "2025-06-09 14:30:00")
};

// Node của danh sách liên kết đơn chứa một bản ghi lịch sử
struct HistoryNode {
    TestRecord data;      
    HistoryNode* next;   
};

// Class quản lý lịch sử thi bằng danh sách liên kết đơn
class HistoryManager {
private:
    HistoryNode* head;  
    HistoryNode* tail;  

public:
    // Constructor: khởi tạo danh sách rỗng
    HistoryManager();

    // Destructor: giải phóng toàn bộ bộ nhớ động
    ~HistoryManager();

    // Thêm một bản ghi lịch sử vào cuối danh sách
    void addRecord(TestRecord r);

    // Ghi toàn bộ danh sách ra file 
    void saveToFile(const string& filename);

    // Lưu thông tin từ file history.text vào danh sách móc nối
    void loadFromFile(const string& filename);
};

#endif // HISTORY_H