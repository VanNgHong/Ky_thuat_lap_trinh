#include "Exam.h"

// CÁC HÀM CƠ BẢN CỦA CLASS EXAM

// Constructor: Cấp phát động mảng Question[N]
Exam::Exam(int n) {
    numberOfQuestions = n;
    questionList = new Question[numberOfQuestions];
}

// Destructor: Giải phóng bộ nhớ động
Exam::~Exam() {
    delete[] questionList;
}

// Hàm nạp câu hỏi từ QuestionBank vào Exam 
void Exam::generateExamFromBank(QuestionBank& bank) {
    int bankCount = bank.getQuestionCount();
    if (bankCount < numberOfQuestions) {
        std::cout << "Loi: Ngan hang khong du " << numberOfQuestions << " cau hoi!\n";
        return;
    }

    int* indices = new int[bankCount];
    for (int i = 0; i < bankCount; ++i) indices[i] = i;

    for (int i = bankCount - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    for (int i = 0; i < numberOfQuestions; ++i) {
        Question* bankQuestion = bank.getQuestionAt(indices[i]);
        if (bankQuestion != nullptr) {
            questionList[i] = *bankQuestion;
        }
    }
    delete[] indices;
}

// Xáo trộn mảng chuỗi (Áp dụng cho mảng answers[4])
// Tương ứng: PROCEDURE Shuffle(arr[], n) dành cho đáp án
void shuffleAnswers(string arr[], int n) {
    for (int i = n - 1; i > 0; --i) {
        // Chọn ngẫu nhiên j trong [0 .. i]
        int j = rand() % (i + 1);

        // Hoán vị arr[i] <-> arr[j]
        string temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Xáo trộn mảng câu hỏi (Áp dụng cho mảng questionList)
// Tương ứng: PROCEDURE Shuffle(arr[], n) dành cho câu hỏi

void shuffleQuestions(Question arr[], int n) {
    for (int i = n - 1; i > 0; --i) {
        // Chọn ngẫu nhiên j trong [0 .. i]
        int j = rand() % (i + 1);

        // Hoán vị arr[i] <-> arr[j]
        Question temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Hàm chính áp dụng logic trộn đề thi
// Tương ứng: PROCEDURE ShuffleExam(questions[], qCount)
void Exam::shuffleExam() {
    
    // 1. Xáo thứ tự câu hỏi: Shuffle(questions, qCount)
    shuffleQuestions(questionList, numberOfQuestions);

    // 2. FOR EACH q IN questions DO
    for (int i = 0; i < numberOfQuestions; ++i) {
        Question& q = questionList[i]; // Lấy câu hỏi hiện tại
// Vì struct Question dùng ký tự 'A','B','C','D' để đánh dấu đáp án đúng, trước khi xáo ta cần biết nội dung chuỗi (text) chính xác của đáp án đúng là gì.
        int originalCorrectIndex = q.correct - 'A'; 
        string correctText = q.answers[originalCorrectIndex];

        // Xáo đáp án riêng từng câu: Shuffle(q.answers, q.answerCount)
        // Với câu hỏi trắc nghiệm cố định là 4 đáp án
        shuffleAnswers(q.answers, 4);

        // Cập nhật lại correctIndex sau khi xáo
        for (int k = 0; k < 4; ++k) {
            if (q.answers[k] == correctText) {
                q.correct = static_cast<char>('A' + k);
                break; 
            }
        }
    }
}

// Hàm in đề thi để kiểm tra
void Exam::printExam() const {
    for (int i = 0; i < numberOfQuestions; ++i) {
        std::cout << "Cau hoi " << i + 1 << " (ID: " << questionList[i].id << "): " << questionList[i].content << "\n";
        std::cout << "  A. " << questionList[i].answers[0] << "\n";
        std::cout << "  B. " << questionList[i].answers[1] << "\n";
        std::cout << "  C. " << questionList[i].answers[2] << "\n";
        std::cout << "  D. " << questionList[i].answers[3] << "\n";
        std::cout << "=> Dap an dung: " << questionList[i].correct << "\n\n";
    }
}
