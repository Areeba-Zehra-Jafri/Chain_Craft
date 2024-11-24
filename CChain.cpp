#include "CChain.h"
#include "Wallet.h"
#include "sha256.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include<iomanip>
#include<queue>
#include<string>
#include <fstream>
#include<unordered_map>
#include <sstream> // For serialization
#include <stdexcept> // For exceptions

using namespace std;

// Constructor to initialize a Blockchain with the Genesis Block
Blockchain::Blockchain()
{
    std::vector<Transaction> emptyTransactions;              // Empty list for genesis block
    genesisBlock = new Block(emptyTransactions, nullptr, 2); // Genesis Block (no previous block)
    latestBlock = genesisBlock;                              // Initially, the genesis block is the latest block
}

// Create a transaction and add it to the list of pending transactions
void Blockchain::createTransaction(Transaction transaction) {
    pendingTransactions.push(std::move(transaction)); // Add to the queue
}


// Mine pending transactions into a new block and add it to the blockchain
void Blockchain::minePendingTransactions()
{
   // Check if there are pending transactions to mine
    if (pendingTransactions.empty())
    {
        std::cout << "No pending transactions to mine." << std::endl;
        return;
    }
    // Collect transactions from the queue to create a block
    std::vector<Transaction> transactionsForBlock;
    while (!pendingTransactions.empty()) {
        transactionsForBlock.push_back(pendingTransactions.front()); // Retrieve transaction
        pendingTransactions.pop();                                   // Remove it from the queue
    }

    // Create a new block with latestBlock as the previous block
    Block* newBlock = new Block(transactionsForBlock, latestBlock, 2);
    latestBlock = newBlock;            // Update the latest block to the newly mined block
    chain.push_back(*newBlock);        // Add the mined block to the chain (optional)
}

// Check if a block's hash is valid
bool Blockchain::isBlockHashValid(const Block &block)
{
    return block.blockHash == block.generateHash();
}

// Check if a transaction is valid
bool Blockchain::isTransactionValid(const Transaction &tx)
{
    return tx.get_amount() > 0;
}

bool Blockchain::isChainValid()
{
    // Iterate over the blocks in the chain starting from the second block (index 1)
    for (size_t i = 1; i < chain.size(); ++i)
    {
        Block *currentBlock = &chain[i];
        Block *previousBlock = &chain[i - 1]; // Simply use the previous block in the vector

        // Check if the current block's hash is valid
        if (!isBlockHashValid(*currentBlock))
        {
            return false;
        }

        // Ensure the previous block's hash matches
        if (currentBlock->prevhash != nullptr && currentBlock->prevhash->blockHash != previousBlock->blockHash)
        {
            return false;
        }

        cout<<"Checking signature: "<<endl;
        bool flag=true;
        // Check the validity of each transaction in the current block
        for (auto &tx : currentBlock->transactions)
        {
            RSA &publicKey = publicKeyMap[tx.get_sender()]; // Retrieve the sender's public key
            if (!tx.isValid(publicKey))

            { // Verify the transaction's signature
                //return false;
                flag=false;
                break;
            }
        }
        if(flag)
            cout<<"Siganture Transaction Valid: "<<endl;
        else
            cout<<"Signature not valid: "<<endl;

       
    }
    return true;
}

// Calculate the Merkle Root of the entire blockchain
std::string Blockchain::calculateBlockchainMerkleRoot()
{
    std::vector<std::string> blockHashes;

    Block *currentBlock = latestBlock;
    // Traverse through all blocks and collect their Merkle Root
    while (currentBlock != nullptr)
    {
        blockHashes.push_back(currentBlock->getMerkleRoot());
        currentBlock = currentBlock->getPrevBlock();
    }

    // If no blocks are present, return an empty string
    if (blockHashes.empty())
    {
        return "";
    }

    // Step 2: Build the Merkle Tree for block hashes
    while (blockHashes.size() > 1)
    {
        // If the number of elements is odd, duplicate the last element
        if (blockHashes.size() % 2 != 0)
        {
            blockHashes.push_back(blockHashes.back());
        }

        std::vector<std::string> newLevel;
        for (size_t i = 0; i < blockHashes.size(); i += 2)
        {
            std::string combinedHash = SHA256::hash(blockHashes[i] + blockHashes[i + 1]);
            newLevel.push_back(combinedHash);
        }
        blockHashes = newLevel;
    }

    // The root of the Merkle Tree is the first element of the list
    return blockHashes[0];
}

