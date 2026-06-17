// ============================================================
//  History.cpp  –  Hoàn thiện các hàm I/O
//  Tác giả: VÂN  |  Role: Kỹ sư Dữ liệu
// ============================================================

#include "../include/History.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
                << rec.subject      << "|"
                << rec.correctCount << "|"
                << rec.totalCount   << "|"
                << rec.score        << "|"
                << rec.datetime     << "\n";

        current = current->next;
    }
    outFile.close();
    cout << "[OK] Da ghi lich su vao file: " << filename << "\n";
}

// Lưu thông tin từ file history.text vào danh sách móc nối
void HistoryManager::loadFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string token;
        TestRecord r;

        getline(ss, r.studentName, '|');
        getline(ss, r.subject, '|');
        getline(ss, token, '|');
        r.correctCount = stoi(token);
        getline(ss, token, '|');
        r.totalCount = stoi(token);
        getline(ss, token, '|');
        r.score = stod(token);
        getline(ss, r.datetime, '|');
        addRecord(r);
    }
    fin.close();
}

void HistoryManager::printAll()
{
    if (head == nullptr)
    {
        cout << "Chua co lich su thi.\n";
        return;
    }

    HistoryNode* current = head;

    while (current != nullptr)
    {
        cout << "Sinh vien: "
             << current->data.studentName
             << "\n";

        cout << "Mon hoc: "
             << current->data.subject
             << "\n";

        cout << "So cau dung: "
             << current->data.correctCount
             << "/"
             << current->data.totalCount
             << "\n";

        cout << "Diem: "
             << current->data.score
             << "\n";

        cout << "Thoi gian: "
             << current->data.datetime
             << "\n";

        cout << "--------------------------\n";

        current = current->next;
    }
}

void HistoryManager::printByUser(const string& username)
{
    bool found = false;

    HistoryNode* current = head;

    while (current != nullptr)
    {
        if (current->data.studentName == username)
        {
            found = true;

            cout << "Mon hoc: "
                 << current->data.subject
                 << "\n";

            cout << "So cau dung: "
                 << current->data.correctCount
                 << "/"
                 << current->data.totalCount
                 << "\n";

            cout << "Diem: "
                 << current->data.score
                 << "\n";

            cout << "Thoi gian: "
                 << current->data.datetime
                 << "\n";

            cout << "--------------------------\n";
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "Khong tim thay lich su thi cua sinh vien "
             << username
             << ".\n";
    }
}