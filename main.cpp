// #include <iostream>
// #include "CChain.h"
// #include "Transaction.h"
// #include "Proof_Of_Work.h"

// using namespace std;
// using namespace blockchain;

// int main() {
//      Create a blockchain with a specific difficulty
//     cout<<"Hello it compiled"<<endl;
//     CChain blockchain(2);

//     // Create a transaction
//     string sender = "Alice";
//     string recipient = "Bob";
//     double amount = 10.0;

//     // Create a ProofOfWork instance to sign the transaction
//     ProofOfWork pow;

//     // Create and sign the transaction
//     blockchain.createTransaction(sender, recipient, amount);

//     // Move to the next block after adding transactions
//     blockchain.nextBlock();

//     // Display the current block's transactions
//     CBlock* currentBlock = blockchain.getCurrentBlock();
//     if (currentBlock) {
//         cout << "Current Block Transactions:" << endl;
//         currentBlock->printTransactions();
//     }

//     return 0;
// }
#include <iostream>
int main() {
    std::cout << "Program output test" << std::endl;
    return 0;
}

