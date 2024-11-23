// #include "CChain.h"
// #include "CBlock.h"
// #include "Transaction.h"
// #include "Wallet.h"
// #include "RSA.h"
// #include <iostream>
// #include <vector>
// #include <string>
// using namespace std;

// int main()
// {
//     cout << "Hello Blockchain Simulation: " << endl;
//     Blockchain myBlockchain;

//     // Create wallets
//     Wallet alice("Alice");
//     Wallet bob("Bob");
//     Wallet charlie("Charlie");

//     // Initialize balances
//     alice.setBalance(100);
//     bob.setBalance(0);
//     charlie.setBalance(0);

//     // Create a vector to hold wallet pointers
//     vector<Wallet *> wallets = {&alice, &bob, &charlie};

//     // First set of transactions
//     cout << "\n--- First Set of Transactions ---\n";
//     Transaction tx1 = alice.sendFunds(bob, 50);
//     Transaction tx2 = alice.sendFunds(charlie, 50);
    
//     // Add transactions to blockchain and mine them in a new block
//     myBlockchain.createTransaction(tx1);
//     myBlockchain.createTransaction(tx2);
//     myBlockchain.minePendingTransactions();

//     // Update wallet balances after first mining
//     myBlockchain.notifyWallets(wallets);

//     // Print blockchain and wallet balances
//     myBlockchain.printChain();
//     cout << "Wallet Balances after First Mining:\n";
//     for (const auto& wallet : wallets) {
//         wallet->printWalletData();
//     }

//     cout<<endl;
    
//     // Second set of transactions
//     cout << "\n--- Second Set of Transactions ---\n";
//     Transaction tx3 = charlie.sendFunds(alice, 20);  // Charlie sends to Alice
//     Transaction tx4 = bob.sendFunds(alice, 10);      // Bob sends to Alice
//     //Transaction tx5 = alice.sendFunds(bob, 15);      // Alice sends to Bob

//     // Add transactions and mine them in a new block
//     myBlockchain.createTransaction(tx3);
//     myBlockchain.createTransaction(tx4);
//     //myBlockchain.createTransaction(tx5);
//     myBlockchain.minePendingTransactions();

//     // Update wallet balances after second mining
//     myBlockchain.notifyWallets(wallets);

//     // Print blockchain and wallet balances
//     myBlockchain.printChain();
//     cout << "Wallet Balances after Second Mining:\n";
//     for (const auto& wallet : wallets) {
//         wallet->printWalletData();
//     }
//     // Validate blockchain integrity
//     if (myBlockchain.isChainValid()) {
//         cout << "Blockchain is valid.\n";
//     } else {
//         cout << "Blockchain is not valid!\n";
//     }

  
//     return 0;
// }
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

int main() {
<<<<<<< HEAD
    // Display login system and allow user to access the blockchain simulation
    Login loginSystem("users.txt"); // Initialize login system with file name
    loginSystem.displayMenu();      // Display the main menu

    // Blockchain simulation after user login
    cout << "\n--- Blockchain Simulation ---\n";
=======
    cout << "Welcome to Blockchain Simulation!" << endl;
>>>>>>> ed4ecc8005c547129dc4d53ba8f4fec24ddb1cac
    Blockchain myBlockchain;

    // File for storing the blockchain
    const string blockchainFile = "blockchain_data.txt";

    // Create wallets
    Wallet alice("Alice");
    Wallet bob("Bob");
    Wallet charlie("Charlie");

    // Initialize balances
    alice.setBalance(100);
    bob.setBalance(0);
    charlie.setBalance(0);

    // Create a vector to hold wallet pointers
    vector<Wallet*> wallets = {&alice, &bob, &charlie};

    // Option to load blockchain from file
    cout << "\n--- Load Blockchain from File ---\n";
    char choice;
    cout << "Do you want to load the blockchain from the file? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        myBlockchain.loadFromFile(blockchainFile);
    } else {
        cout << "Starting with a new blockchain.\n";
    }

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
<<<<<<< HEAD
    Transaction tx3 = charlie.sendFunds(alice, 20); // Charlie sends to Alice
    Transaction tx4 = bob.sendFunds(alice, 10);     // Bob sends to Alice
=======
    Transaction tx3 = charlie.sendFunds(alice, 20);  // Charlie sends to Alice
    Transaction tx4 = bob.sendFunds(alice, 10);      // Bob sends to Alice
>>>>>>> ed4ecc8005c547129dc4d53ba8f4fec24ddb1cac

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

<<<<<<< HEAD
=======
    // Save blockchain to file
    cout << "\n--- Save Blockchain to File ---\n";
    myBlockchain.saveToFile(blockchainFile);

>>>>>>> ed4ecc8005c547129dc4d53ba8f4fec24ddb1cac
    return 0;
}
