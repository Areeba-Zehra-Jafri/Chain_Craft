// #include "CChain.h"
// using namespace blockchain;

// CChain::CChain(int difficulty)
// {
//     mDifficulty = difficulty;
//     mChain.push_back(CBlock(0));
//     mCurrentBlock = &mChain[0];

// }

// void CChain::appendToCurrentBlock(uint8_t* data, uint32_t size)
// {
//     mCurrentBlock->appendData(data,size);
// }

// void CChain::nextBlock()
// {
//     CBlock block(mCurrentBlock);
//     block.mine(mDifficulty);
//     mChain.push_back(block);
//     mCurrentBlock = &mChain.back();
// }

// CBlock* CChain::getCurrentBlock()
// {
//     return mCurrentBlock;
// }
#include "CChain.h"
#include "Transaction.h"
#include "Proof_Of_Work.h"
#include "CBlock.h"
#include <iostream>

using namespace blockchain;

CChain::CChain(int difficulty) : mHead(nullptr), mTail(nullptr), mDifficulty(difficulty)
{
    // Create the first block with a null previous block (nullptr)
    CBlock genesisBlock(nullptr);
    genesisBlock.mine(mDifficulty);

    // Create the head node and set it as both head and tail
    mHead = new Node(genesisBlock);
    mTail = mHead;
}

CChain::~CChain()
{
    // Free memory allocated for the chain
    Node* current = mHead;
    while (current != nullptr)
    {
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

void CChain::appendToCurrentBlock(uint8_t* data, uint32_t size)
{
    if (mTail != nullptr)
    {
        mTail->block.appendData(data, size);
    }
}

void CChain::nextBlock()
{
    if (mTail != nullptr)
    {
        // Create a new block with the current tail's block as the previous block
        CBlock newBlock(&mTail->block); // Pass the pointer to the current tail's block
        newBlock.mine(mDifficulty);

        // Create a new node and set it as the next node for the tail
        Node* newNode = new Node(newBlock);
        mTail->next = newNode;

        // Update the tail to the new node
        mTail = newNode;
    }
}

CBlock* CChain::getCurrentBlock()
{
    // Return the block stored in the tail node
    return (mTail != nullptr) ? &mTail->block : nullptr;
}

void CChain::createTransaction(const std::string &sender, const std::string &recipient, double amount) {
    Transaction tx(sender, recipient, amount);
    tx.signTransaction(powobj); // Ensure powobj is defined and accessible
    if (mTail != nullptr) {
        mTail->block.addTransaction(tx); // Add the transaction to the current block
    }
}
