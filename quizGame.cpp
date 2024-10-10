// quiz_game.cpp

#include <iostream>
#include <string>

using namespace std;

// Structure to represent a question
struct Question {
    string text;
    string options[5];
    int correctAnswerIndex;
};

// Function to ask a question and get user's answer
string askQuestion(Question q) {
    cout << endl << q.text << endl;
    for (int i = 0; i < 5; i++) {
        cout << (i + 1) << ". " << q.options[i] << endl;
    }
    cout << "Enter your answer (1-5): ";
    int userChoice;
    cin >> userChoice;
    if (userChoice - 1 == q.correctAnswerIndex) {
        return "Correct!";
    } else {
        return "Incorrect. The correct answer is " + q.options[q.correctAnswerIndex];
    }
}

// Function to calculate user's score
int calculateScore(int correctAnswers) {
    return correctAnswers;
}

int main() {
    Question questions[5] = {
        {"What is the capital of France?", {"Berlin", "Paris", "London", "Rome", "Madrid"}, 1},
        {"What is 2 + 2?", {"1", "2", "3", "4", "5"}, 3},
        {"What is the largest planet in our solar system?", {"Earth", "Saturn", "Jupiter", "Uranus", "Neptune"}, 2},
        {"What is the smallest country in the world?", {"Monaco", "Vatican City", "Nauru", "Tuvalu", "San Marino"}, 1},
        {"What is the largest living species of lizard?", {"Komodo dragon", "Saltwater crocodile", "Black mamba", "Green anaconda", "African elephant"}, 0}
    };

    int correctAnswers = 0;

    cout << "Welcome to the Quiz Game!" << endl;

    for (int i = 0; i < 5; i++) {
        string result = askQuestion(questions[i]);
        cout << result << endl;
        if (result == "Correct!") {
            correctAnswers++;
        }
    }
    int score = calculateScore(correctAnswers);
    cout << endl << "Your final score is " << score << " out of 5." << endl;
    return 0;
}