//new one
void Blockchain::printChain()
{
    Block *currentBlock = latestBlock; // Start from the genesis block
    std::cout << "Printing Blockchain...\n";
    
    while (currentBlock != nullptr)
    {
        // Convert timestamp to system time (thread-safe)
        std::time_t timeT = std::chrono::system_clock::to_time_t(currentBlock->timestamp);
        struct tm *tm_info = std::localtime(&timeT);

        // Check if localtime conversion was successful
        if (tm_info)
        {
            // Print formatted timestamp (date and time)
            std::cout << "Block Timestamp: " << std::put_time(tm_info, "%Y-%m-%d %H:%M:%S") << std::endl;
        }
        else
        {
            std::cout << "Block Timestamp: Invalid Timestamp" << std::endl;
        }

        // Print previous block's hash, or "None" if this is the genesis block
        std::cout << "Previous Hash: " << (currentBlock->prevhash ? currentBlock->prevhash->blockHash : "None") << std::endl;

        // Print current block's hash
        std::cout << "Block Hash: " << currentBlock->blockHash << std::endl;

        // Print Merkle Root of the current block
       // std::cout << "Merkle Root of block: " << currentBlock->getMerkleRoot() << std::endl;
string merkleRoot = currentBlock->getMerkleRoot();
        if (!merkleRoot.empty())
        {
            std::cout << "Merkle Root of block: " << merkleRoot << std::endl;
        }
        else
        {
            std::cout << "Merkle Root of block: No transactions in this block\n";
        }

        // Print Merkle Root of the entire blockchain
        std::cout << "Merkle Root of Blockchain: " << calculateBlockchainMerkleRoot() << std::endl;

        std::cout << "Transactions:" << std::endl;
        
        // Loop through the transactions in the current block
        for (const auto &tx : currentBlock->transactions)
        {
            std::cout << "  Sender: " << tx.get_sender() << " Receiver: " << tx.get_receiver() << " Amount: " << tx.get_amount() << std::endl;
        }

        // Print the nonce value for the block
        std::cout << "Nonce: " << currentBlock->nonce << std::endl;
        std::cout << std::endl;

        // Move to the previous block in the chain
        currentBlock = currentBlock->prevhash ? currentBlock->prevhash : nullptr;
    }
    if(isChainValid())
        cout<<"BlockChain is Valid: "<<endl;
    else 
        cout<<"BlockChain is not valid: "<<endl;
}


// Notify wallets with updated transactions and balances
void Blockchain::notifyWallets(std::vector<Wallet *> &wallets)
{
    for (auto &wallet : wallets)
    {
        // Retrieve the public key pair from the wallet
        std::pair<unsigned long long, unsigned long long> keyPair = wallet->getPublicKey();

        // Create an RSA object
        RSA rsa;

        // Set the public key in the RSA object using the setter method
        rsa.setPublicKey(keyPair);

        // Store the RSA object in the publicKeyMap
        publicKeyMap[wallet->getId()] = rsa; // Store the RSA object with the wallet ID

        // Iterate through blocks and update wallet balances
        Block *currentBlock = latestBlock; // Start from the genesis block
        wallet->updateBalance(currentBlock->transactions);
        // currentBlock = currentBlock->prevhash ? currentBlock->prevhash : nullptr;  // Move to previous block
    }
}

