#ifndef CCHAIN_H
#define CCHAIN_H
#include "CBlock.h"
#include <vector>

namespace blockchain
{
    class CChain
    {
    private:
        std::vector<CBlock> mChain;
        CBlock* mCurrentBlock;
        int mDifficulty;
    public:
        CChain(int difficulty);
        void appendToCurrentBlock(uint8_t* data, uint32_t size);
        void nextBlock();
        CBlock* getCurrentBlock();

    };
}

#endif