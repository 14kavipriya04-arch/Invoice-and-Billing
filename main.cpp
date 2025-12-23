#include <iostream>
#include "billing.h"

using namespace std;

int main() {
    initItems();
    loadUsers();

    int choice;
    do {

        system("cls"); 
        cout << "\n===== BILLING SYSTEM =====\n";
        cout << "1. Add User\n";
        cout << "2. Delete User\n";
        cout << "3. Show Users\n";
        cout << "4. Generate Bill\n";
        cout << "5. View Bill History\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addUser(); break;
            case 2: deleteUser(); break;
            case 3: showUsers(); break;
            case 4: generateBill(); break;
            case 5: showHistory(); break;
            case 6: cout << "Exiting \n"; break;
            default: cout << "Invalid choice \n";
        }
    } while (choice != 6);

    return 0;
}
