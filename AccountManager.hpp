#pragma once
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

// Manages user and admin interaction with the banking system
class AccountManager {
private:
    unordered_map<string, unique_ptr<BankAccount>> accounts; // this Stores all the bank accounts
    shared_ptr<BankAccount> currentAccount; // this Points to the currently logged-in user

public:
    bool loginScreen(); // this helps the user login into their account 
    //or allows them to create another account
    void userMenu(); // displays the options the user has in there account
    void adminMenu(); // this runs the admin panel that allows the user to see all the accounts
    void run(); // helps run the main loop

private:
    void clearInput(); //this will clear any given invaild input
    int safeIntInput(const string& prompt); // this allows the user input an integer as an input 
    double safeDoubleInput(const string& prompt); // this allows the user input a double as an input 
	void saveAccountsToFiles(); // Saves all accounts to files
	void loadAccountsFromFiles(); // Loads accounts from files
};

