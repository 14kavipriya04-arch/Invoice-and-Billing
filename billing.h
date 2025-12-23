#ifndef BILLING_H
#define BILLING_H

#include <string>
#include <vector>

struct Item {
    int id;
    std::string name;
    double price;
};

struct User {
    int id;
    std::string name;
    std::string phone;
};


/* functions */
void initItems();
void addUser();
void deleteUser();
void showUsers();
void generateBill();
void showHistory();
void loadUsers();
void saveUsers();

int generateInvoiceNumber();
std::string getCurrentDateTime();

#endif
