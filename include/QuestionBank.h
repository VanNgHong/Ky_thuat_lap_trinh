// ============================================================
//  QuestionBank.h  –  Khai báo cấu trúc câu hỏi & ngân hàng
//  Tác giả: VÂN  |  Role: Kỹ sư Dữ liệu
// ============================================================

#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <string>
using namespace std;

// Struct lưu thông tin một câu hỏi trắc nghiệm
struct Question {
    int id;      
    string subject;      
    string difficulty;   // Easy, Medium, Hard            
    string content;     
    string answers[4];  
    char correct;            
    // Ký tự đáp án đúng: 'A', 'B', 'C', hoặc 'D'
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

    // Kiem tra ID da ton tai trong ngan hang chua
    bool isIdExists(int id);

    // Thêm câu hỏi vào cuối danh sách
    bool addQuestion(Question q);

    // Xóa câu hỏi theo id
    void removeQuestion(int id);

    // Đếm tổng số câu hỏi hiện có
    int getQuestionCount();

    // Lấy con trỏ Question tại vị trí index (0-indexed)
    Question* getQuestionAt(int index);

    // Đếm số câu theo mức độ
    int countByDifficulty(const string& difficulty);

    // Lấy danh sách câu hỏi theo mức độ
    Question* getQuestionsByDifficulty(const string& difficulty, int& count);

    // Lay danh sach cac mon hoc khac nhau co trong ngan hang
    string* getDistinctSubjects(int& count);

    // Dem so cau hoi theo DUNG mon hoc VA muc do (dung khi sinh de theo mon)
    int countBySubjectAndDifficulty(const string& subject, const string& difficulty);

    // Lay danh sach cau hoi theo mon hoc VA muc do
    Question* getQuestionsBySubjectAndDifficulty(const string& subject, const string& difficulty, int& count);

    void printAll();
    
    //I/O
    bool loadFromFile(string filename);
    bool saveToFile(string filename);

    // Bốc ngẫu nhiên N câu, trả về mảng động 
    Question* generateRandomSet(int n);
};

#endif // QUESTIONBANK_H
