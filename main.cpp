#include <iostream>
#include <cstdlib>
#include <cryptoTools/Network/IOService.h>
#include "sh4/Sh4Encryptor.h"

using namespace oc;
using namespace aby4;

/*
void setup(u64 partyIdx, IOService& ios, Sh4Encryptor& enc)
{
    CommPkg comm;         
    
    switch (partyIdx)
    {
        case 0:
            comm.mPrev = Session(ios, "127.0.0.1:1313", SessionMode::Server, "01").addChannel(); 
            comm.mNext = Session(ios, "127.0.0.1:1313", SessionMode::Server, "02").addChannel();
            comm.mFar = Session(ios, "127.0.0.1:1313", SessionMode::Server, "03").addChannel();
            break; 
    
        case 1:
            comm.mPrev = Session(ios, "127.0.0.1:1313", SessionMode::Server, "13").addChannel(); 
            comm.mNext = Session(ios, "127.0.0.1:1313", SessionMode::Server, "12").addChannel();
            comm.mFar = Session(ios, "127.0.0.1:1313", SessionMode::Client, "01").addChannel();
            break; 
 
        case 2:
            comm.mPrev = Session(ios, "127.0.0.1:1313", SessionMode::Client, "12").addChannel(); 
            comm.mNext = Session(ios, "127.0.0.1:1313", SessionMode::Server, "23").addChannel();
            comm.mFar = Session(ios, "127.0.0.1:1313", SessionMode::Client, "02").addChannel();
            break; 

        default:
            comm.mPrev = Session(ios, "127.0.0.1:1313", SessionMode::Client, "23").addChannel(); 
            comm.mNext = Session(ios, "127.0.0.1:1313", SessionMode::Client, "13").addChannel();
            comm.mFar = Session(ios, "127.0.0.1:1313", SessionMode::Client, "03").addChannel();
            break; 
    }
  
    block seed1 = sysRandomSeed();
    block seed2 = sysRandomSeed();
    block seed3 = sysRandomSeed();


    if (partyIdx == 0)
        enc.init(partyIdx, comm, seed1, seed2, seed3);
    else
        enc.init(partyIdx, comm);

}
*/

void Sh4_Encryptor_IO_test()
{

    IOService ios;

    auto chl01 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "01").addChannel(); 
    auto chl02 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "02").addChannel();
    auto chl03 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "03").addChannel();
   
    auto chl13 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "13").addChannel(); 
    auto chl12 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "12").addChannel();
    auto chl10 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "01").addChannel();


    auto chl21 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "12").addChannel(); 
    auto chl23 = Session(ios, "127.0.0.1:1313", SessionMode::Server, "23").addChannel();
    auto chl20 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "02").addChannel();


    auto chl32 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "23").addChannel(); 
    auto chl31 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "13").addChannel();
    auto chl30 = Session(ios, "127.0.0.1:1313", SessionMode::Client, "03").addChannel();

    CommPkg comm[4];
    comm[0] = { chl01, chl02, chl03};
    comm[1] = { chl13, chl12, chl10};
    comm[2] = { chl21, chl23, chl20};
    comm[3] = { chl32, chl31, chl30};

    Sh4Encryptor enc[4];
    block seed1 = toBlock(0, 0); 
    block seed2 = toBlock(1, 1); 
    block seed3 = toBlock(2, 2); 
    
    enc[0].init(0, seed1, seed2, seed3);
    enc[1].init(1, seed1);
    enc[2].init(2, seed2);
    enc[3].init(3, seed3);
}


int main()
{
    Sh4_Encryptor_IO_test();
    std::cout << "Yoooo!" << std::endl; 

    return 0;
}

