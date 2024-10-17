#include <iostream>

int main()
{
	int ptrnum = 7;

	int* ptr = &ptrnum;

	std::cout << "Value of the var: " << ptrnum << std::endl;
	std::cout << "Memory Address in Pointer: " << ptr << std::endl;
	std::cout << "Pointer points to value: " << *ptr << std::endl;
}