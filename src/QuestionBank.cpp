#include "../include/QuestionBank.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>       
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

// loadFromFile
// Định dạng mỗi dòng:
// id|content|answerA|answerB|answerC|answerD|correct
bool QuestionBank::loadFromFile(string filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "[Loi] Khong the mo file de doc: " << filename << "\n";
        return false;
    }

    string line;
    int lineNum = 0;
    while (getline(fin, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;
        istringstream ss(line);
        string token;
        Question q;

        // Truong 1: id
        if (!getline(ss, token, '|')) {
            cout << "[Canh bao] Dong " << lineNum << ": thieu truong id. Bo qua.\n";
            continue;
        }
        try {
            q.id = stoi(token);
        } catch (...) {
            cout << "[Canh bao] Dong " << lineNum << ": id khong phai so (\"" << token << "\"). Bo qua.\n";
            continue;
        }

        // Truong 2: content 
        if (!getline(ss, token, '|')) {
            cout << "[Canh bao] Dong " << lineNum << ": thieu noi dung cau hoi. Bo qua.\n";
            continue;
        }
        q.content = token;

        // Truong 3-6: 4 dap an A B C D 
        bool answersOk = true;
        for (int i = 0; i < 4; i++) {
            if (!getline(ss, token, '|')) {
                cout << "[Canh bao] Dong " << lineNum << ": thieu dap an thu " << (i + 1) << ". Bo qua.\n";
                answersOk = false;
                break;
            }
            q.answers[i] = token;
        }
        if (!answersOk) continue;

        // Truong 7: correct ('A'/'B'/'C'/'D') 
        if (!getline(ss, token, '|')) {
            cout << "[Canh bao] Dong " << lineNum << ": thieu dap an dung. Bo qua.\n";
            continue;
        }
        if (token.empty()) {
            cout << "[Canh bao] Dong " << lineNum << ": truong dap an dung dang trong. Bo qua.\n";
            continue;
        }
        char c = toupper((unsigned char)token[0]); 
        if (c < 'A' || c > 'D') {
            cout << "[Canh bao] Dong " << lineNum
                 << ": dap an dung '" << token << "' khong hop le (chi chap nhan A-D). Bo qua.\n";
            continue;
        }
        q.correct = c;

        addQuestion(q);
    }

    fin.close();
    cout << "[OK] Da nap " << getQuestionCount() << " cau hoi tu file \"" << filename << "\".\n";
    return true;
}

// saveToFile
// Ghi toan bo danh sach xuong file theo dinh dang phan tach '|':
// id|content|answerA|answerB|answerC|answerD|correct
bool QuestionBank::saveToFile(string filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "[Loi] Khong the mo file de ghi: " << filename << "\n";
        return false;
    }

    QuestionNode* current = head;
    int saved = 0;
    while (current != nullptr) {
        const Question& q = current->data;
        fout << q.id         << '|' 
             << q.content    << '|'
             << q.answers[0] << '|'
             << q.answers[1] << '|'
             << q.answers[2] << '|'
             << q.answers[3] << '|'
             << q.correct
             << '\n';
        saved++;
        current = current->next;
    }

    fout.close();
    cout << "[OK] Da luu " << saved
         << " cau hoi vao file \"" << filename << "\".\n";
    return true;
}

void QuestionBank::printAll()
{
    QuestionNode* current = head;

    while (current != nullptr)
    {
        cout << "ID: " << current->data.id << "\n";
        cout << "Cau hoi: " << current->data.content << "\n";

        cout << "A. " << current->data.answers[0] << "\n";
        cout << "B. " << current->data.answers[1] << "\n";
        cout << "C. " << current->data.answers[2] << "\n";
        cout << "D. " << current->data.answers[3] << "\n";

        cout << "Dap an dung: "
             << current->data.correct << "\n\n";

        current = current->next;
    }

    // mới thêm 16/06/2026
    Question* QuestionBank::generateRandomSet(int n) {
    int total = getQuestionCount();
    if (n > total) {
        cout << "[Loi] Khong du cau hoi!\n";
        return nullptr;
    }

    // Tạo mảng index rồi Fisher-Yates
    int* indices = new int[total];
    for (int i = 0; i < total; i++) indices[i] = i;
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }

    // Lấy N câu đầu
    Question* result = new Question[n];
    for (int i = 0; i < n; i++)
        result[i] = *getQuestionAt(indices[i]);

    delete[] indices;
    return result;
   }
}
