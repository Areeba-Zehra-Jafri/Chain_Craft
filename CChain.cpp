#include "CChain.h"
#include "Wallet.h"
#include <iostream>

// Constructor to initialize a Blockchain with the Genesis Block
Blockchain::Blockchain() {
    std::vector<Transaction> emptyTransactions;  // Empty list for genesis block
    genesisBlock = new Block(emptyTransactions, nullptr, 2);  // Genesis Block (no previous block)
    latestBlock = genesisBlock;  // Initially, the genesis block is the latest block
}

// Create a transaction and add it to the list of pending transactions
void Blockchain::createTransaction(Transaction transaction) {
    pendingTransactions.push_back(std::move(transaction));
}

// Mine pending transactions into a new block and add it to the blockchain
void Blockchain::minePendingTransactions() {
        // Check if there are pending transactions to mine
    if (pendingTransactions.empty()) {
        std::cout << "No pending transactions to mine." << std::endl;
        return;
    }

    Block* newBlock = new Block(pendingTransactions, latestBlock, 2);  // Create a new block with latestBlock as the previous block
    latestBlock = newBlock;  // Update the latest block to the newly mined block
    chain.push_back(*newBlock);  // Add the mined block to the chain (optional, if you want to keep both the linked list and vector)
    pendingTransactions.clear();  // Clear pending transactions
}

// Check if a block's hash is valid
bool Blockchain::isBlockHashValid(const Block& block) {
    return block.blockHash == block.generateHash();
}

// Check if a transaction is valid
bool Blockchain::isTransactionValid(const Transaction& tx) {
    return tx.get_amount() > 0;
}

bool Blockchain::isChainValid() {
    // Iterate over the blocks in the chain starting from the second block (index 1)
    for (size_t i = 1; i < chain.size(); ++i) {
        Block* currentBlock = &chain[i];
        Block* previousBlock = &chain[i - 1];  // Simply use the previous block in the vector

        // Check if the current block's hash is valid
        if (!isBlockHashValid(*currentBlock)) {
            return false;
        }

        // Ensure the previous block's hash matches
        if (currentBlock->prevhash != nullptr && currentBlock->prevhash->blockHash != previousBlock->blockHash) {
            return false;
        }

        // Check the validity of each transaction in the current block
        for (auto& tx : currentBlock->transactions) {
            RSA& publicKey = publicKeyMap[tx.get_sender()];  // Retrieve the sender's public key
            if (!tx.isValid(publicKey)) {  // Verify the transaction's signature
                return false;
            }
        }
    }
    return true;
}



// Display the details of the entire blockchain
void Blockchain::printChain() {
    Block* currentBlock = latestBlock;  // Start from the genesis block
    std::cout << "Printing Blockchain...\n";
    while (currentBlock != nullptr) {
        std::cout << "Block Timestamp: " << currentBlock->timestamp << std::endl;
        std::cout << "Previous Hash: " << (currentBlock->prevhash ? currentBlock->prevhash->blockHash : "None") << std::endl;
        std::cout << "Block Hash: " << currentBlock->blockHash << std::endl;
        std::cout << "Transactions:" << std::endl;

        for (const auto& tx : currentBlock->transactions) {
            std::cout << "  Sender: " << tx.get_sender() << " Receiver: " << tx.get_receiver() << " Amount: " << tx.get_amount() << std::endl;
        }

        std::cout << "Nonce: " << currentBlock->nonce << std::endl;
        std::cout << std::endl;

        // Move to the previous block in the chain
        currentBlock = currentBlock->prevhash ? currentBlock->prevhash : nullptr;
    }
}

// Notify wallets with updated transactions and balances
void Blockchain::notifyWallets(std::vector<Wallet*>& wallets) {
    for (auto& wallet : wallets) {
        // Retrieve the public key pair from the wallet
        std::pair<unsigned long long, unsigned long long> keyPair = wallet->getPublicKey();
        
        // Create an RSA object
        RSA rsa;
        
        // Set the public key in the RSA object using the setter method
        rsa.setPublicKey(keyPair);

        // Store the RSA object in the publicKeyMap
        publicKeyMap[wallet->getId()] = rsa;  // Store the RSA object with the wallet ID

        // Iterate through blocks and update wallet balances
        Block* currentBlock = latestBlock;  // Start from the genesis block
        while (currentBlock != nullptr) {
            wallet->updateBalance(currentBlock->transactions);
            currentBlock = currentBlock->prevhash ? currentBlock->prevhash : nullptr;  // Move to previous block
        }
    }
}
