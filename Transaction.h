


#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "RSA.h"

class Transaction {
public:
    // Constructor to initialize a Transaction
    Transaction(const std::string& sender, const std::string& receiver, double amount);

    // Method to get the transaction data as a string
    std::string getTransactionData() const;

    // Method to calculate the hash of the transaction data
    std::string calculateHash() const;

    // Method to sign the transaction with RSA
    unsigned long long signTransaction(RSA& rsa);

    // Method to verify the transaction signature with RSA
    bool verifySignature(RSA& rsa)const;

    // New method to check if the transaction is valid
    bool isValid(RSA& publicKey) ;  // To be implemented

    // Getters for sender, receiver, and amount
    std::string get_sender() const;
    std::string get_receiver() const;
    double get_amount() const;
    unsigned long long getSignature()const;



private:
    std::string sender;
    std::string receiver;
    double amount;
    unsigned long long signature; // The signature for the transaction
    std::string hash;  // The hash of the transaction data
};

#endif
