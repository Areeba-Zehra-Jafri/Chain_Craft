#include <iostream>
#include <vector>
#include "CChain.h"
#include "Wallet.h"

int main() {
    Blockchain myBlockchain;

    Wallet alice("Alice");
    Wallet bob("Bob");
    Wallet charlie("Charlie");

    // Initialize balances
    alice.setBalance(100);
    bob.setBalance(100);
    charlie.setBalance(0);

    // Create a vector to hold wallet pointers
    std::vector<Wallet*> wallets = { &alice, &bob, &charlie };

    // Create transactions
    Transaction tx1 = alice.sendFunds(bob, 50);  
    Transaction tx2 = bob.sendFunds(charlie, 30); 

    // Add transactions to the blockchain
    myBlockchain.createTransaction(tx1);
    myBlockchain.createTransaction(tx2);

    // Mine pending transactions to confirm them
    myBlockchain.minePendingTransactions();

    // Update each wallet’s balance based on the blockchain’s transactions
    myBlockchain.notifyWallets(wallets);

    // Validate blockchain integrity
    if (myBlockchain.isChainValid()) {
        std::cout << "Blockchain is valid.\n";
    } else {
        std::cout << "Blockchain is not valid!\n";
    }

    // Print the entire blockchain
    myBlockchain.printChain();

    // Print wallet balances
    for (const auto& wallet : wallets) {
        wallet->printWalletData();
    }

    return 0;
}
