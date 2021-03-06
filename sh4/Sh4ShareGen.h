#pragma once
#include "Sh4Types.h"
#include <cryptoTools/Crypto/PRNG.h>
#include <iostream>

namespace aby4
{
    struct Sh4ShareGen
    {
        oc::PRNG mPrevCommon, mNextCommon, mFarCommon;

        u64 mPartyIdx = -1;
        u64 mShareIdx = 0, mShareGenIdx = 0;
        std::array<oc::AES, 3> mShareGen;
        std::array<std::vector<block>, 3> mShareBuff;

        void init (u64 partyIdx, block seed1, block seed2,
                   block seed3, u64 buffSize = 256)
        {        
            mPartyIdx = partyIdx;
 
            mPrevCommon.SetSeed(seed1);
            mNextCommon.SetSeed(seed2);
            mFarCommon.SetSeed(seed3);
        
            mShareGenIdx = 0;
            mShareBuff[0].resize(buffSize);
            mShareBuff[1].resize(buffSize);
            mShareBuff[2].resize(buffSize);

            mShareGen[0].setKey(mPrevCommon.get<block>());
            mShareGen[1].setKey(mNextCommon.get<block>());
            mShareGen[2].setKey(mFarCommon.get<block>());

            refillBuffer();
//            std::cout << *(u64*)((u8*)mShareBuff[0].data() + mShareIdx) << std::endl;
//            std::cout << *(u64*)((u8*)mShareBuff[1].data() + mShareIdx) << std::endl;
//            std::cout << *(u64*)((u8*)mShareBuff[2].data() + mShareIdx) << std::endl;
        }
        
        void init (u64 partyIdx, block seed, u64 buffSize = 256)
        {

            mPartyIdx = partyIdx;

            mFarCommon.SetSeed(seed);

            mShareGenIdx = 0;
            mShareBuff[0].resize(buffSize);

            mShareGen[0].setKey(mFarCommon.get<block>());
        
            refillBuffer(); 
 //           std::cout << *(u64*)((u8*)mShareBuff[0].data() + mShareIdx) << std::endl;
        }

        void refillBuffer()
        {
            
            mShareGen[0].ecbEncCounterMode(mShareGenIdx, mShareBuff[0].size(),
            mShareBuff[0].data());
            
            if(mPartyIdx == 0)
            {
                mShareGen[1].ecbEncCounterMode(mShareGenIdx, mShareBuff[1].size(),
                mShareBuff[1].data());

                mShareGen[2].ecbEncCounterMode(mShareGenIdx, mShareBuff[2].size(),
                mShareBuff[2].data());

            } 

            mShareGenIdx += mShareBuff[0].size();
            mShareIdx = 0;
        }
        
   
          
        i64 getShare()
        {
            if (mShareIdx + sizeof(i64) > mShareBuff[0].size() * sizeof(block))
            {
                refillBuffer();
            }
            
            short x1, x2, x3;

            i64 ret;
            if (mPartyIdx == 0)
            {
                x1 = *(short*)((u8*)mShareBuff[0].data() + mShareIdx);
                x2 = *(short*)((u8*)mShareBuff[1].data() + mShareIdx);
                x3 = *(short*)((u8*)mShareBuff[2].data() + mShareIdx);
                ret = (i64)(x1) + (i64)(x2) + (i64)(x3);
                //std::cout << "ret " << ret << std::endl;

            }
            else
            {
                x1 = *(short*)((u8*)mShareBuff[0].data() + mShareIdx); 
                ret = (i64)(x1);
                //std::cout << ret  << std::endl;
            }
            mShareIdx += sizeof(i64);
             
            return ret;
        }

    };
}
