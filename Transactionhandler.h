#ifndef TRANSACTION_HANDLER_H
#define TRANSACTION_HANDLER_H

#include <string>
#include <vector>
#include "RSA.h"
#include "Transaction.h"

class TransactionHandler {
public:
    // Method to handle the sending of funds
    static Transaction createTransaction(const std::string& sender, const std::string& receiver, double amount, RSA& rsa);

    // Method to verify a transaction signature
    static bool verifyTransaction(const Transaction& tx, RSA& rsa);

    // Method to display transaction details
    static void displayTransactionDetails(const Transaction& tx);
};

#endif
