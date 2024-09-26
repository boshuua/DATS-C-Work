// Online C++ compiler to run C++ program online
#include <iostream>
#include <string>


int main() 
{
    
    int gradeNum = 0; // Declaring Datatype
    std::cout << "Enter your score: "; // Prompting user to int
    std::cin >> gradeNum;
    
    if (gradeNum >= 90)
    {
        std::cout << "Your grade is: A";
    }
    else if (gradeNum >= 80)
    {
        std::cout << "Your grade is : B";
    }
    else if (gradeNum >= 70)
    {
        std::cout << "Your grade is: C";
    }
    else if (gradeNum >= 60)
    {
        std::cout << "Your grade is: D";
    }
    else if(gradeNum < 60)
    {
        std::cout << "Your grade is: F"; 
    }
    return 0;
}
