#pragma once
#include "Sh4Types.h"
#include "Sh4ShareGen.h"
#include "Sh4Runtime.h"
#include <cryptoTools/Common/MatrixView.h>

namespace aby4
{
    class Sh4Encryptor
    {
    public:

        u64 mPartyIdx = -1;
        Sh4ShareGen mShareGen;
        
        void init(u64 partyIdx, block seed1, block& seed2, block& seed3,
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
        
           
        si64 localInt(CommPkg& comm, i64 val);
        Sh4Task localInt(Sh4Task dep, i64 val, si64& dest);


        si64 remoteInt(u64 sPartyIdx, CommPkg& comm);
        Sh4Task remoteInt(Sh4Task dep, u64 sPartyIdx, si64& dest);

        void revealSend(CommPkg& comm, const si64& x);
        i64 revealRcv(CommPkg& comm, const si64& x);

        Sh4Task revealSend(Sh4Task dep, const si64& x);
        Sh4Task revealRcv(Sh4Task dep, const si64& x, i64& dest);

    };
}
