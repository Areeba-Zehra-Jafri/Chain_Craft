// #ifndef CCHAIN_H
// #define CCHAIN_H
// #include "CBlock.h"
// #include <vector>

// namespace blockchain
// {
//     class CChain
//     {
//     private:
//         std::vector<CBlock> mChain;
//         CBlock* mCurrentBlock;
//         int mDifficulty;
//     public:
//         CChain(int difficulty);
//         void appendToCurrentBlock(uint8_t* data, uint32_t size);
//         void nextBlock();
//         CBlock* getCurrentBlock();

//     };
// }

// #endif

#ifndef CCHAIN_H
#define CCHAIN_H

#include "CBlock.h"

namespace blockchain
{
    class CChain
    {
    private:
        struct Node
        {
            CBlock block;
            Node* next;
            Node(CBlock blk) : block(blk), next(nullptr) {}
        };

        Node* mHead;      // Pointer to the first block in the chain
        Node* mTail;      // Pointer to the last block in the chain
        int mDifficulty;

    public:
        CChain(int difficulty);
        ~CChain(); // Destructor to free allocated memory

        ProofOfWork powobj; 
        void appendToCurrentBlock(uint8_t* data, uint32_t size);
        void nextBlock();
        void createTransaction(const string &sender, const string &recipient, double amount);
        CBlock* getCurrentBlock();
    };
}

#endif 
