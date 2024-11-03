#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class ProofOfWork; // Forward declaration

class Transaction {
private:
    std::string sender;     // Transaction sender
    std::string recipient;  // Transaction recipient
    double amount;          // Transaction amount
    std::string signature;  // Signature for the transaction

public:
    // Constructor to initialize transaction details
    Transaction(const std::string &sender, const std::string &recipient, double amount);

    // Sign the transaction using the ProofOfWork instance
    void signTransaction( ProofOfWork &pow);

    // Verify the validity of the transaction
    bool isValid( ProofOfWork &pow) const;

    // Getters for member variables (optional)
    const std::string& getSender() const { return sender; }
    const std::string& getRecipient() const { return recipient; }
    double getAmount() const { return amount; }
    const std::string& getSignature() const { return signature; }
};

#endif // TRANSACTION_H