bool Blockchain::saveToFile(const string& filename) const {
    cout << "[DEBUG] Starting saveToFile function." << endl;

    ofstream outFile(filename, ios::binary | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "[ERROR] Unable to open file for saving blockchain." << endl;
        throw runtime_error("Unable to open file for saving blockchain.");
    }

    Block* currentBlock = latestBlock;
    while (currentBlock != nullptr) {
        cout << "[DEBUG] Saving block: " << currentBlock->blockHash << endl;

        // Save block hash
        size_t hashSize = currentBlock->blockHash.size();
        outFile.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
        outFile.write(currentBlock->blockHash.data(), hashSize);
        cout << "[DEBUG] Block hash saved (size: " << hashSize << ")." << endl;

        // Save previous hash
        bool hasPrevHash = currentBlock->prevhash != nullptr;
        outFile.write(reinterpret_cast<const char*>(&hasPrevHash), sizeof(hasPrevHash));
        if (hasPrevHash) {
            size_t prevHashSize = currentBlock->prevhash->blockHash.size();
            outFile.write(reinterpret_cast<const char*>(&prevHashSize), sizeof(prevHashSize));
            outFile.write(currentBlock->prevhash->blockHash.data(), prevHashSize);
            cout << "[DEBUG] Previous hash saved (size: " << prevHashSize << ")." << endl;
        } else {
            cout << "[DEBUG] No previous hash to save." << endl;
        }

        // Save Merkle root
        string merkleRoot = currentBlock->getMerkleRoot();
        size_t merkleSize = merkleRoot.size();
        outFile.write(reinterpret_cast<const char*>(&merkleSize), sizeof(merkleSize));
        outFile.write(merkleRoot.data(), merkleSize);
        cout << "[DEBUG] Merkle root saved (size: " << merkleSize << ")." << endl;

        // Save nonce
        outFile.write(reinterpret_cast<const char*>(&currentBlock->nonce), sizeof(currentBlock->nonce));
        cout << "[DEBUG] Nonce saved: " << currentBlock->nonce << endl;

        // Save timestamp
        time_t timestamp = chrono::system_clock::to_time_t(currentBlock->timestamp);
        outFile.write(reinterpret_cast<const char*>(&timestamp), sizeof(timestamp));
        cout << "[DEBUG] Timestamp saved: " << timestamp << endl;

        // Save transactions
        size_t txCount = currentBlock->transactions.size();
        outFile.write(reinterpret_cast<const char*>(&txCount), sizeof(txCount));
        cout << "[DEBUG] Number of transactions: " << txCount << endl;

        for (const auto& tx : currentBlock->transactions) {
            // Save sender
            size_t senderSize = tx.get_sender().size();
            outFile.write(reinterpret_cast<const char*>(&senderSize), sizeof(senderSize));
            outFile.write(tx.get_sender().data(), senderSize);
            cout << "[DEBUG] Sender saved: " << tx.get_sender() << endl;

            // Save receiver
            size_t receiverSize = tx.get_receiver().size();
            outFile.write(reinterpret_cast<const char*>(&receiverSize), sizeof(receiverSize));
            outFile.write(tx.get_receiver().data(), receiverSize);
            cout << "[DEBUG] Receiver saved: " << tx.get_receiver() << endl;

            // Save amount
            double amount = tx.get_amount();
            outFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
            cout << "[DEBUG] Amount saved: " << amount << endl;
        }

        currentBlock = currentBlock->prevhash; // Move to the previous block
    }

    outFile.close();
    cout << "[DEBUG] saveToFile completed successfully." << endl;
    return true;
}
// bool Blockchain::loadFromFile(const string& filename) {
//     cout << "[DEBUG] Starting loadFromFile function." << endl;

//     ifstream inFile(filename, ios::binary);
//     if (!inFile.is_open()) {
//         cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
//         throw runtime_error("Unable to open file for loading blockchain.");
//     }

//     vector<Block*> loadedBlocks;

//     while (true) {
//         cout << "[DEBUG] Reading block data." << endl;

//         // Read block hash
//         size_t hashSize;
//         if (!inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize))) break;
//         string blockHash(hashSize, '\0');
//         inFile.read(&blockHash[0], hashSize);
//         cout << "[DEBUG] Block hash read: " << blockHash << " (size: " << hashSize << ")" << endl;

