// #include "Transaction.h"
// #include "SHA256.h"
// #include "RSA.h"
// #include <sstream>
// #include <iomanip>
// #include <functional>

// // Constructor to initialize a Transaction
// Transaction::Transaction(const std::string& sender, const std::string& receiver, double amount)
//     : sender(sender), receiver(receiver), amount(amount) {
//     hash = calculateHash();
// }

// // Method to get the transaction data as a string
// std::string Transaction::getTransactionData() const {
//     std::ostringstream ss;
//     ss << sender << receiver << std::fixed << std::setprecision(2) << amount;
//     return ss.str();
// }

// // Calculate the SHA-256 hash of the transaction data
// std::string Transaction::calculateHash() const {
//     return SHA256::hash(getTransactionData());
// }

// // Sign the transaction using the RSA class
// unsigned long long Transaction::signTransaction(RSA& rsa) {
//     // Hash the transaction data to a numeric value for signing
//     std::hash<std::string> str_hash;
//     unsigned long long dataNumeric = str_hash(getTransactionData());
//     return rsa.encrypt(dataNumeric);  // Return the encrypted numeric data as the signature
// }

// // Verify the transaction signature using RSA
// bool Transaction::verifySignature( RSA& rsa, unsigned long long signature) {
//     unsigned long long decryptedData = rsa.decrypt(signature);
//     unsigned long long expectedDataNumeric = std::hash<std::string>{}(getTransactionData());
//     return decryptedData == expectedDataNumeric;
// }


// // Getters for sender, receiver, and amount
// std::string Transaction::get_sender() const {
//     return sender;
// }

// std::string Transaction::get_receiver() const {
//     return receiver;
// }

// double Transaction::get_amount() const {
//     return amount;
// }


#include "Transaction.h"
#include "SHA256.h"
#include "RSA.h"
#include <sstream>
#include <iomanip>
#include <functional>

// Constructor to initialize a Transaction
Transaction::Transaction(const std::string& sender, const std::string& receiver, double amount)
    : sender(sender), receiver(receiver), amount(amount) {
    hash = calculateHash();
}

// Method to get the transaction data as a string
std::string Transaction::getTransactionData() const {
    std::ostringstream ss;
    ss << sender << receiver << std::fixed << std::setprecision(2) << amount;
    return ss.str();
}

// Calculate the SHA-256 hash of the transaction data
std::string Transaction::calculateHash() const {
    return SHA256::hash(getTransactionData());
}

// Sign the transaction using the RSA class
unsigned long long Transaction::signTransaction(RSA& rsa) {
    // Hash the transaction data to a numeric value for signing
    std::hash<std::string> str_hash;
    unsigned long long dataNumeric = str_hash(getTransactionData());
    signature = rsa.encrypt(dataNumeric);  // Store the encrypted signature
    return signature;
}

// Verify the transaction signature using RSA
bool Transaction::verifySignature(RSA& rsa, unsigned long long signature) {
    unsigned long long decryptedData = rsa.decrypt(signature);
    unsigned long long expectedDataNumeric = std::hash<std::string>{}(getTransactionData());
    return decryptedData == expectedDataNumeric;
}

// New method to check if the transaction is valid
bool Transaction::isValid( RSA& publicKey) {
    // Verify the signature using the public key
    return verifySignature(publicKey, signature);
}

// Getters for sender, receiver, and amount
std::string Transaction::get_sender() const {
    return sender;
}

std::string Transaction::get_receiver() const {
    return receiver;
}

double Transaction::get_amount() const {
    return amount;
}
