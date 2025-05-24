#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <limits>
#include <cctype>
#include <fstream>
#include "AccountManager.h"
#include "BankAccount.h"
#include "AccountBase.h"

using namespace std;

int main() {
    try {
        AccountManager manager;
        manager.run();
    }
    catch (const std::exception& e) {
        cerr << "Unhandled exception: " << e.what() << "\n";
    }
    catch (...) {
        cerr << "An unknown error occurred.\n";
    }
    return 0;
}
