// Transaction.cpp


#include "Transaction.h"
#include "Proof_Of_Work.h" // Include your ProofOfWork class


using namespace std;

Transaction::Transaction(const string &sender, const string &recipient, double amount)
    : sender(sender), recipient(recipient), amount(amount) {}

void Transaction::signTransaction(ProofOfWork &pow) {
    string data = sender + recipient + to_string(amount);
    signature = pow.signData(data);
}

bool Transaction::isValid(ProofOfWork &pow) const {
    string data = sender + recipient + to_string(amount);
    return pow.verifyData(data, signature);
}
