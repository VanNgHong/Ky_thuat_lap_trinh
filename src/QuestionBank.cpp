#include "../include/QuestionBank.h"
#include <iostream>
using namespace std;

// Constructor: khởi tạo danh sách rỗng
QuestionBank::QuestionBank() {
    head = nullptr;
    tail = nullptr;
}

// Destructor: duyệt toàn bộ danh sách và giải phóng từng node để tránh Memory Leak
QuestionBank::~QuestionBank() {
    QuestionNode* current = head;

    while (current != nullptr) {
        QuestionNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
}

// Thêm câu hỏi mới vào cuối danh sách 
void QuestionBank::addQuestion(Question q) {
    QuestionNode* newNode = new QuestionNode();
    newNode->data = q;
    newNode->next = nullptr; 

    if (tail == nullptr) {
        // Danh sách đang rỗng
        head = newNode;
        tail = newNode;
    } else {
        // Danh sách đã có phần tử
        tail->next = newNode;
        tail = newNode;
    }
}

// Xóa node có id trùng khớp khỏi danh sách
void QuestionBank::removeQuestion(int id) {
    // Trường hợp 1: Danh sách rỗng
    if (head == nullptr) {
        cout << "[Loi] Danh sach cau hoi dang rong.\n";
        return;
    }

    // Trường hợp 2: Node cần xóa chính là HEAD
    if (head->data.id == id) {
        QuestionNode* toDelete = head;  
        head = head->next; 
        if (head == nullptr) {
            tail = nullptr;
        }
        delete toDelete; 
        return;
    }

    // Trường hợp 3: Node cần xóa nằm ở giữa hoặc cuối 
    QuestionNode* prev    = head;
    QuestionNode* current = head->next;

    while (current != nullptr) {
        // Tìm thấy node cần xóa tại 'current'
        if (current->data.id == id) {
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
            delete current; 
            return;
        }
        // Chưa tìm thấy
        prev    = current;
        current = current->next;
    }

    // Trường hợp 4: Không tìm thấy id trong danh sách
    cout << "[Canh bao] Khong tim thay cau hoi co id = " << id << ".\n";
}

// Đếm và trả về tổng số câu hỏi hiện có trong danh sách
int QuestionBank::getQuestionCount() {
    int count = 0;
    QuestionNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// Trả về con trỏ tới Question tại vị trí index (0-indexed)
// Trả về nullptr nếu index vượt quá phạm vi
Question* QuestionBank::getQuestionAt(int index) {
    // Nếu index âm
    if (index < 0) {
        return nullptr;
    }

    int currentIndex  = 0;
    QuestionNode* current = head;
    while (current != nullptr) {
        if (currentIndex == index) {
            return &(current->data);
        }
        currentIndex++;
        current = current->next;
    }
    
    // Duyệt hết danh sách mà không tìm thấy 
    return nullptr;
}