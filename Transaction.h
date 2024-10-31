// Transaction.h

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>

using namespace std;

class ProofOfWork; // Forward declaration

class Transaction {
public:
    string sender;
    string recipient;
    double amount;
    string signature;

    Transaction(const string &sender, const string &recipient, double amount);

    void signTransaction(ProofOfWork &pow);
    bool isValid(ProofOfWork &pow) const;
};

#endif // TRANSACTION_H
