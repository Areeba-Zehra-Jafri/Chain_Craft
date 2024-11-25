
// #include "CChain.h"
// #include "CBlock.h"
// #include "Transaction.h"
// #include "Wallet.h"
// #include "RSA.h"
// #include "login.h" // Include login header
// #include <iostream>
// #include <vector>
// #include <string>

// using namespace std;



// int main() {
//     // Display login system and allow user to access the blockchain simulation
//     Login loginSystem("users.txt"); // Initialize login system with file name
//     loginSystem.displayMenu();      // Display the main menu

//     return 0;
// }
#include "CChain.h"
#include "CBlock.h"
#include "Transaction.h"
#include "Wallet.h"
#include "RSA.h"
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <conio.h> // For getch() to hide password input
using namespace std;

// Function to hide password input and validate it
string inputPassword() {
    string password;
    char ch;
    cout << "Enter password (at least 1 letter, 1 number, 1 special character): ";
    while ((ch = getch()) != '\r') { // '\r' is Enter key
        if (ch == '\b' && !password.empty()) { // Handle backspace
            cout << "\b \b";
            password.pop_back();
        } else if (ch != '\b') { 
            password += ch;
            cout << '*';
        }
    }
    cout << endl;

    // Validation
    regex validPassword("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[@$!%*?&#])[A-Za-z\\d@$!%*?&#]{6,}$");
    if (!regex_match(password, validPassword)) {
        cout << "Invalid password. Try again.\n";
        return inputPassword();
    }
    return password;
}

// Function for login/signup
Wallet* loginOrSignup(vector<Wallet*>& wallets, const string& walletsFile) {
    while (true) {
        cout << "1- Login\n2- Signup\nChoose an option: ";
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice == 1) { // Login
            cout << "Enter username: ";
            string username;
            getline(cin, username);
            cout << "Enter password: ";
            string password = inputPassword();

            for (auto wallet : wallets) {
                if (wallet->getId() == username && wallet->getPassword() == password) {
                    cout << "Login successful!\n";
                    return wallet;
                }
            }
            cout << "Invalid username or password. Try again.\n";
        } else if (choice == 2) { // Signup
            cout << "Enter username: ";
            string username;
            getline(cin, username);
            string password = inputPassword();

            Wallet* newWallet = new Wallet(username, password);
            newWallet->setBalance(100); // Default initial balance
            wallets.push_back(newWallet);
            cout << "Signup successful! Welcome, " << username << ".\n";
            return newWallet;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }
}

// Function to display main menu and perform blockchain operations
void blockchainMenu(Wallet* user, Blockchain& myBlockchain, vector<Wallet*>& wallets, const string& walletsFile) {
    while (true) {
        cout << "\n1- Perform a Transaction\n2- Mine a Block\n3- Print Blockchain\n4- Logout\nChoose an option: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) { // Perform a transaction
            cout << "Enter Username of receiver: ";
            string receiver;
            getline(cin, receiver);
            cout << "Enter amount: ";
            int amount;
            cin >> amount;
            cin.ignore();

            Wallet* receiverWallet = nullptr;
            for (auto wallet : wallets) {
                if (wallet->getId() == receiver) {
                    receiverWallet = wallet;
                    break;
                }
            }

            if (receiverWallet) {
                Transaction tx = user->sendFunds(*receiverWallet, amount);
                myBlockchain.createTransaction(tx);
                cout << "Transaction added to the pending transactions.\n";
            } else {
                cout << "Receiver not found.\n";
            }
        } else if (choice == 2) { // Mine a block
            cout << "Mining..........\n";
            myBlockchain.minePendingTransactions();
            cout << "Mining successful!\n";
            myBlockchain.notifyWallets(wallets);
        } else if (choice == 3) { // Print blockchain
            myBlockchain.printChain();
        } else if (choice == 4) { // Logout
            user->saveAllToFile(wallets, walletsFile);
            cout << "Logged out successfully.\n";
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }
}

int main() {
    const string walletsFile = "wallets.dat";

    // Load wallets
    Wallet tempWallet("temp","temp");
    vector<Wallet*> wallets = tempWallet.loadAllFromFile(walletsFile);

    Blockchain myBlockchain;

    while (true) {
        cout << "Welcome to ChainCraft\nS- Start\nE- Exit\nChoose an option: ";
        char option;
        cin >> option;
        cin.ignore();

        if (option == 'S' || option == 's') {
            Wallet* user = loginOrSignup(wallets, walletsFile);
            blockchainMenu(user, myBlockchain, wallets, walletsFile);
        } else if (option == 'E' || option == 'e') {
            cout << "Exiting ChainCraft. Goodbye!\n";
            break;
        } else {
            cout << "Invalid option. Try again.\n";
        }
    }

    // Clean up dynamically allocated wallets
    for (auto wallet : wallets) {
        delete wallet;
    }

    return 0;
}
