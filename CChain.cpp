#include "CChain.h"
using namespace blockchain;

CChain::CChain(int difficulty)
{
    mDifficulty = difficulty;
    mChain.push_back(CBlock(0));
    mCurrentBlock = &mChain[0];

}

void CChain::appendToCurrentBlock(uint8_t* data, uint32_t size)
{
    mCurrentBlock->appendData(data,size);
}

void CChain::nextBlock()
{
    CBlock block(mCurrentBlock);
    block.mine(mDifficulty);
    mChain.push_back(block);
    mCurrentBlock = &mChain.back();
}

CBlock* CChain::getCurrentBlock()
{
    return mCurrentBlock;
}