// #ifndef TRANSACTION_H
// #define TRANSACTION_H

// #include <string>
// #include "RSA.h"

// class Transaction {
// public:
//     // Constructor
//     Transaction(const std::string& sender, const std::string& receiver, double amount);

//     // Get transaction data as a string
//     std::string getTransactionData() const;

//     // Calculate the SHA-256 hash of the transaction data
//     std::string calculateHash() const;

//     // Sign the transaction using RSA
//     unsigned long long signTransaction(RSA& rsa);

//     // Verify the transaction signature using RSA
//     bool verifySignature( RSA& rsa, unsigned long long signature);

//     // Getters for sender, receiver, and amount
//     std::string get_sender() const;
//     std::string get_receiver() const;
//     double get_amount() const;

//      bool isValid(const RSA& publicKey) const;

// private:
//     std::string sender;
//     std::string receiver;
//     double amount;
//     std::string hash;
// };

// #endif // TRANSACTION_H



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
    bool verifySignature(RSA& rsa, unsigned long long signature);

    // New method to check if the transaction is valid
    bool isValid(RSA& publicKey) ;  // To be implemented

    // Getters for sender, receiver, and amount
    std::string get_sender() const;
    std::string get_receiver() const;
    double get_amount() const;

private:
    std::string sender;
    std::string receiver;
    double amount;
    unsigned long long signature; // The signature for the transaction
    std::string hash;  // The hash of the transaction data
};

#endif
