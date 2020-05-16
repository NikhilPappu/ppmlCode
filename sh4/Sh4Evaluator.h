#pragma once
#include "Sh4Types.h"
#include "Sh4ShareGen.h"
#include "Sh4Runtime.h"

namespace aby4
{
    class Sh4Evaluator
    {
    public:

        u64 mPartyIdx = -1;
        Sh4ShareGen mShareGen;
        
        void init(u64 partyIdx, block& seed1, block& seed2, block& seed3,
                  u64 buffSize = 256)
        {
            mShareGen.init(partyIdx, seed1, seed2, seed3, buffSize); 
            mPartyIdx = partyIdx; 
        }

        void init(u64 partyIdx, block seed, u64 buffSize = 256)
        {
            mShareGen.init(partyIdx, seed, buffSize);
            mPartyIdx = partyIdx; 
        }

        si64 asyncMul(CommPkg& comm, const si64& A, const si64& B);
        Sh4Task asyncMul(Sh4Task dep, const si64& A, const si64& B, si64& C);

    };
}
