#include "Exam.h"
#include <iostream>
#include <chrono>
#include <limits>
#include <ctime>
#include <cctype>
using namespace std;

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

// Chạy toàn bộ bài thi: hiển thị từng câu, nhận đáp án, đếm giờ ngược,
// tự động thu bài khi hết giờ và tính điểm theo thang 10.
TestRecord Exam::run(const std::string& username, int timeLimitSec) {
    TestRecord record;
    record.studentName = username;
    record.correctCount = 0;
    record.totalCount   = numberOfQuestions;

    auto startTime = chrono::steady_clock::now();

    for (int i = 0; i < numberOfQuestions; ++i) {
        // Tính thời gian còn lại
        auto now = chrono::steady_clock::now();
        long elapsedSec   = chrono::duration_cast<chrono::seconds>(now - startTime).count();
        long remainingSec = static_cast<long>(timeLimitSec) - elapsedSec;

        if (remainingSec <= 0) {
            cout << "\n*** Het gio lam bai! He thong tu dong thu bai. ***\n";
            break;
        }

        Question& q = questionList[i];
        cout << "\n----------------------------------------\n";
        cout << "Cau " << (i + 1) << "/" << numberOfQuestions
             << "   (Thoi gian con lai: ~" << remainingSec << " giay)\n";
        cout << q.content << "\n";
        cout << "  A. " << q.answers[0] << "\n";
        cout << "  B. " << q.answers[1] << "\n";
        cout << "  C. " << q.answers[2] << "\n";
        cout << "  D. " << q.answers[3] << "\n";

        char chosen = 0;
        bool validInput = false;
        while (!validInput) {
            cout << "Chon dap an (A/B/C/D): ";
            string line;
            if (!(cin >> line)) {
                if (cin.eof()) {
                    cout << "\n*** Het du lieu dau vao. Ket thuc bai thi som. ***\n";
                    chosen = 0; // coi nhu khong tra loi -> sai
                    validInput = true;
                    break;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            char c = static_cast<char>(toupper(static_cast<unsigned char>(line[0])));
            if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
                chosen = c;
                validInput = true;
            } else {
                cout << "Lua chon khong hop le. Vui long nhap A, B, C hoac D.\n";
            }
        }

        if (chosen == q.correct) {
            record.correctCount++;
            cout << "=> Chinh xac!\n";
        } else {
            cout << "=> Sai. Dap an dung la: " << q.correct << "\n";
        }
    }

    // Tính điểm theo thang 10
    if (record.totalCount > 0) {
        record.score = (static_cast<double>(record.correctCount) / record.totalCount) * 10.0;
    } else {
        record.score = 0.0;
    }

    // Lấy thời gian hiện tại làm dấu thời gian (timestamp)
    time_t nowTime = time(nullptr);
    tm* localTm = localtime(&nowTime);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTm);
    record.datetime = string(buf);

    return record;
}