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

#include "Proof_Of_Work.h"  // Ensure this header file is included
#include "CBlock.h"
#include <string> // Include for std::string

namespace blockchain
{
    class CChain
    {
    private:
        struct Node
        {
            CBlock block;  // Block in the node
            Node* next;    // Pointer to the next node
            Node(CBlock blk) : block(blk), next(nullptr) {} // Node constructor
        };

        Node* mHead;      // Pointer to the first block in the chain
        Node* mTail;      // Pointer to the last block in the chain
        int mDifficulty;  // Difficulty level for mining

    public:
        CChain(int difficulty); // Constructor
        ~CChain();              // Destructor to free allocated memory

        ProofOfWork powobj;     // Instance of ProofOfWork for transaction signing
        void appendToCurrentBlock(uint8_t* data, uint32_t size); // Append data to the current block
        void nextBlock();       // Create the next block in the chain
        void createTransaction(const std::string &sender, const std::string &recipient, double amount); // Create and add a transaction
        CBlock* getCurrentBlock(); // Get the current block
    };
}

#endif // CCHAIN_H
