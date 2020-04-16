#pragma once
#include "Sh4Types.h"
#include "Sh4ShareGen.h"
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
        
           
        si64 localInt(u64 partyIdx, CommPkg& comm, i64 val);
        si64 remoteInt(u64 partyIdx, CommPkg& comm);
    };
}