//         // Read previous hash
//         bool hasPrevHash;
//         inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
//         string prevHash;
//         if (hasPrevHash) {
//             size_t prevHashSize;
//             inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
//             prevHash.resize(prevHashSize);
//             inFile.read(&prevHash[0], prevHashSize);
//             cout << "[DEBUG] Previous hash read: " << prevHash << " (size: " << prevHashSize << ")" << endl;
//         } else {
//             cout << "[DEBUG] No previous hash found." << endl;
//         }

//         // Read Merkle root
//         size_t merkleSize;
//         inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
//         string merkleRoot(merkleSize, '\0');
//         inFile.read(&merkleRoot[0], merkleSize);
//         cout << "[DEBUG] Merkle root read: " << merkleRoot << " (size: " << merkleSize << ")" << endl;

//         // Read nonce
//         int nonce;
//         inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));
//         cout << "[DEBUG] Nonce read: " << nonce << endl;

//         // Read timestamp
//         time_t timestamp;
//         inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
//         cout << "[DEBUG] Timestamp read: " << timestamp << endl;

//         // Read transactions
//         size_t txCount;
//         inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
//         cout << "[DEBUG] Number of transactions read: " << txCount << endl;

//         vector<Transaction> transactions;
//         for (size_t i = 0; i < txCount; ++i) {
//             cout << "[DEBUG] Reading transaction " << i + 1 << " of " << txCount << "." << endl;

//             // Read sender
//             size_t senderSize;
//             inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
//             string sender(senderSize, '\0');
//             inFile.read(&sender[0], senderSize);
//             cout << "[DEBUG] Sender read: " << sender << endl;

//             // Read receiver
//             size_t receiverSize;
//             inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
//             string receiver(receiverSize, '\0');
//             inFile.read(&receiver[0], receiverSize);
//             cout << "[DEBUG] Receiver read: " << receiver << endl;

//             // Read amount
//             double amount;
//             inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));
//             cout << "[DEBUG] Amount read: " << amount << endl;

//             transactions.emplace_back(sender, receiver, amount);
//         }

//         // Reconstruct block
//         Block* prevBlock = !loadedBlocks.empty() ? loadedBlocks.back() : nullptr;
//         Block* newBlock = new Block(transactions, prevBlock, nonce, blockHash, chrono::system_clock::from_time_t(timestamp));
//         loadedBlocks.push_back(newBlock);
//         cout << "[DEBUG] Block reconstructed and added to list." << endl;
//     }

//     inFile.close();

//     // Rebuild the blockchain
//     // if (!loadedBlocks.empty()) {
//     //     cout << "[DEBUG] Rebuilding blockchain from loaded blocks." << endl;
//     //     genesisBlock = loadedBlocks.back();
//     //     latestBlock = loadedBlocks.front();
//     //     chain.clear();
//     //     for (auto block : loadedBlocks) {
//     //         chain.push_back(*block);
//     //     }
//     // }
//     if (!loadedBlocks.empty()) {
//     cout << "[DEBUG] Rebuilding blockchain from loaded blocks." << endl;

//     // Set genesisBlock and latestBlock
//     genesisBlock = loadedBlocks.back();  // Last block read is genesis
//     latestBlock = loadedBlocks.front(); // First block read is the latest

//     chain.clear();

//     // Add all blocks to the chain and relink prevhash pointers
//     for (size_t i = 0; i < loadedBlocks.size(); ++i) {
//         if (i > 0) {
//             loadedBlocks[i]->prevhash = loadedBlocks[i - 1]; // Link to previous block
//         }
//         chain.push_back(*loadedBlocks[i]); // Add block to the chain
//     }

//     cout << "[DEBUG] Blockchain successfully rebuilt with " << chain.size() << " blocks." << endl;
//     }

