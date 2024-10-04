// Reference to the validation used | https://www.hackerearth.com/practice/notes/validating-user-input-in-c/ 

// Implement header files
#include <iostream>
#include <limits>

int main() {
    // Set the datatypes to variables 
    int n; // Number of grades
    float gradNum[10], sum = 0.0, average, highest, lowest, threshold; // Array to store grades and variables for calculations

    // Display the program title
    std::cout << "***** GRADE CALCULATOR *****" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // Prompt the user to enter the number of grades
    std::cout << "Enter how many grades you want to store: ";

    // Input validation for number of grades
    while (!(std::cin >> n) || n < 1 || n > 10) {
        std::cout << "ERROR! Number of grades should be in range of (1-10)." << std::endl;
        std::cout << "Enter how many grades you want to store again: ";
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
    }

    // Display a separator
    std::cout << "-------------------------------" << std::endl;

    // Loop to input each grade
    for (int i = 0; i < n; ++i) {
        // Prompt the user to enter a grade
        std::cout << i + 1 << ". Enter grade: ";

        // Input validation for each grade
        while (!(std::cin >> gradNum[i])) {
            std::cout << "Invalid input. Please enter a valid grade: ";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
        }

        // Additional validation: grade should be in range of (0-100)
        while (gradNum[i] < 0 || gradNum[i] > 100) {
            std::cout << "Invalid input. Grade should be in range of (0-100).\nPlease enter the correct grade number:  ";
            std::cin >> gradNum[i];
        }

        // Add the grade to the sum
        sum += gradNum[i];
    }

    // Calculate the average
    average = sum / n;

    // Display the average
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Average = " << average << std::endl;

    // Display the grades entered
    std::cout << "Grades entered: ";
    for (int i = 0; i < n; ++i) {
        std::cout << gradNum[i] << " ";
    }
    std::cout << std::endl;

    // Initialize the highest and lowest grades
    highest = gradNum[0];
    lowest = gradNum[0];

    // Loop to find the highest and lowest grades
    for (int i = 1; i < n; ++i) {
        if (gradNum[i] > highest)
            highest = gradNum[i];
        if (gradNum[i] < lowest)
            lowest = gradNum[i];
    }

    // Display the highest and lowest grades
    std::cout << "Highest grade = " << highest << std::endl;
    std::cout << "Lowest grade = " << lowest << std::endl;

    // Prompt the user to enter a grade threshold
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Enter a grade threshold (1-100): ";

    // Input validation for the threshold
    while (!(std::cin >> threshold) || threshold < 1 || threshold > 100) {
        std::cout << "ERROR! Threshold should be in range of (1-100)." << std::endl;
        std::cout << "Enter a grade threshold again: ";
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
    }

    // Display the grades above the threshold
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Grades above set threshold: " << threshold << std::endl;
    for(int i = 0; i < n; ++i)
    {
        if(gradNum[i] > threshold)
            std::cout << "Grade " << i + 1 << ": " << gradNum[i] << std::endl;
    }

    return 0;
}
