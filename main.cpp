#include <iostream>
#include <cstdlib>
#include <cryptoTools/Network/IOService.h>
#include "sh4/Sh4Encryptor.h"
#include "sh4/Sh4Evaluator.h"
#include "sh4/Sh4Runtime.h"

using namespace oc;
using namespace aby4;


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
        std::vector<si64> shr(4);
        auto& e = enc[0];
        auto& ev = evals[0];
        Sh4Task task;
        task = e.localInt(rt.noDependencies(), 60, shr[0]);
        task&= e.remoteInt(rt.noDependencies(), 1, shr[1]);
        task&= e.remoteInt(rt.noDependencies(), 2, shr[2]);
        task&= e.remoteInt(rt.noDependencies(), 3, shr[3]);

        task.get();

        si64 prod;
        task = ev.asyncMul(task, shr[0], shr[1], prod);
        task.get();
        

        si64 shrEq = prod + shr[2]*3 + shr[3]*4;
        e.revealSend(rt.noDependencies(), shrEq).get();
        i64 ret0;
        e.revealRcv(rt.noDependencies(), shrEq, ret0).get();
        std::cout << ret0 << std::endl;
    });
    
    auto t1 = std::thread([&]()
    {
        Sh4Runtime rt(1, comm[1]);
        std::vector<si64> shr(4); 
        auto& e = enc[1];
        auto& ev = evals[1];
        Sh4Task task;
        task = e.remoteInt(rt.noDependencies(), 0, shr[0]);
        task&= e.localInt(rt.noDependencies(), 50, shr[1]);
        task&= e.remoteInt(rt.noDependencies(), 2, shr[2]);
        task&= e.remoteInt(rt.noDependencies(), 3, shr[3]);

        task.get();

        si64 prod;
        task = ev.asyncMul(task, shr[0], shr[1], prod);
        task.get();
        

        si64 shrEq = prod + shr[2]*3 + shr[3]*4;

        e.revealSend(rt.noDependencies(), shrEq).get();
        i64 ret0;
        e.revealRcv(rt.noDependencies(), shrEq, ret0).get();
        std::cout << ret0 << std::endl;
    });

    auto t2 = std::thread([&]()
    {
        Sh4Runtime rt(2, comm[2]);
        std::vector<si64> shr(4);
        auto& e = enc[2];
        auto& ev = evals[2];
        Sh4Task task;
        task = e.remoteInt(rt.noDependencies(), 0, shr[0]);
        task&= e.remoteInt(rt.noDependencies(), 1, shr[1]);
        task&= e.localInt(rt.noDependencies(), 40, shr[2]);
        task&= e.remoteInt(rt.noDependencies(), 3, shr[3]);

        task.get();

        si64 prod;
        task = ev.asyncMul(task, shr[0], shr[1], prod);
        task.get();
        

        si64 shrEq = prod + shr[2]*3 + shr[3]*4;
        e.revealSend(rt.noDependencies(), shrEq).get();
        i64 ret0;
        e.revealRcv(rt.noDependencies(), shrEq, ret0).get();
        std::cout << ret0 << std::endl;
    });

    auto t3 = std::thread([&]()
    {
        Sh4Runtime rt(3, comm[3]);
        std::vector<si64> shr(4);
        auto& e = enc[3];
        auto& ev = evals[3];
        Sh4Task task;
        task = e.remoteInt(rt.noDependencies(), 0, shr[0]);
        task &= e.remoteInt(rt.noDependencies(), 1, shr[1]);
        task &= e.remoteInt(rt.noDependencies(), 2, shr[2]);
        task&= e.localInt(rt.noDependencies(), 30, shr[3]);

        task.get();

        si64 prod;
        task = ev.asyncMul(task, shr[0], shr[1], prod);
        task.get();
        

        si64 shrEq = prod + shr[2]*3 + shr[3]*4;
        e.revealSend(rt.noDependencies(), shrEq).get();
        i64 ret0;
        e.revealRcv(rt.noDependencies(), shrEq, ret0).get();
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
//    Sh4_Encryptor_asyncIO_test();
    std::cout << "Yoooo!" << std::endl; 

    return 0;
}

