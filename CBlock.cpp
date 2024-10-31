#include "CBlock.h"
#include <cstring>  // Include for memcpy if you use it
#include <iostream> // Include for debugging output (optional)
#include <unistd.h>
#include <openssl/ssl.h>        // SSL/TLS functions
#include <openssl/err.h>        // Error handling
#include <openssl/sha.h>        // SHA hashing functions
#include <openssl/rsa.h>        // RSA encryption functions
#include <openssl/pem.h>        // PEM (Privacy Enhanced Mail) functions for reading/writing keys
#include <openssl/x509.h>       // X.509 certificate handling
#include <openssl/hmac.h>       // HMAC (Hash-based Message Authentication Code) functions


using namespace blockchain;

// Constructor implementation
CBlock::CBlock(CBlock *prevBlock)
{
    mpPrevBlock = prevBlock; // Set the pointer to the previous block
                             //    memeset(mHash,SHA256_DIGEST_LENGTH,0);
    if(mpPrevBlock)
    {
        memcpy(mPrevHash,mpPrevBlock->getHash(),SHA256_DIGEST_LENGTH);
    }
    else
    {
        memset(mPrevHash,SHA256_DIGEST_LENGTH,0);
    }
    mData = 0;            // Initially, there's no data
    mDataSize = 0;        // Data size is zero at creation
    mCreatedTS = time(0); // Set the creation timestamp to current time
    mNonce = 0;           // Initialize nonce to zero

    calculateHash();
}

// Implement other methods below...

void CBlock::calculateHash()
{
    uint32_t sz = (SHA256_DIGEST_LENGTH * sizeof(uint8_t)) + sizeof(time_t) + mDataSize + sizeof(uint32_t);
    uint8_t* buf = new uint8_t[sz];
    uint8_t* ptr = buf;

    memcpy(ptr, mPrevHash, SHA256_DIGEST_LENGTH * sizeof(uint8_t));
    ptr += 20 * sizeof(uint8_t);
    memcpy(ptr, &mCreatedTS, sizeof(time_t));
    ptr+= sizeof(time_t);
    if(mDataSize!=0)
    {
        memcpy(ptr,mData,mDataSize);
        ptr += mDataSize;
    }
    memcpy(ptr,&mNonce,sizeof(uint32_t));
    ptr+= sizeof(uint32_t);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256,buf,sz);
    SHA256_Final(mHash,&sha256);
}

uint8_t *CBlock::getHash()
{
    return mHash;
}

std::string CBlock::getHashStr()
{
    char buf[SHA256_DIGEST_LENGTH*2+1];
    for(uint32_t n=0;n<SHA256_DIGEST_LENGTH;n++)
    {
        sprintf(buf + (n*2),"%02x",mHash[n]);
    }
    buf[SHA256_DIGEST_LENGTH*2]=0;
    return std::string(buf);
   // Implement logic to convert the hash to string
    return ""; // Placeholder
}

CBlock *CBlock::getPrevBlock()
{
    return mpPrevBlock; // Return pointer to the previous block
}

void CBlock::appendData(uint8_t *data, uint32_t size)
{
    uint8_t* newData = new uint8_t[mDataSize+size];
    uint8_t* ptr= newData;
    if(mDataSize!=0)
    {
        memcpy(ptr,mData,mDataSize);
        ptr+=mDataSize;
        delete[] mData;
    }
    memcpy(ptr,data,size);
    mData=newData;
    mDataSize +=size;
}

bool CBlock::isDifficulty(int difficulty)
{
    for(uint32_t n=0;n<difficulty;n++)
    {
        if(mHash[n]!=0)
            return false;
    }
    return true;
}

void CBlock::mine(int difficulty)
{
    while(isDifficulty(difficulty))
    {
        mNonce++;
        calculateHash();
        usleep(10);
    }
}

uint32_t CBlock::getNonce()
{
    return mNonce; // Return the current nonce
}

void CBlock::addTransaction(const Transaction &tx) {
    transactions.push_back(tx);
}