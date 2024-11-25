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

Block::Block(const std::vector<Transaction>& transactions, Block* prevBlock, int nonce, 
          const std::string& blockHash, const std::chrono::system_clock::time_point& timestamp)
        : transactions(transactions), prevhash(prevBlock), nonce(nonce), blockHash(blockHash), timestamp(timestamp) {
        // No additional computation
        std::cout << "\033[34mSimplified Block constructor called.\033[0m" << std::endl;
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

////New one

// Merkle Node structure to represent each node in the Merkle Tree
struct MerkleNode
{
    std::string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(const std::string& hash_value) : hash(hash_value), left(nullptr), right(nullptr) {}
};

// Calculate the Merkle Root of all transactions in the block
std::string Block::calculateMerkleRoot()
{
    if (transactions.empty())
    {  
        return ""; // Return empty string if no transactions
    }

    // Step 1: Create leaf nodes for each transaction
    std::vector<MerkleNode*> nodes;

    for (const auto &tx : transactions)
    {
        std::string txData = tx.get_sender() + tx.get_receiver() + std::to_string(tx.get_amount());
        nodes.push_back(new MerkleNode(sha256(txData))); // Hash each transaction and create a node
    }

    // Step 2: Build the Merkle Tree
    while (nodes.size() > 1)
    {
        std::vector<MerkleNode*> newLevel;
        for (size_t i = 0; i < nodes.size(); i += 2)
        {
            // If there is an odd number of nodes, duplicate the last one
            if (i + 1 >= nodes.size())
            {
                nodes.push_back(nodes[i]);
            }

            // Combine each pair of adjacent hashes
            std::string combinedHash = sha256(nodes[i]->hash + nodes[i + 1]->hash);
            MerkleNode* parent = new MerkleNode(combinedHash);
            parent->left = nodes[i];
            parent->right = nodes[i + 1];
            newLevel.push_back(parent);
        }
        nodes = newLevel; // Move to the next level of the tree
    }

    // Step 3: The root of the Merkle Tree is the only element left
    return nodes.empty() ? "" : nodes[0]->hash;
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

