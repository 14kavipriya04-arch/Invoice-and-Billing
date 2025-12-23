#include "billing.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility>
#include <ctime>
#include <sstream>

using namespace std;

vector<Item> items;
vector<User> users;
int nextUserId = 1;

/* ---------- INIT ITEMS ---------- */
void initItems() {
    items.push_back({1, "Rice", 50});
    items.push_back({2, "Milk", 30});
    items.push_back({3, "Bread", 25});
    items.push_back({4, "Oil", 120});
}

/* ---------- SHOW ITEMS ---------- */
void showItems() {
    cout << "\nAvailable Items:\n";
    cout << left << setw(5) << "ID"
         << setw(15) << "Item"
         << "Price\n";
    for (auto &i : items)
        cout << setw(5) << i.id << setw(15) << i.name << i.price << endl;
}

/* ---------- USER FUNCTIONS ---------- */
void addUser() {
    User u;
    u.id = nextUserId++;

    cout << "Enter user name: ";
    cin.ignore();
    getline(cin, u.name);

    cout << "Enter phone number: ";
    getline(cin, u.phone);

    users.push_back(u);
    cout << "User added. ID: " << u.id << endl;
    saveUsers();

}

void deleteUser() {
    int id;
    cout << "Enter user ID to delete: ";
    cin >> id;

    for (auto it = users.begin(); it != users.end(); it++) {
        if (it->id == id) {
            users.erase(it);
            cout << "User deleted\n";
            return;
        }
    }
    cout << "Invalid user\n";
    saveUsers();

}

void showUsers() {
    cout << "\nUsers List:\n";
    for (auto &u : users)
        cout << "ID: " << u.id
             << " | Name: " << u.name
             << " | Phone: " << u.phone << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

/* ---------- BILLING ---------- */
void generateBill() {
    if (users.empty()) {
        cout << "No users available\n";
        return;
    }

    showUsers();
    int uid;
    cout << "Select user ID: ";
    cin >> uid;

    User *currentUser = nullptr;
    for (auto &u : users)
        if (u.id == uid)
            currentUser = &u;

    if (!currentUser) {
        cout << "Invalid user\n";
        return;
    }

    int invoiceNo = generateInvoiceNumber();
    string dateTime = getCurrentDateTime();

    vector<pair<Item, int>> billItems;
    double subtotal = 0;

    while (true) {
        showItems();
        int itemId, qty;
        cout << "Enter item ID (0 to finish): ";
        cin >> itemId;

        if (itemId == 0) break;

        Item *selected = nullptr;
        for (auto &i : items)
            if (i.id == itemId)
                selected = &i;

        if (!selected) {
            cout << "Invalid item\n";
            continue;
        }

        cout << "Enter quantity: ";
        cin >> qty;
        if (qty <= 0) {
            cout << "Invalid quantity\n";
            continue;
        }

        billItems.push_back({*selected, qty});
        subtotal += selected->price * qty;
    }

    double gst = subtotal * 0.18;
    double discount = 20;
    double total = subtotal + gst - discount;

    /* ---------- DISPLAY ---------- */
    system("cls");   

    cout << "\nInvoice No : " << invoiceNo << endl;
    cout << "Date & Time: " << dateTime << endl;
    cout << "Customer  : " << currentUser->name << endl;
    cout << "Phone     : " << currentUser->phone << endl;

    cout << "\n=========== INVOICE ===========\n";
    cout << left << setw(10) << "Item"
         << setw(10) << "Qty"
         << setw(10) << "Price"
         << setw(10) << "Total\n";
    cout << "--------------------------------\n";

    for (auto &b : billItems) {
        cout << left << setw(10) << b.first.name
             << setw(10) << b.second
             << setw(10) << b.first.price
             << setw(10) << b.first.price * b.second << endl;
    }

    cout << "--------------------------------\n";
    cout << "Subtotal : " << subtotal << endl;
    cout << "GST (18%): " << gst << endl;
    cout << "Discount : " << discount << endl;
    cout << "TOTAL    : " << total << endl;
    cout << "================================\n";

    /* ---------- SAVE FILES ---------- */
    ofstream history("history.txt", ios::app);

    string invoiceFileName = "invoice_" + to_string(invoiceNo) + ".txt";
    ofstream invoiceFile(invoiceFileName);

    history << "\nInvoice No : " << invoiceNo << "\n";
    history << "Date & Time: " << dateTime << "\n";
    history << "Customer  : " << currentUser->name << "\n";
    history << "Phone     : " << currentUser->phone << "\n";

    invoiceFile << "=========== INVOICE ==========\n";
    invoiceFile << "Invoice No : " << invoiceNo << "\n";
    invoiceFile << "Date & Time: " << dateTime << "\n";
    invoiceFile << "Customer  : " << currentUser->name << "\n";
    invoiceFile << "Phone     : " << currentUser->phone << "\n";

    for (auto &b : billItems) {
        history << b.first.name << " "
                << b.second << " x "
                << b.first.price << " = "
                << b.first.price * b.second << "\n";

        invoiceFile << b.first.name << " "
                    << b.second << " x "
                    << b.first.price << " = "
                    << b.first.price * b.second << "\n";
    }

    history << "TOTAL: " << total << "\n";
    history << "-----------------------------\n";

    invoiceFile << "TOTAL: " << total << "\n";
    invoiceFile << "=============================\n";

    history.close();
    invoiceFile.close();
    cout << "================================\n";

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();

    }

/* ---------- HISTORY ---------- */
void showHistory() {
    system("cls"); 
    ifstream file("history.txt");
    if (!file) {
        cout << "No history found\n";
        return;
    }

    cout << "\n===== BILL HISTORY =====\n";
    string line;
    while (getline(file, line))
        cout << line << endl;
    file.close();

    cout << "================================\n";

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();

    }

/* ---------- UTIL ---------- */
int generateInvoiceNumber() {
    ifstream in("invoice_no.txt");
    int last = 1000;
    if (in >> last) in.close();

    int next = last + 1;
    ofstream out("invoice_no.txt");
    out << next;
    out.close();

    return next;
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << (1 + ltm->tm_mon) << "-"
       << ltm->tm_mday << " "
       << ltm->tm_hour << ":"
       << ltm->tm_min << ":"
       << ltm->tm_sec;

    return ss.str();
}

void loadUsers() {
    ifstream file("users.txt");
    if (!file) return;

    users.clear();
    nextUserId = 1;

    string line;
    while (getline(file, line)) {
        User u;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        u.id = stoi(line.substr(0, p1));
        u.name = line.substr(p1 + 1, p2 - p1 - 1);
        u.phone = line.substr(p2 + 1);

        users.push_back(u);

        if (u.id >= nextUserId)
            nextUserId = u.id + 1;
    }
    file.close();
}
void saveUsers() {
    ofstream file("users.txt");
    for (auto &u : users) {
        file << u.id << "|"
             << u.name << "|"
             << u.phone << "\n";
    }
    file.close();
}
