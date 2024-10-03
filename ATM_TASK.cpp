#include <iostream>
 
int main() {
    
    double balance  = 1000;
    double amount; 
    int choice;
    
    do {
      std::cout << "****ATM MACHINE****\n";
      std::cout << "1. Check Balance:\n";
      std::cout << "2. Deposit Money:\n";
      std::cout << "3. Withdraw Money:\n";
      std::cout << "4. Exit:\n";
      std::cout << "Enter your own choice: \n";
      std::cin >> choice; 
      
      switch (choice) {
          case 1:
            std::cout << "Displaying Balance: \n";
            std::cout << "Balance = " << balance << "\n";
            break;
          case 2:
            std::cout << "Enter Amount to be deposited \n";
            std::cin >> amount; 
            balance += amount;
            std::cout << "Deposit Successfull! New Balance is: " << balance << std::endl;
            break;
          case 3:
            std::cout << "Enter Amount to withdraw: \n";
            std::cin >> amount;
            balance -= amount;
            std::cout << "Withdrawal successful! " << "\n" << "New Balance is : " << balance << std::endl;
            break;
          case 4:
            break;
      }
    } while (choice != 4);
    
    return 0;
}
