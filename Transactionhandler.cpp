#include "TransactionHandler.h"
#include <iostream>

// Create a transaction and sign it
Transaction TransactionHandler::createTransaction(const std::string& sender, const std::string& receiver, double amount, RSA& rsa) {
    // Create a transaction object
    Transaction tx(sender, receiver, amount);
    
    // Sign the transaction with the sender's private key
    tx.signTransaction(rsa);
    
    return tx;
}

// Verify a transaction's signature
bool TransactionHandler::verifyTransaction(const Transaction& tx, RSA& rsa) {
    return tx.verifySignature(rsa);  // Using the verifySignature method from the Transaction class
}

// Display the transaction details
void TransactionHandler::displayTransactionDetails(const Transaction& tx) {
    std::cout << "Transaction Details: \n";
    std::cout << "Sender: " << tx.get_sender() << "\n";
    std::cout << "Receiver: " << tx.get_receiver() << "\n";
    std::cout << "Amount: " << tx.get_amount() << "\n";
    std::cout << "Signature: " << tx.getSignature() << "\n";
}
