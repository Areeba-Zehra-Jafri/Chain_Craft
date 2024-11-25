
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
        cout << "\033[31mInvalid password. Try again.\n\033[0m";
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
            cout<<"------------------------\n";
            cout << "\nEnter username: ";
            string username;
            getline(cin, username);
            cout<<"------------------------\n";
            cout << "\nEnter password: ";
            string password = inputPassword();
            cout<<"------------------------\n";

            for (auto wallet : wallets) {
                if (wallet->getId() == username && wallet->getPassword() == password) {
                    cout<<"--------------------------------------\n";
                    cout << "\033[34mLogin successful!\033[0m\n";
                    cout<<"---------------------------------------\n";
                    return wallet;
                }
            }
            cout << "\033[31mInvalid username or password. Try again.\033[0m\n";
        } else if (choice == 2) { // Signup
        cout<<"--------------------------\n";
            cout << "\nEnter username: ";
            string username;
            getline(cin, username);
            string password = inputPassword();
            cout<<"------------------------\n";

            Wallet* newWallet = new Wallet(username, password);
            newWallet->setBalance(100); // Default initial balance
            wallets.push_back(newWallet);
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            cout << "\033[34mSignup successful! Welcome, " << username << ".\033[0m\n";
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            return newWallet;
        } else {
            cout << "\033[31mInvalid option. Try again.\033[0m\n";
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
           cout<<"-----------------------------------\n";
            cout << "\nEnter Username of receiver: ";
            string receiver;
            getline(cin, receiver);
             cout<<"----------------------------------\n";
             cout << "\nEnter amount: ";
             int amount;
             cin >> amount;
             cout<<"----------------------------------\n";
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
                cout << "\033[34m--------------------------------------------------\033[0m\n";
                cout << "\033[34mTransaction added to the pending transactions.\033[0m\n";
                cout << "\033[34m--------------------------------------------------\033[0m\n";
            } else {
                cout << "\033[31mReceiver not found.\n\033[0m";
            }
        } else if (choice == 2) { // Mine a block
            cout << "\033[33mMining..........\033[0m\n";
            myBlockchain.minePendingTransactions();
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            cout << "\033[34mMining successful!\033[0m\n";
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            myBlockchain.notifyWallets(wallets);
        } else if (choice == 3) { // Print blockchain
            myBlockchain.printChain();
        } else if (choice == 4) { // Logout
            user->saveAllToFile(wallets, walletsFile);
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            cout << "\033[34mLogged out successfully.\033[0m\n";
            cout << "\033[34m--------------------------------------------------\033[0m\n";
            break;
        } else {
            cout << "\033[31mInvalid option. Try again.\033[0m\n";
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
cout << "          ######  ##     ##    ###    #### ##    ##    ######  ########     ###    ######## ########  \n";
cout << "         ##    ## ##     ##   ## ##    ##  ###   ##   ##    ## ##     ##   ## ##   ##          ##     \n";
cout << "         ##       ##     ##  ##   ##   ##  ####  ##   ##       ##     ##  ##   ##  ##          ##     \n";
cout << "         ##       ######### ##     ##  ##  ## ## ##   ##       ########  ##     ## ######      ##     \n";
cout << "         ##       ##     ## #########  ##  ##  ####   ##       ##   ##   ######### ##          ##     \n";
cout << "         ##    ## ##     ## ##     ##  ##  ##   ###   ##    ## ##    ##  ##     ## ##          ##     \n";
cout << "          ######  ##     ## ##     ## #### ##    ##    ######  ##     ## ##     ## ##          ##     \n";
                           
          cout<<"============================================================================================\n";
        cout << "\nS- Start\nE- Exit\nChoose an option: ";
        char option;
        cin >> option;
        cin.ignore();

        if (option == 'S' || option == 's') {
            Wallet* user = loginOrSignup(wallets, walletsFile);
            blockchainMenu(user, myBlockchain, wallets, walletsFile);
        } else if (option == 'E' || option == 'e') {
            cout << "\033[33mExiting ChainCraft. Goodbye!\033[0m\n";
            break;
        } else {
            cout << "\033[31mInvalid option. Try again.\033[0m\n";
        }
    }

    // Clean up dynamically allocated wallets
    for (auto wallet : wallets) {
        delete wallet;
    }

    return 0;
}
