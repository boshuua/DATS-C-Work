// quiz_game.cpp

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits> // For input validation

using namespace std;

// Structure to represent a question
struct Question {
    string text; // The text of the question
    string options[5]; // The options for the question
    int correctAnswerIndex; // The index of the correct answer
};

// Function to ask a question and get user's answer
string askQuestion(Question q) {
    // Display the question and options
    cout << endl << q.text << endl;
    for (int i = 0; i < 5; i++) {
        cout << (i + 1) << ". " << q.options[i] << endl;
    }
    // Prompt the user to enter their answer
    cout << "Enter your answer (1-5): ";
    int userChoice;
    while (!(cin >> userChoice) || userChoice < 1 || userChoice > 5) {
        // Input validation: ensure the user enters a number between 1 and 5
        cout << "Invalid input. Please enter a number between 1 and 5: ";
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
    }
    // Check if the user's answer is correct
    if (userChoice - 1 == q.correctAnswerIndex) {
        return "Correct!";
    } else {
        return "Incorrect. The correct answer is " + q.options[q.correctAnswerIndex];
    }
}

// Function to calculate user's score
int calculateScore(int correctAnswers) {
    // Simply return the number of correct answers
    return correctAnswers;
}

// Function to randomize the order of the questions
void randomizeQuestions(vector<Question>& questions) {
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    // Shuffle the questions using the random_shuffle algorithm
    random_shuffle(questions.begin(), questions.end());
}

int main() {
    // Create a vector of Question objects
    vector<Question> questions = {
        {"What is the capital of France?", {"Berlin", "Paris", "London", "Rome", "Madrid"}, 1},
        {"What is 2 + 2?", {"1", "2", "3", "4", "5"}, 3},
        {"What is the largest planet in our solar system?", {"Earth", "Saturn", "Jupiter", "Uranus", "Neptune"}, 2},
        {"What is the smallest country in the world?", {"Monaco", "Vatican City", "Nauru", "Tuvalu", "San Marino"}, 1},
        {"What is the largest living species of lizard?", {"Komodo dragon", "Saltwater crocodile", "Black mamba", "Green anaconda", "African elephant"}, 0}
    };

    // Randomize the order of the questions
    randomizeQuestions(questions);

    int correctAnswers = 0;

    // Display a welcome message and instructions
    cout << "Welcome to the Quiz Game!" << endl;
    cout << "You will be asked 5 questions. Good luck!" << endl;
    
    char playAgain;
        do {

        // Ask each question and keep track of the correct answers
        for (int i = 0; i < 5; i++) {
            string result = askQuestion(questions[i]);
            cout << result << endl;
            if (result == "Correct!") {
                correctAnswers++;
            }
        }
        // Calculate and display the final score
        int score = calculateScore(correctAnswers);
        cout << endl << "Your final score is " << score << " out of 5." << endl;
        cout << "Thanks for playing!" << endl;
        // Ask the user if they want to play again
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
        // Reset the correct answers count for the next game
        correctAnswers = 0;
        // Randomize the questions again for the next game
        randomizeQuestions(questions);
    } while (playAgain == 'y' || playAgain == 'Y');
    return 0;

}
