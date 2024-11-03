#include "Transaction.h"
#include "Proof_Of_Work.h" // Include your ProofOfWork class
#include <sstream> // Include stringstream for data creation

using namespace std;

Transaction::Transaction(const string &sender, const string &recipient, double amount)
    : sender(sender), recipient(recipient), amount(amount) {}

void Transaction::signTransaction(ProofOfWork &pow) {
    // Create data string from transaction details
    ostringstream dataStream;
    dataStream << sender << recipient << amount;
    string data = dataStream.str();

    // Sign the transaction data
    signature = pow.signData(data); // No error here as pow is non-const now
}

bool Transaction::isValid(ProofOfWork &pow) const {
    // Create data string from transaction details
    ostringstream dataStream;
    dataStream << sender << recipient << amount;
    string data = dataStream.str();

    // Verify the transaction signature
    return pow.verifyData(data, signature);
}
