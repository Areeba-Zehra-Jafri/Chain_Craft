#ifndef CBLOCK_H
#define CBLOCK_H

#include <string>
#include<vector>
#include <cstdint>   // Include for fixed-width integer types
#include <ctime>    // Include for time_t
#include <openssl/ssl.h>        // SSL/TLS functions
#include <openssl/err.h>        // Error handling
#include <openssl/sha.h>        // SHA hashing functions
#include <openssl/rsa.h>        // RSA encryption functions
#include <openssl/pem.h>        // PEM (Privacy Enhanced Mail) functions for reading/writing keys
#include <openssl/x509.h>       // X.509 certificate handling
#include <openssl/hmac.h>       // HMAC (Hash-based Message Authentication Code) functions
#include "Transaction.h"

namespace blockchain
{
    class CBlock
    {
    private:
        // Uncomment these lines if you plan to implement hashing with OpenSSL
        uint8_t mHash[SHA256_DIGEST_LENGTH]; // Variable to store the block's hash
        uint8_t mPrevHash[SHA256_DIGEST_LENGTH]; // Variable to store the previous block's hash
        CBlock* mpPrevBlock; // Pointer to the previous block
        uint8_t* mData; // Pointer to the data stored in the block
        uint32_t mDataSize; // Size of the data
        time_t mCreatedTS; // Timestamp for when the block was created
        uint32_t mNonce; // Nonce value for mining
        vector<Transaction> transactions; // Store transactions in the block

    // Existing methods...

    public:
        // Constructor
        CBlock(CBlock* prevBlock);

        // Method to calculate the hash of the block
        void calculateHash();

        // Method to retrieve the hash
        uint8_t* getHash();

        // Method to get the hash as a string
        std::string getHashStr();

        // Method to get the previous block
        CBlock* getPrevBlock();

        // Method to append data to the block
        void appendData(uint8_t* data, uint32_t size);

        // Method to check if the block meets the difficulty level
        bool isDifficulty(int difficulty);

        // Method to mine the block
        void mine(int difficulty);

        // Method to get the nonce value
        uint32_t getNonce();

        //Method to  add transaction record
        void addTransaction(const Transaction &tx);

    
    };
}

#endif // CBLOCK_H
