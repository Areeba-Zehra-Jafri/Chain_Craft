#include "CChain.h"
#include "Wallet.h"
#include <iostream>

// Constructor to initialize a Blockchain with the Genesis Block
Blockchain::Blockchain() {
    std::vector<Transaction> emptyTransactions;  // Empty list for genesis block
    chain.emplace_back(emptyTransactions, "0", 2);  // Genesis Block
}

// Create a transaction and add it to the list of pending transactions
void Blockchain::createTransaction(Transaction transaction) {
    pendingTransactions.push_back(std::move(transaction));
}

// Mine pending transactions into a new block and add it to the blockchain
void Blockchain::minePendingTransactions() {
    Block newBlock(pendingTransactions, chain.back().blockHash, 2);
    chain.push_back(newBlock);
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
    for (size_t i = 1; i < chain.size(); ++i) {
        Block currBlock = chain[i];
        Block prevBlock = chain[i - 1];

        // Check if the current block's hash is valid
        if (!isBlockHashValid(currBlock)) {
            return false;
        }

        // Check if the previous block's hash matches
        if (currBlock.prevHash != prevBlock.blockHash) {
            return false;
        }

        // Check the validity of each transaction in the current block
        for ( auto& tx : currBlock.transactions) {
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
    for (const auto& block : chain) {
        std::cout << "Block Timestamp: " << block.timestamp << std::endl;
        std::cout << "Previous Hash: " << block.prevHash << std::endl;
        std::cout << "Block Hash: " << block.blockHash << std::endl;
        std::cout << "Transactions:" << std::endl;

        for (const auto& tx : block.transactions) {
            std::cout << "  Sender: " << tx.get_sender() << " Receiver: " << tx.get_receiver() << " Amount: " << tx.get_amount() << std::endl;
        }

        std::cout << "Nonce: " << block.nonce << std::endl;
        std::cout << std::endl;
    }
}

// Notify wallets with updated transactions and balances
// void Blockchain::notifyWallets(std::vector<Wallet*>& wallets) {
//     for (auto& wallet : wallets) {
//         publicKeyMap[wallet->getId()] = wallet->getPublicKey();  // Store the public key in the map
//         for (auto& block : chain) {
//             wallet->updateBalance(block.transactions);
//         }
//     }
// }
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
        for (auto& block : chain) {
            wallet->updateBalance(block.transactions);
        }
    }
}
