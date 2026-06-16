#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <string>
using namespace std;

// Struct lưu thông tin một câu hỏi trắc nghiệm
struct Question {
    int id;                  
    string content;     
    string answers[4];  
    char correct;            // Ký tự đáp án đúng: 'A', 'B', 'C', hoặc 'D'
};

// Node của danh sách liên kết đơn chứa một câu hỏi
struct QuestionNode {
    Question data;        
    QuestionNode* next;   
};

// Class quản lý ngân hàng câu hỏi bằng danh sách liên kết đơn
class QuestionBank {
private:
    QuestionNode* head;   
    QuestionNode* tail;   
    
public:
    // Constructor: khởi tạo danh sách rỗng
    QuestionBank();

    // Destructor: giải phóng toàn bộ bộ nhớ động
    ~QuestionBank();

    // Thêm câu hỏi vào cuối danh sách
    void addQuestion(Question q);

    // Xóa câu hỏi theo id
    void removeQuestion(int id);

    // Đếm tổng số câu hỏi hiện có
    int getQuestionCount();

    // Lấy con trỏ Question tại vị trí index (0-indexed)
    Question* getQuestionAt(int index);

    void printAll();
    
    //I/O
    bool loadFromFile(string filename);
    bool saveToFile(string filename);

    // Bốc ngẫu nhiên N câu, trả về mảng động 
    Question* generateRandomSet(int n);
};

#endif // QUESTIONBANK_H
