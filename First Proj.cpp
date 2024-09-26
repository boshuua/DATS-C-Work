
#include <iostream>
#include <string>

int main() 
{

    std::string name;
    int age;
    float height;
    
    std::cout << "PLEASE ENTER YOUR NAME: ";
    std::cin >> name;
    
    std::cout << "\nPLEASE ALSO ENTER YOUR AGE: ";
    std::cin >> age;
    
    std::cout << "\nPLEASE ENTER YOUR HEIGHT: ";
    std::cin >> height;
    
    std::cout << "\nHello " << name << std::endl;
    std::cout << "\nYour are " << age << " years old and your height is " << height << " meters." << std::endl;
    return 0;
}
