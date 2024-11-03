// CBlock.h

#ifndef CBLOCK_H
#define CBLOCK_H

#include <vector>
#include <cstdint>
#include <string>
#include <openssl/sha.h> // Include for SHA256_DIGEST_LENGTH
#include "Transaction.h" // Make sure to include the Transaction class

namespace blockchain {

class CBlock {
public:
    CBlock(CBlock* prevBlock);
    void mine(int difficulty);
    void appendData(uint8_t *data, uint32_t size);
    uint8_t *getHash();
    std::string getHashStr();
    CBlock *getPrevBlock();
    uint32_t getNonce();
    void addTransaction(const Transaction &tx);
    void printTransactions() const; // Declaration of the printTransactions method

private:
    uint8_t mPrevHash[SHA256_DIGEST_LENGTH]; // Previous block hash
    uint8_t mHash[SHA256_DIGEST_LENGTH];      // Current block hash
    time_t mCreatedTS;                         // Creation timestamp
    uint32_t mNonce;                           // Nonce for mining
    uint8_t *mData;                            // Data pointer
    uint32_t mDataSize;                        // Size of the data
    CBlock *mpPrevBlock;                       // Pointer to the previous block
    std::vector<Transaction> transactions;     // Vector to hold transactions

    void calculateHash();                       // Calculate the block hash
    bool isDifficulty(int difficulty);          // Check difficulty
};

} // namespace blockchain

#endif // CBLOCK_H
