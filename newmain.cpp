#include "CChain.h"
#include "CBlock.h"
#include "Transaction.h"
#include "Wallet.h"
#include "RSA.h"
#include "login.h" // Include login header
#include <iostream>
#include <vector>
#include <string>

using namespace std;


void loadBlockchain(Blockchain &myBlockchain, const string &blockchainFile) {
    cout << "\n--- Loading Blockchain from File ---\n";
    char choice;
    cout << "Do you want to load the blockchain from the file? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        if (myBlockchain.loadFromFile(blockchainFile)) {
            cout << "Blockchain successfully loaded from file.\n";
        } else {
            cout << "Failed to load blockchain. Starting with a new blockchain.\n";
        }
    } 
}

void saveBlockchain(const Blockchain &myBlockchain, const string &blockchainFile) {
    cout << "\n--- Save Blockchain to File ---\n";
    if (myBlockchain.saveToFile(blockchainFile)) {
        cout << "Blockchain successfully saved to file.\n";
    } else {
        cout << "Failed to save blockchain to file.\n";
    }
}


int main() {
    // Display login system and allow user to access the blockchain simulation
    Login loginSystem("users.txt"); // Initialize login system with file name
    loginSystem.displayMenu();      // Display the main menu

    // Blockchain simulation after user login
    cout << "\n--- Blockchain Simulation ---\n";
    cout << "Welcome to Blockchain Simulation!" << endl;
    Blockchain myBlockchain;

    // File for storing the blockchain
    const string blockchainFile = "blockchain_data.txt";

    // Load blockchain at the start
    loadBlockchain(myBlockchain, blockchainFile);

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

    // First set of transactions
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
    myBlockchain.printChain();
    cout << "Wallet Balances after First Mining:\n";
    for (const auto &wallet : wallets) {
        wallet->printWalletData();
    }

    cout << endl;

    // Second set of transactions
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
    myBlockchain.printChain();
    cout << "Wallet Balances after Second Mining:\n";
    for (const auto &wallet : wallets) {
        wallet->printWalletData();
    }

    // Validate blockchain integrity
    if (myBlockchain.isChainValid()) {
        cout << "Blockchain is valid.\n";
    } else {
        cout << "Blockchain is not valid!\n";
    }

    // Save blockchain before exiting
   saveBlockchain(myBlockchain, blockchainFile);

    return 0;
}
