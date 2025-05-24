#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>
#include <cctype>
#include <fstream>
#include "AccountBase.h"

using namespace std;

class BankAccount : public AccountBase{
private:
    string accountHolder;
    int accountId;
    double balance;
    vector<string> transactionHistory;

public:
    BankAccount(const string& name, int id); //constructor

    void deposit(double amount); //deposit func
    void withdraw(double amount); //withdraw func
	void displayAccountDetails() const; //displays the account details
	void displayTransactionHistory() const; //displays the transaction history

	int getId() const; //gets the account ID
	const string& getName() const; //gets the account holder's name
	double getBalance() const; //gets the account balance
	const vector<string>& getTransactionHistory() const; //gets the transaction history

	void setBalance(double newBalance); // sets the account balance
	void setTransactionHistory(const vector<string>& history); // sets the transaction history
};
