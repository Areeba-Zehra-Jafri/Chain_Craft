#include "CBlock.h"
#include <cstring>   // For memcpy
#include <iostream>  // For debugging output (optional)
#include <unistd.h>
#include <openssl/ssl.h>        // SSL/TLS functions
#include <openssl/err.h>        // Error handling
#include <openssl/evp.h>        // EVP API for hashing
#include <openssl/rsa.h>        // RSA encryption functions
#include <openssl/pem.h>        // PEM for reading/writing keys
#include <openssl/x509.h>       // X.509 certificate handling
#include <openssl/hmac.h>       // HMAC functions
#include <windows.h>

using namespace std;
using namespace blockchain;

// Constructor implementation
CBlock::CBlock(CBlock *prevBlock)
{
    mpPrevBlock = prevBlock; // Set the pointer to the previous block
    if (mpPrevBlock)
    {
        memcpy(mPrevHash, mpPrevBlock->getHash(), SHA256_DIGEST_LENGTH);
    }
    else
    {
        memset(mPrevHash, 0, SHA256_DIGEST_LENGTH);
    }
    mData = nullptr;          // Initialize data pointer to nullptr
    mDataSize = 0;            // Data size is zero at creation
    mCreatedTS = time(0);     // Set the creation timestamp to current time
    mNonce = 0;               // Initialize nonce to zero

    calculateHash();
}

void CBlock::printTransactions() const {
    cout << "Transactions in this block:" << endl;
    // for (const auto& tx : transactions) {
    //     cout << "Sender: " << tx.sender << endl;
    //     cout << "Recipient: " << tx.recipient << endl;
    //     cout << "Amount: " << tx.amount << endl;
    //     cout << "Signature: " << tx.signature << endl;
    //     cout << "-----------------------------" << endl;
    // }
}

void CBlock::calculateHash()
{
    size_t sz = SHA256_DIGEST_LENGTH + sizeof(time_t) + mDataSize + sizeof(uint32_t);
    uint8_t* buf = new uint8_t[sz];
    uint8_t* ptr = buf;

    memcpy(ptr, mPrevHash, SHA256_DIGEST_LENGTH);
    ptr += SHA256_DIGEST_LENGTH;
    memcpy(ptr, &mCreatedTS, sizeof(time_t));
    ptr += sizeof(time_t);
    if (mDataSize != 0)
    {
        memcpy(ptr, mData, mDataSize);
        ptr += mDataSize;
    }
    memcpy(ptr, &mNonce, sizeof(uint32_t));

    // Using EVP API for SHA-256 hashing
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context == nullptr) {
        delete[] buf;
        throw std::runtime_error("Failed to create EVP context");
    }

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(context, buf, sz) != 1 ||
        EVP_DigestFinal_ex(context, mHash, nullptr) != 1) 
    {
        EVP_MD_CTX_free(context);
        delete[] buf;
        throw std::runtime_error("Failed to calculate hash");
    }

    EVP_MD_CTX_free(context);
    delete[] buf;
}

uint8_t* CBlock::getHash()
{
    return mHash;
}

std::string CBlock::getHashStr()
{
    char buf[SHA256_DIGEST_LENGTH*2+1];
    for (uint32_t n = 0; n < SHA256_DIGEST_LENGTH; n++)
    {
        sprintf(buf + (n * 2), "%02x", mHash[n]);
    }
    buf[SHA256_DIGEST_LENGTH*2] = '\0';
    return std::string(buf);
}

CBlock* CBlock::getPrevBlock()
{
    return mpPrevBlock;
}

void CBlock::appendData(uint8_t *data, uint32_t size)
{
    uint8_t* newData = new uint8_t[mDataSize + size];
    uint8_t* ptr = newData;
    if (mDataSize != 0)
    {
        memcpy(ptr, mData, mDataSize);
        ptr += mDataSize;
        delete[] mData;
    }
    memcpy(ptr, data, size);
    mData = newData;
    mDataSize += size;
}

bool CBlock::isDifficulty(int difficulty)
{
    for (uint32_t n = 0; n < difficulty; n++)
    {
        if (mHash[n] != 0)
            return false;
    }
    return true;
}

void CBlock::mine(int difficulty)
{
    while (!isDifficulty(difficulty))
    {
        mNonce++;
        calculateHash();
        Sleep(10);
    }
}

uint32_t CBlock::getNonce()
{
    return mNonce;
}

void CBlock::addTransaction(const Transaction &tx) {
    transactions.push_back(tx);
}
