#include "CChain.h"
#include "CBlock.h"
#include "Transaction.h"
#include "Wallet.h"
#include "login.h"
#include "RSA.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string username, password;
    int choice;
    bool isAuthenticated = false;

    cout << "\n--- Welcome to the Blockchain System ---\n";
    cout << "1. Register\n2. Login\nChoose an option: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer

    // Register flow
    if (choice == 1) {
        cout << "Enter username: ";
        getline(cin, username);

        // Get hidden password input
        password = Login::getHiddenPassword();

        // Register user
        if (Login::registerUser(username, password)) {
            isAuthenticated = true;
            cout << "User registered successfully.\n";
        }
    }
    // Login flow
    else if (choice == 2) {
        while (!isAuthenticated) {
            cout << "Enter username: ";
            getline(cin, username);

            // Get hidden password input
            password = Login::getHiddenPassword();

            // Attempt to login
            if (Login::loginUser(username, password)) {
                isAuthenticated = true;
                cout << "Login successful.\n";
            } else {
                cout << "Invalid username or password.\n";
                cout << "Please try again.\n";
            }
        }
    }

    // If authentication fails after trying login (both register and login flow)
    if (!isAuthenticated) {
        cout << "Authentication failed. Please try again.\n";
        return 0;  // Or restart the process if necessary, instead of exiting
    }

    // Blockchain Simulation Section
    cout << "\n--- Starting Blockchain Simulation ---\n";
    Blockchain myBlockchain;

    // Create wallets
    Wallet alice("Alice");
    Wallet bob("Bob");
    Wallet charlie("Charlie");

    // Initialize balances
    alice.setBalance(100);
    bob.setBalance(0);
    charlie.setBalance(0);

    // Create a vector to hold wallet pointers
    vector<Wallet *> wallets = {&alice, &bob, &charlie};

    // First Set of Transactions
    cout << "\n--- First Set of Transactions ---\n";
    Transaction tx1 = alice.sendFunds(bob, 50);
    Transaction tx2 = alice.sendFunds(charlie, 50);

    // Add transactions to blockchain and mine them in a new block
    myBlockchain.createTransaction(tx1);
    myBlockchain.createTransaction(tx2);
    myBlockchain.minePendingTransactions();

    // Update wallet balances after first mining
    myBlockchain.notifyWallets(wallets);

    // Print blockchain and wallet balances
    cout << "\n--- Blockchain After First Mining ---\n";
    myBlockchain.printChain();
    cout << "\n--- Wallet Balances After First Mining ---\n";
    for (const auto &wallet : wallets) {
        wallet->printWalletData();
    }

    cout << endl;

    // Second Set of Transactions
    cout << "\n--- Second Set of Transactions ---\n";
    Transaction tx3 = charlie.sendFunds(alice, 20); // Charlie sends to Alice
    Transaction tx4 = bob.sendFunds(alice, 10);     // Bob sends to Alice

    // Add transactions and mine them in a new block
    myBlockchain.createTransaction(tx3);
    myBlockchain.createTransaction(tx4);
    myBlockchain.minePendingTransactions();

    // Update wallet balances after second mining
    myBlockchain.notifyWallets(wallets);

    // Print blockchain and wallet balances
    cout << "\n--- Blockchain After Second Mining ---\n";
    myBlockchain.printChain();
    cout << "\n--- Wallet Balances After Second Mining ---\n";
    for (const auto &wallet : wallets) {
        wallet->printWalletData();
    }

    // Validate Blockchain Integrity
    cout << "\n--- Blockchain Integrity Check ---\n";
    if (myBlockchain.isChainValid()) {
        cout << "Blockchain is valid.\n";
    } else {
        cout << "Blockchain is not valid!\n";
    }

    return 0;
}
