#include "CBlock.h"
#include "SHA256.h"  // Include custom SHA256 implementation
#include <sstream>
#include <iomanip>

// Constructor to initialize a Block with given transactions, previous hash, and difficulty level.
Block::Block(std::vector<Transaction> transactions, Block* previoushash, int difficulty) {
    this->transactions = std::move(transactions);
    this->prevhash = previoushash;  // Store the previous block pointer
    this->timestamp = std::time(nullptr);
    this->difficulty = difficulty;
    this->nonce = 0;
    this->blockHash = mineBlock(); // Compute the block's hash
}

// Mining function to find a valid hash for the block based on the given difficulty
std::string Block::mineBlock() {
    std::string target(difficulty, '0');
    while (blockHash.substr(0, difficulty) != target) {
        nonce++;
        blockHash = generateHash();
    }
    return blockHash;
}

// Generate the hash of the block
std::string Block::generateHash() const {
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&timestamp), "%Y-%m-%dT%H:%M:%S");  // Time in ISO format

    // Append all transaction details to the hash input
    for (const auto& tx : transactions) {
        ss << tx.get_sender() << tx.get_receiver() << tx.get_amount();
    }

    // Append the previous block's hash (use prevHash->blockHash for the previous block's hash)
    if (prevhash != nullptr) {
        ss << prevhash->blockHash;  // Correctly access the previous block's hash
    }

    // Append the nonce value (which will be mined)
    ss << nonce;

    // Return the final hash (SHA-256) of the string
    return sha256(ss.str());
}

// Compute SHA-256 hash for a given string using the custom SHA256 class
std::string Block::sha256(const std::string& str) const {
    SHA256 sha256;
    return sha256.hash(str);  // Assuming SHA256 has a hash() method that returns a std::string
}

// Define the setter and getter methods
Block* Block::getPrevBlock() const {
    return prevhash;
}

void Block::setPrevBlock(Block* prev) {
    prevhash = prev;
}