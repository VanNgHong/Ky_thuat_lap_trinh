#include "../include/History.h"
#include <iostream>
#include <fstream>
using namespace std;

// Constructor: khởi tạo danh sách rỗng
HistoryManager::HistoryManager() {
    head = nullptr;
    tail = nullptr;
}

// Destructor: duyệt toàn bộ danh sách và giải phóng từng node
HistoryManager::~HistoryManager() {
    HistoryNode* current = head;

    while (current != nullptr) {
        HistoryNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
}

// Thêm bản ghi mới vào cuối danh sách 
void HistoryManager::addRecord(TestRecord r) {
    HistoryNode* newNode = new HistoryNode();
    newNode->data = r;
    newNode->next = nullptr;

    if (tail == nullptr) {
        // Trường hợp: Danh sách rỗng
        head = newNode;
        tail = newNode;
    } else {
        // Trường hợp còn lại
        tail->next = newNode;
        tail = newNode;
    }
}

// Ghi nối toàn bộ danh sách lịch sử vào file text
// Định dạng mỗi dòng: studentName|correctCount|totalCount|score|datetime
void HistoryManager::saveToFile(const string& filename) {
    ofstream outFile(filename, ios::app);

    if (!outFile.is_open()) {
        cerr << "[Loi] Khong the mo file: " << filename << "\n";
        return;
    }

    HistoryNode* current = head;
    while (current != nullptr) {
        const TestRecord& rec = current->data;
        outFile << rec.studentName  << "|"
                << rec.correctCount << "|"
                << rec.totalCount   << "|"
                << rec.score        << "|"
                << rec.datetime     << "\n";

        current = current->next;
    }

    outFile.close();
    cout << "[OK] Da ghi lich su vao file: " << filename << "\n";
}