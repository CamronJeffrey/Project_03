#include "AccountManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>
#include <cctype>
#include <fstream>
#include "BankAccount.h"

using namespace std;


void AccountManager::clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
} 
//this will clear any given invaild input

int AccountManager::safeIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Enter a number.\n";
        clearInput();
    }
}
// this allows the user input an integer as an input 

double AccountManager::safeDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Enter a valid amount.\n";
        clearInput();
    }
}
// this allows the user input a double as an input 

bool AccountManager::loginScreen() {
    string name;
    int password;
    char choice;

    cout << "Do you have an account? (y/n): ";
    while (!(cin >> choice) || !(choice == 'y' || choice == 'n' || choice == 'Y' || choice == 'N')) {
        cout << "Invalid input. Please enter 'y' or 'n': ";
        clearInput();
    }
    clearInput();

    if (tolower(choice) == 'n') {
        cout << "Enter new account holder name: ";
        getline(cin, name);
        if (name == "admin") {
            cout << "Cannot use reserved username.\n";
            return false;
        }
        password = safeIntInput("Set a numeric password: ");
        accounts[name] = make_unique<BankAccount>(name, password);
        cout << "Account created!\n";
    }

    cout << "\nLogin to your account\nUsername: ";
    getline(cin, name);
    password = safeIntInput("Password: ");

    if (name == "admin" && password == 9999) {
        currentAccount = nullptr;
        return true;
    }

    auto it = accounts.find(name);
    if (it != accounts.end() && it->second->getId() == password) {
        currentAccount = shared_ptr<BankAccount>(it->second.get(), [](BankAccount*) {});
        return true;
    }

    cout << "Login failed. Check username/password.\n";
    return false;
}
// this helps the user login into their account or allows them to create another account

void AccountManager::userMenu() {
    enum UserChoice { DEPOSIT = 1, WITHDRAW, DETAILS, HISTORY, LOGOUT };
    int choice;

    do {
        cout << "\nBank System - " << currentAccount->getName();
        cout << "\n1. Deposit\n2. Withdraw\n3. Account Details\n4. Transaction History\n5. Logout\n";
        choice = safeIntInput("Enter choice: ");
        system("cls");

        switch (choice) {
        case DEPOSIT:
            currentAccount->deposit(safeDoubleInput("Deposit amount: "));
            break;
        case WITHDRAW:
            currentAccount->withdraw(safeDoubleInput("Withdraw amount: "));
            break;
        case DETAILS:
            currentAccount->displayAccountDetails();
            break;
        case HISTORY:
            currentAccount->displayTransactionHistory();
            break;
        case LOGOUT:
            currentAccount = nullptr;
            cout << "Logged out.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != LOGOUT);
}
// displays the options the user has in there account

void AccountManager::adminMenu() {
    int choice;
    do {
        std::cout << "\n===== Admin Panel =====";
        std::cout << "\n1. View All Accounts";
        std::cout << "\n2. Logout";
        choice = safeIntInput("\nEnter your choice: ");

        system("cls"); // optional, clears the terminal

        switch (choice) {
        case 1:
            std::cout << "\n--- All Account Details ---\n";
            for (const auto& pair : accounts) {
                const std::string& username = pair.first;
                const AccountBase* accPtr = pair.second.get(); // ✅ this was missing

                std::cout << "\nUser: " << username << "\n";
                std::cout << "ID/Password: " << accPtr->getId() << "\n";
                std::cout << "Balance: $" << accPtr->getBalance() << "\n";
                std::cout << "Transactions:\n";
                for (const std::string& tx : accPtr->getTransactionHistory()) {
                    std::cout << "  - " << tx << "\n";
                }
                std::cout << "-------------------------\n";
            }
            break;
        case 2:
            std::cout << "Logging out of Admin Panel...\n";
            break;
        default:
            std::cout << "Invalid option.\n";
        }
    } while (choice != 2);
}
// this runs the admin panel that allows the user to see all the accounts

void AccountManager::run() {
    loadAccountsFromFiles(); // Load accounts when program starts

    while (true) {
        system("cls");
        if (loginScreen()) {
            if (!currentAccount)
                adminMenu();
            else
                userMenu();

            saveAccountsToFiles(); // Save after logout
        }
        else {
            char retry;
            cout << "Retry login? (y/n): ";
            while (!(cin >> retry) || !(retry == 'y' || retry == 'n' || retry == 'Y' || retry == 'N')) {
                cout << "Invalid input. 'y' or 'n': ";
                clearInput();
            }
            if (tolower(retry) != 'y') break;
        }
    }
}
// helps run the main loop

void AccountManager::saveAccountsToFiles() {
    ofstream textOut("accounts.txt");
    ofstream binOut("accounts.bin", ios::binary);

    if (!textOut || !binOut) {
        cerr << "Error opening file(s) for writing.\n";
        return;
    }

    for (const auto& [username, acc] : accounts) {
        // Write to text file
        textOut << username << '\n'
            << acc->getId() << '\n'
            << acc->getBalance() << '\n';

        const auto& history = acc->getTransactionHistory();
        textOut << history.size() << '\n';
        for (const auto& tx : history) {
            textOut << tx << '\n';
        }

        // Write to binary file
        size_t nameLen = username.size();
        binOut.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        binOut.write(username.c_str(), nameLen);

        int id = acc->getId();
        double balance = acc->getBalance();
        binOut.write(reinterpret_cast<char*>(&id), sizeof(id));
        binOut.write(reinterpret_cast<char*>(&balance), sizeof(balance));

        size_t txCount = history.size();
        binOut.write(reinterpret_cast<char*>(&txCount), sizeof(txCount));
        for (const auto& tx : history) {
            size_t txLen = tx.size();
            binOut.write(reinterpret_cast<char*>(&txLen), sizeof(txLen));
            binOut.write(tx.c_str(), txLen);
        }
    }

    textOut.close();
    binOut.close();
    cout << "Accounts saved to accounts.txt and accounts.bin\n";
}
// Saves all accounts to files

void AccountManager::loadAccountsFromFiles() {
    ifstream binIn("accounts.bin", ios::binary);
    if (!binIn) {
        cerr << "Binary file not found. Starting fresh.\n";
        return;
    }

    while (binIn.peek() != EOF) {
        // Read username
        size_t nameLen;
        binIn.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        string username(nameLen, '\0');
        binIn.read(&username[0], nameLen);

        // Read ID and balance
        int id;
        double balance;
        binIn.read(reinterpret_cast<char*>(&id), sizeof(id));
        binIn.read(reinterpret_cast<char*>(&balance), sizeof(balance));

        // Read transaction history
        size_t txCount;
        binIn.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
        vector<string> history;
        for (size_t i = 0; i < txCount; ++i) {
            size_t txLen;
            binIn.read(reinterpret_cast<char*>(&txLen), sizeof(txLen));
            string tx(txLen, '\0');
            binIn.read(&tx[0], txLen);
            history.push_back(tx);
        }

        // Recreate the account
        auto acc = make_unique<BankAccount>(username, id);
        acc->setBalance(balance);
        acc->setTransactionHistory(history);
        accounts[username] = move(acc);
    }

    binIn.close();
    cout << "Accounts loaded from accounts.bin\n";
}
// Loads accounts from files


