#ifndef CCHAIN_H
#define CCHAIN_H

#include "CBlock.h"
#include "Transaction.h"
#include "wallet.h"
#include <vector>
#include <map>

class Blockchain {
private:
    Block* genesisBlock;           // Pointer to the genesis block
    Block* latestBlock;            // Pointer to the latest block
    std::vector<Block> chain;      // Vector to store the entire blockchain
    std::vector<Transaction> pendingTransactions; // List of pending transactions
    std::map<std::string, RSA> publicKeyMap;  // Map to store public keys by wallet ID

public:
    Blockchain();  // Constructor

    // Create a transaction and add it to the list of pending transactions
    void createTransaction(Transaction transaction);

    // Mine pending transactions into a new block and add it to the blockchain
    void minePendingTransactions();

    // Validate the integrity of the blockchain
    bool isChainValid();

    // Display the details of the entire blockchain
    void printChain();

    // Check if a block's hash is valid
    bool isBlockHashValid(const Block& block);

    // Check if a transaction is valid
    bool isTransactionValid(const Transaction& tx);

    // Notify wallets with updated transactions and balances
    void notifyWallets(std::vector<Wallet*>& wallets);

  
    std::string calculateBlockchainMerkleRoot(); // Method to calculate Merkle Root of the entire blockchain



    Block* get_genesis()
    {
        return genesisBlock;
    }

    Block* getLatestBlock()
    {
        return latestBlock;
    } 

};

#endif  // CCHAIN_H
