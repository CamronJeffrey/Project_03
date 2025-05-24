#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>
#include <cctype>
#include <fstream>

using namespace std;

class AccountBase {
public:
	virtual ~AccountBase() = default; // Virtual destructor 

    virtual void deposit(double amount) = 0; 
    virtual void withdraw(double amount) = 0;
    virtual void displayAccountDetails() const = 0;
    virtual void displayTransactionHistory() const = 0;

    virtual int getId() const = 0;
    virtual const string& getName() const = 0;
    virtual double getBalance() const = 0;
    virtual const vector<string>& getTransactionHistory() const = 0;

    virtual void setBalance(double newBalance) = 0;
    virtual void setTransactionHistory(const vector<string>& history) = 0;
};
