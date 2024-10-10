#include <iostream>
#include <limits>

// Function to get a valid integer input within a range
int getValidIntInput(int min, int max, const std::string& prompt) {
    int input;
    while (!(std::cin >> input) || input < min || input > max) {
        std::cout << "ERROR! Input should be in range of (" << min << "-" << max << ")." << std::endl;
        std::cout << prompt;
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
    }
    return input;
}

// Function to get a valid float input within a range
float getValidFloatInput(float min, float max, const std::string& prompt) {
    float input;
    while (!(std::cin >> input) || input < min || input > max) {
        std::cout << "ERROR! Input should be in range of (" << min << "-" << max << ")." << std::endl;
        std::cout << prompt;
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
    }
    return input;
}

// Function to calculate the average of an array of grades
float calculateAverage(float grades[], int n) {
    float sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += grades[i];
    }
    return sum / n;
}

// Function to find the highest and lowest grades in an array
void findHighestAndLowest(float grades[], int n, float& highest, float& lowest) {
    highest = grades[0];
    lowest = grades[0];
    for (int i = 1; i < n; ++i) {
        if (grades[i] > highest)
            highest = grades[i];
        if (grades[i] < lowest)
            lowest = grades[i];
    }
}

// Function to display grades above a specified threshold
void displayGradesAboveThreshold(float grades[], int n, float threshold) {
    std::cout << "Grades above set threshold: " << threshold << std::endl;
    for (int i = 0; i < n; ++i) {
        if (grades[i] > threshold)
            std::cout << "Grade " << i + 1 << ": " << grades[i] << std::endl;
    }
}

int main() {
    // Display the program title
    std::cout << "***** GRADE CALCULATOR *****" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // Prompt the user to enter the number of grades
    std::cout << "Enter how many grades you want to store: ";
    int n = getValidIntInput(1, 10, "Enter how many grades you want to store again: ");

    // Display a separator
    std::cout << "-------------------------------" << std::endl;

    // Array to store grades
    float gradNum[10];

    // Loop to input each grade
    for (int i = 0; i < n; ++i) {
        // Prompt the user to enter a grade
        std::cout << i + 1 << ". Enter grade: ";
        gradNum[i] = getValidFloatInput(0, 100, "Please enter the correct grade number: ");
    }

    // Calculate the average
    float average = calculateAverage(gradNum, n);

    // Display the average
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Average = " << average << std::endl;

    // Display the grades entered
    std::cout << "Grades entered: ";
    for (int i = 0; i < n; ++i) {
        std::cout << gradNum[i] << " ";
    }
    std::cout << std::endl;

    // Find the highest and lowest grades
    float highest, lowest;
    findHighestAndLowest(gradNum, n, highest, lowest);

    // Display the highest and lowest grades
    std::cout << "Highest grade = " << highest << std::endl;
    std::cout << "Lowest grade = " << lowest << std::endl;

    // Prompt the user to enter a grade threshold
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Enter a grade threshold (1-100): ";
    float threshold = getValidFloatInput(1, 100, "Enter a grade threshold again: ");

    // Display the grades above the threshold
    displayGradesAboveThreshold(gradNum, n, threshold);

    return 0;
}
