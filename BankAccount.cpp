#include "BankAccount.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>
#include <cctype>
#include <fstream>
using namespace std;

BankAccount::BankAccount(const string& name, int id)
    : accountHolder(name), accountId(id), balance(0.0) {
}
// Constructor initializes account holder's name, ID, and balance

void BankAccount::deposit(double amount) {
    if (amount > 0 && amount <= 10000) {
        balance += amount;
        transactionHistory.push_back("Deposited: $" + to_string(amount));
        cout << "Deposit successful!\n";
    }
    else {
        cout << "Invalid deposit amount. Max $10,000.\n";
    }
}
// Deposits money into the account, ensuring it does not exceed $10,000

void BankAccount::withdraw(double amount) {
    if (amount > 0 && amount <= balance && amount <= 20000) {
        balance -= amount;
        transactionHistory.push_back("Withdrew: $" + to_string(amount));
        cout << "Withdrawal successful!\n";
    }
    else {
        cout << "Invalid withdrawal. Max $20,000 or insufficient funds.\n";
    }
}
// Withdraws money from the account, ensuring it does not exceed $20,000 or the current balance

void BankAccount::displayAccountDetails() const {
    cout << "\n===== Account Details =====\n";
    cout << "Account Holder: " << accountHolder << "\n";
    cout << "Account ID: " << accountId << "\n";
    cout << "Balance: $" << balance << "\n";
    cout << "===========================\n";
}
// Displays the account details including holder's name, ID, and balance

void BankAccount::displayTransactionHistory() const {
    std::cout << "\n===== Transaction History =====\n";
    for (const auto& tx : transactionHistory) {
        std::cout << tx << "\n";
    }
    std::cout << "Total Balance: $" << balance << "\n";
}
// Displays the transaction history along with the current balance

int BankAccount::getId() const { return accountId; } // Gets the account ID
const string& BankAccount::getName() const { return accountHolder; } // Gets the account holder's name
double BankAccount::getBalance() const { return balance; } // Gets the account balance
const vector<string>& BankAccount::getTransactionHistory() const { return transactionHistory; } // Gets the transaction history

void BankAccount::setBalance(double newBalance) { balance = newBalance; } // Sets the account balance
void BankAccount::setTransactionHistory(const vector<string>& history) {
    transactionHistory = history;
} // Sets the transaction history
