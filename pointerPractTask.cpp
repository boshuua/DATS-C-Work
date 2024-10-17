#include <iostream>
void swap(int* ptrnum, int* ptrnum2)
{
    int temp = *ptrnum; // * = value of pointer
    
    *ptrnum = *ptrnum2;
    
    *ptrnum2 = temp;
}
int main() {
    int ptrnum = 7, ptrnum2 = 12;
    
    std::cout << "Before Swapping x = " << ptrnum << ", y = " << ptrnum2 << std::endl;
    
    swap(&ptrnum, &ptrnum2);
    
    std::cout << "After Swapping x = " << ptrnum << ", y = " << ptrnum2 << std::endl;
}