//     cout << "[DEBUG] loadFromFile completed successfully." << endl;
//     return true;
// }
bool Blockchain::loadFromFile(const string& filename) {
    cout << "[DEBUG] Starting loadFromFile function." << endl;

    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cerr << "[ERROR] Unable to open file for loading blockchain." << endl;
        throw runtime_error("Unable to open file for loading blockchain.");
    }

    unordered_map<string, Block*> blockMap; // Map to store blocks by their hash
    vector<Block*> loadedBlocks;

    while (true) {
        cout << "[DEBUG] Reading block data." << endl;

        // Read block hash
        size_t hashSize;
        if (!inFile.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize))) break;
        string blockHash(hashSize, '\0');
        inFile.read(&blockHash[0], hashSize);
        cout << "[DEBUG] Block hash read: " << blockHash << " (size: " << hashSize << ")" << endl;

        // Read previous hash
        bool hasPrevHash;
        inFile.read(reinterpret_cast<char*>(&hasPrevHash), sizeof(hasPrevHash));
        string prevHash;
        if (hasPrevHash) {
            size_t prevHashSize;
            inFile.read(reinterpret_cast<char*>(&prevHashSize), sizeof(prevHashSize));
            prevHash.resize(prevHashSize);
            inFile.read(&prevHash[0], prevHashSize);
            cout << "[DEBUG] Previous hash read: " << prevHash << " (size: " << prevHashSize << ")" << endl;
        } else {
            cout << "[DEBUG] No previous hash found." << endl;
        }

        // Read Merkle root
        size_t merkleSize;
        inFile.read(reinterpret_cast<char*>(&merkleSize), sizeof(merkleSize));
        string merkleRoot(merkleSize, '\0');
        inFile.read(&merkleRoot[0], merkleSize);
        cout << "[DEBUG] Merkle root read: " << merkleRoot << " (size: " << merkleSize << ")" << endl;

        // Read nonce
        int nonce;
        inFile.read(reinterpret_cast<char*>(&nonce), sizeof(nonce));
        cout << "[DEBUG] Nonce read: " << nonce << endl;

        // Read timestamp
        time_t timestamp;
        inFile.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
        cout << "[DEBUG] Timestamp read: " << timestamp << endl;

        // Read transactions
        size_t txCount;
        inFile.read(reinterpret_cast<char*>(&txCount), sizeof(txCount));
        cout << "[DEBUG] Number of transactions read: " << txCount << endl;

        vector<Transaction> transactions;
        for (size_t i = 0; i < txCount; ++i) {
            cout << "[DEBUG] Reading transaction " << i + 1 << " of " << txCount << "." << endl;

            // Read sender
            size_t senderSize;
            inFile.read(reinterpret_cast<char*>(&senderSize), sizeof(senderSize));
            string sender(senderSize, '\0');
            inFile.read(&sender[0], senderSize);
            cout << "[DEBUG] Sender read: " << sender << endl;

            // Read receiver
            size_t receiverSize;
            inFile.read(reinterpret_cast<char*>(&receiverSize), sizeof(receiverSize));
            string receiver(receiverSize, '\0');
            inFile.read(&receiver[0], receiverSize);
            cout << "[DEBUG] Receiver read: " << receiver << endl;

            // Read amount
            double amount;
            inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));
            cout << "[DEBUG] Amount read: " << amount << endl;

            transactions.emplace_back(sender, receiver, amount);
        }

        // Create new block (temporary prevhash link is null)
        Block* newBlock = new Block(transactions, nullptr, nonce, blockHash, chrono::system_clock::from_time_t(timestamp));
        loadedBlocks.push_back(newBlock);
        blockMap[blockHash] = newBlock; // Store in map for later linking
        cout << "[DEBUG] Block reconstructed and added to map." << endl;

        // Link prevhash if available
        if (hasPrevHash && blockMap.find(prevHash) != blockMap.end()) {
            newBlock->prevhash = blockMap[prevHash];
        }
    }

    inFile.close();

    // Rebuild the blockchain
    if (!loadedBlocks.empty()) {
        cout << "[DEBUG] Rebuilding blockchain from loaded blocks." << endl;
        genesisBlock = loadedBlocks.back();
        latestBlock = loadedBlocks.front();
        chain.clear();

        for (auto block : loadedBlocks) {
            chain.push_back(*block);
        }
    }

    cout << "[DEBUG] loadFromFile completed successfully." << endl;
    return true;
}
