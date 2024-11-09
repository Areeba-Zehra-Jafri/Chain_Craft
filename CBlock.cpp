// #include "CBlock.h"
// #include "SHA256.h" // Include custom SHA256 implementation
// #include <sstream>
// #include <iomanip>
// #include <chrono>

// using namespace std;

// static long long lastTimestamp = 0;
// static int blockCounter = 0;


// // Constructor to initialize a Block with given transactions, previous hash, and difficulty level.
// Block::Block(std::vector<Transaction> transactions, Block *previoushash, int difficulty)
// {
//     this->transactions = std::move(transactions);
//     this->prevhash = previoushash; // Store the previous block pointer
//     // this->timestamp = std::time(nullptr);
//     //    this->timestamp = std::chrono::system_clock::now(); // Use high-resolution timestamp
//     // this is working.. this->timestamp = std::chrono::system_clock::now(); // Store high-resolution timestamp
//     auto currentTime = std::chrono::system_clock::now();

//     // Adjust the timestamp to avoid duplicates if multiple blocks are mined in the same millisecond
//     auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();

//     if (currentTimeMs == lastTimestamp)
//     {
//          currentTimeMs += (blockCounter % 10);
//         // If the timestamp is the same as the last block, increment by 1ms
//         //currentTimeMs++;
//     }

//     lastTimestamp = currentTimeMs; // Update the global timestamp tracker

//     // Convert time to std::chrono::milliseconds and assign to timestamp
//     timestamp = std::chrono::system_clock::from_time_t(currentTimeMs / 1000);
//     blockCounter++;

//     this->difficulty = difficulty;
//     this->nonce = 0;
//     this->blockHash = mineBlock(); // Compute the block's hash
// }

// // Mining function to find a valid hash for the block based on the given difficulty
// std::string Block::mineBlock()
// {
//     std::string target(difficulty, '0');
//     while (blockHash.substr(0, difficulty) != target)
//     {
//         nonce++;
//         blockHash = generateHash();
//     }
//     return blockHash;
// }

// // Generate the hash of the block
// // std::string Block::generateHash() const {
// //     std::stringstream ss;
// //     ss << std::put_time(std::gmtime(&timestamp), "%Y-%m-%dT%H:%M:%S");  // Time in ISO format

// //     // Append all transaction details to the hash input
// //     for (const auto& tx : transactions) {
// //         ss << tx.get_sender() << tx.get_receiver() << tx.get_amount();
// //     }

// //     // Append the previous block's hash (use prevHash->blockHash for the previous block's hash)
// //     if (prevhash != nullptr) {
// //         ss << prevhash->blockHash;  // Correctly access the previous block's hash
// //     }

// //     // Append the nonce value (which will be mined)
// //     ss << nonce;

// //     // Return the final hash (SHA-256) of the string
// //     return sha256(ss.str());
// // }

// // Generate the hash of the block
// std::string Block::generateHash() const
// {
//     std::stringstream ss;

//     // Convert `std::chrono::system_clock::time_point` to `std::time_t`
//     auto timeT = std::chrono::system_clock::to_time_t(timestamp);

//     // Convert time to a readable format
//     ss << std::put_time(std::gmtime(&timeT), "%Y-%m-%dT%H:%M:%S");

//     // Get the milliseconds part
//     auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()) % 1000;
//     ss << '.' << std::setw(3) << std::setfill('0') << ms.count();

//     // Append all transaction details to the hash input
//     for (const auto &tx : transactions)
//     {
//         ss << tx.get_sender() << tx.get_receiver() << tx.get_amount();
//     }

//     // Append the previous block's hash
//     if (prevhash != nullptr)
//     {
//         ss << prevhash->blockHash;
//     }

//     // Append the nonce value (which will be mined)
//     ss << nonce;

//     // Return the final SHA-256 hash of the constructed string
//     return sha256(ss.str());
// }

// // Compute SHA-256 hash for a given string using the custom SHA256 class
// std::string Block::sha256(const std::string &str) const
// {
//     SHA256 sha256;
//     return sha256.hash(str); // Assuming SHA256 has a hash() method that returns a std::string
// }

// // Define the setter and getter methods
// Block *Block::getPrevBlock() const
// {
//     return prevhash;
// }

// void Block::setPrevBlock(Block *prev)
// {
//     prevhash = prev;
// }

// // Calculate the Merkle Root of all transactions in the block
// std::string Block::calculateMerkleRoot()
// {
//     if (transactions.empty())
//     {
//         return ""; // Return empty string if no transactions
//     }

//     std::vector<std::string> merkleTree;

//     // Step 1: Add all transaction hashes to the list
//     for (const auto &tx : transactions)
//     {
//         std::string txData = tx.get_sender() + tx.get_receiver() + std::to_string(tx.get_amount());
//         merkleTree.push_back(sha256(txData)); // Hash each transaction
//     }

//     // Step 2: Build the Merkle Tree
//     while (merkleTree.size() > 1)
//     {
//         // If the number of elements is odd, duplicate the last element
//         if (merkleTree.size() % 2 != 0)
//         {
//             merkleTree.push_back(merkleTree.back());
//         }

