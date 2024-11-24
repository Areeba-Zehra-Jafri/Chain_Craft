#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "Transaction.h"
#include "RSA.h"

class Wallet {
private:
    std::string id;
    float balance;
    std::pair<unsigned long long, unsigned long long> publicKey;  // Public key as a pair (e, n)
    std::pair<unsigned long long, unsigned long long> privateKey; // Private key as a pair (d, n)

public:
    // Constructor

    RSA rsa;
    const std::pair<unsigned long long, unsigned long long>& getPublicKey() const {
        return publicKey;
    }

    void setBalance(float newBalance);
    
    Wallet(const std::string& id);

    // Destructor
    ~Wallet();

    // Method to generate the RSA key pair
    void generateKeys();

    // Method to send funds to another wallet, signing the transaction
    Transaction sendFunds(Wallet& receiver, float amount);

    // Method to update balance based on transactions
    void updateBalance(const std::vector<Transaction>& transactions);

    // Method to print wallet details
    void printWalletData() const;

    // Getter for wallet ID
    std::string getId() const;

    // Getter for wallet balance
    float getBalance() const;

    std::pair<unsigned long long, unsigned long long> convertSignatureToPair(const std::string& signature);
};

#endif // WALLET_H
