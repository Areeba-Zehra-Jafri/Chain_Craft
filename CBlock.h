#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "Transaction.h"
#include "SHA256.h"  // Include our custom SHA-256 implementation

class Block {
public:
    Block* prevhash;           // Pointer to the previous block (linked list)
    std::string blockHash;      // Hash of the current block
    std::vector<Transaction> transactions; // List of transactions in this block
    std::time_t timestamp;      // Timestamp for when this block was created
    int nonce;                  // Nonce used for mining
    int difficulty;             // The difficulty level for mining this block
    std::string merkleRoot; // Store the Merkle Root of the block

    // Constructor to initialize a block with transactions, previous block pointer, and difficulty
    Block(std::vector<Transaction> transactions, Block* prevBlock, int difficulty);

    // Method to mine the block by finding a nonce that satisfies the difficulty
    std::string mineBlock();

    // Method to generate the hash of the block's contents
    std::string generateHash() const;

    // Getter for the previous block
    Block* getPrevBlock() const;

    // Setter for the previous block
    void setPrevBlock(Block* prev);

    std::string calculateMerkleRoot();

    std::string getMerkleRoot();

private:
    // Utility method to perform SHA-256 hashing using the custom SHA256 class
    std::string sha256(const std::string& str) const;
};