//         std::vector<std::string> newLevel;
//         // Combine each pair of adjacent hashes
//         for (size_t i = 0; i < merkleTree.size(); i += 2)
//         {
//             std::string combinedHash = sha256(merkleTree[i] + merkleTree[i + 1]);
//             newLevel.push_back(combinedHash);
//         }
//         merkleTree = newLevel; // Move to the next level of the tree
//     }

//     // Step 3: The root of the Merkle Tree is the first element of the list
//     return merkleTree.empty() ? "" : merkleTree[0];
// }

// // Method to get the Merkle Root (compute if not already computed)
// std::string Block::getMerkleRoot()
// {
//     if (merkleRoot.empty())
//     {
//         merkleRoot = calculateMerkleRoot();
//     }
//     return merkleRoot;
// }


//newww one
#include "CBlock.h"
#include "SHA256.h" // Include custom SHA256 implementation
#include <sstream>
#include <iomanip>
#include <chrono>
#include <iostream>

using namespace std;

static long long lastTimestamp = 0;
static int blockCounter = 0;

// Constructor to initialize a Block with given transactions, previous hash, and difficulty level.
Block::Block(std::vector<Transaction> transactions, Block *previoushash, int difficulty)
{
    this->transactions = std::move(transactions);
    this->prevhash = previoushash; // Store the previous block pointer

    auto currentTime = std::chrono::system_clock::now();

    // Adjust the timestamp to avoid duplicates if multiple blocks are mined in the same time range
    auto currentTimeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime.time_since_epoch()).count();

    if (currentTimeNs == lastTimestamp)
    {
        currentTimeNs += (blockCounter % 100); // Use modulo for a greater increment to ensure uniqueness
    }

    lastTimestamp = currentTimeNs; // Update the global timestamp tracker

    // Convert time to std::chrono::milliseconds and assign to timestamp
    timestamp = std::chrono::system_clock::from_time_t(currentTimeNs / 1000000000); // Convert back to seconds

    blockCounter++;

    this->difficulty = difficulty;
    this->nonce = 0;
    this->blockHash = mineBlock(); // Compute the block's hash
}

// Mining function to find a valid hash for the block based on the given difficulty
std::string Block::mineBlock()
{
    std::string target(difficulty, '0');
    while (blockHash.substr(0, difficulty) != target)
    {
        nonce++;
        blockHash = generateHash();
    }
    return blockHash;
}

// Generate the hash of the block
std::string Block::generateHash() const
{
    std::stringstream ss;

    // Convert `std::chrono::system_clock::time_point` to `std::time_t`
    auto timeT = std::chrono::system_clock::to_time_t(timestamp);

    // Convert time to a readable format
    ss << std::put_time(std::gmtime(&timeT), "%Y-%m-%dT%H:%M:%S");

    // Get the nanoseconds part
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()) % 1000000000;
    ss << '.' << std::setw(9) << std::setfill('0') << ns.count();

    // Append all transaction details to the hash input
    for (const auto &tx : transactions)
    {
        ss << tx.get_sender() << tx.get_receiver() << tx.get_amount();
    }

    // Append the previous block's hash
    if (prevhash != nullptr)
    {
        ss << prevhash->blockHash;
    }

    // Append the nonce value (which will be mined)
    ss << nonce;

    // Return the final SHA-256 hash of the constructed string
    return sha256(ss.str());
}

// Compute SHA-256 hash for a given string using the custom SHA256 class
std::string Block::sha256(const std::string &str) const
{
    SHA256 sha256;
    return sha256.hash(str); // Assuming SHA256 has a hash() method that returns a std::string
}

// Define the setter and getter methods
Block *Block::getPrevBlock() const
{
    return prevhash;
}

void Block::setPrevBlock(Block *prev)
{
    prevhash = prev;
}

// Calculate the Merkle Root of all transactions in the block
std::string Block::calculateMerkleRoot()
{
    if (transactions.empty())
    {
        return ""; // Return empty string if no transactions
    }

    std::vector<std::string> merkleTree;

    // Step 1: Add all transaction hashes to the list
    for (const auto &tx : transactions)
    {
        std::string txData = tx.get_sender() + tx.get_receiver() + std::to_string(tx.get_amount());
        merkleTree.push_back(sha256(txData)); // Hash each transaction
    }

    // Step 2: Build the Merkle Tree
    while (merkleTree.size() > 1)
    {
        // If the number of elements is odd, duplicate the last element
        if (merkleTree.size() % 2 != 0)
        {
            merkleTree.push_back(merkleTree.back());
        }

        std::vector<std::string> newLevel;
        // Combine each pair of adjacent hashes
        for (size_t i = 0; i < merkleTree.size(); i += 2)
        {
            std::string combinedHash = sha256(merkleTree[i] + merkleTree[i + 1]);
            newLevel.push_back(combinedHash);
        }
        merkleTree = newLevel; // Move to the next level of the tree
    }

    // Step 3: The root of the Merkle Tree is the first element of the list
    return merkleTree.empty() ? "" : merkleTree[0];
}

// Method to get the Merkle Root (compute if not already computed)
std::string Block::getMerkleRoot()
{
    if (merkleRoot.empty())
    {
        merkleRoot = calculateMerkleRoot();
    }
    return merkleRoot;
}
