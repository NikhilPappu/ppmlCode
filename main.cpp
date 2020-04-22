#include <iostream>
#include <cstdlib>
#include <cryptoTools/Network/IOService.h>
#include "sh4/Sh4Encryptor.h"
#include "sh4/Sh4Evaluator.h"
#include "sh4/Sh4Runtime.h"

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
    block seed2 = toBlock(0, 1); 
    block seed3 = toBlock(0, 2); 
    block seed4 = toBlock(1, 0);
    block seed5 = toBlock(1, 1);
    block seed6 = toBlock(1, 2);
   
    enc[0].init(0, seed1, seed2, seed3);
    enc[1].init(1, seed1);
    enc[2].init(2, seed2);
    enc[3].init(3, seed3);

    Sh4Evaluator evals[4];
    evals[0].init(0, seed4, seed5, seed6);
    evals[1].init(1, seed4);
    evals[2].init(2, seed5);
    evals[3].init(3, seed6);

    bool failed = false;

    auto t0 = std::thread([&]()
    {
        Sh4Runtime rt(0, comm[0]);
        Sh4Task task;

        std::vector<si64> shr(4);
        shr[0] = enc[0].localInt(comm[0], 60);
        shr[1] = enc[0].remoteInt(1, comm[0]);
        shr[2] = enc[0].remoteInt(2, comm[0]);
        shr[3] = enc[0].remoteInt(3, comm[0]);

        //si64 shrEq = shr[0]*1 + shr[1]*2 + shr[2]*3 + shr[3]*4;
        si64 shrEq = evals[0].asyncMul(comm[0], shr[0], shr[1]) + shr[2]*3 + shr[3]*4;
        enc[0].revealSend(comm[0], shrEq);
        i64 ret0;
        ret0 = enc[0].revealRcv(comm[0], shrEq);
        std::cout << ret0 << std::endl;
    });
    
    auto t1 = std::thread([&]()
    {
        Sh4Runtime rt(1, comm[1]);
        std::vector<si64> shr(4);

        shr[0] = enc[1].remoteInt(0, comm[1]);
        shr[1] = enc[1].localInt(comm[1], 50);
        shr[2] = enc[1].remoteInt(2, comm[1]);
        shr[3] = enc[1].remoteInt(3, comm[1]);

        //si64 shrEq = shr[0]*1 + shr[1]*2 + shr[2]*3 + shr[3]*4;
        si64 shrEq = evals[1].asyncMul(comm[1], shr[0], shr[1]) + shr[2]*3 + shr[3]*4;
        enc[1].revealSend(comm[1], shrEq);
        i64 ret0 = enc[1].revealRcv(comm[1], shrEq);
        std::cout << ret0 << std::endl;
    });

    auto t2 = std::thread([&]()
    {
        Sh4Runtime rt(2, comm[2]);
        std::vector<si64> shr(4);
        shr[0] = enc[2].remoteInt(0, comm[2]);
        shr[1] = enc[2].remoteInt(1, comm[2]);
        shr[2] = enc[2].localInt(comm[2], 40);
        shr[3] = enc[2].remoteInt(3, comm[2]);

        //si64 shrEq = shr[0]*1 + shr[1]*2 + shr[2]*3 + shr[3]*4;
        si64 shrEq = evals[2].asyncMul(comm[2], shr[0], shr[1]) + shr[2]*3 + shr[3]*4;
        enc[2].revealSend(comm[2], shrEq);
        i64 ret0 = enc[2].revealRcv(comm[2], shrEq);
        std::cout << ret0 << std::endl;
    });

    auto t3 = std::thread([&]()
    {
        Sh4Runtime rt(3, comm[3]);
        std::vector<si64> shr(4);
        shr[0] = enc[3].remoteInt(0, comm[3]);
        shr[1] = enc[3].remoteInt(1, comm[3]);
        shr[2] = enc[3].remoteInt(2, comm[3]);
        shr[3] = enc[3].localInt(comm[3], 30);

        //si64 shrEq = shr[0]*1 + shr[1]*2 + shr[2]*3 + shr[3]*4;
        si64 shrEq = evals[3].asyncMul(comm[3], shr[0], shr[1]) + shr[2]*3 + shr[3]*4;
        enc[3].revealSend(comm[3], shrEq);
        i64 ret0 = enc[3].revealRcv(comm[3], shrEq);
        std::cout << ret0 << std::endl;
    });

    t0.join();
    t1.join();
    t2.join();
    t3.join();
}


int main()
{
    Sh4_Encryptor_IO_test();
    std::cout << "Yoooo!" << std::endl; 

    return 0;
}

