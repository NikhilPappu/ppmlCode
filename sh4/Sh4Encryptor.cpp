#include "Sh4Encryptor.h"
#include <libOTe/Tools/Tools.h>

namespace aby4
{
     
    si64 Sh4Encryptor::localInt(u64 partyIdx, CommPkg & comm, i64 val)
    {
        si64 ret;
        ret[0] = mShareGen.getShare();

        if(partyIdx == 0)
        {
            ret[1] = 0;

            comm.mPrev.asyncSendCopy(val - ret[0]);
            comm.mNext.asyncSendCopy(val - ret[0]);
            comm.mFar.asyncSendCopy(val - ret[0]);
        }
        else
        {
            comm.mFar.recv(ret[1]);
            ret[1] = val - ret[1];
            comm.mPrev.asyncSendCopy(ret[1]);
            comm.mNext.asyncSendCopy(ret[1]);
        }
        //std::cout << ret[0] << " : " << ret[1] << std::endl;
        return ret;
    }

    si64 Sh4Encryptor::remoteInt(u64 partyIdx, u64 sPartyIdx, CommPkg & comm)
    {

        si64 ret;
        ret[0] = mShareGen.getShare();

        if(sPartyIdx == 0)
        {
            comm.mFar.recv(ret[1]); 
        }
        else if(sPartyIdx == 1)
        {
            if(partyIdx == 0)
                comm.mPrev.asyncSendCopy(ret[0]);
            else if(partyIdx == 2)
                comm.mPrev.recv(ret[1]);
            else if(partyIdx == 3)
                comm.mNext.recv(ret[1]);
        }

        else if(sPartyIdx == 2)
        {
            if(partyIdx == 0)
                comm.mNext.asyncSendCopy(ret[0]);
            else if(partyIdx == 1)
                comm.mNext.recv(ret[1]);
            else if(partyIdx == 3)
                comm.mPrev.recv(ret[1]);
        }
        
        else if(sPartyIdx == 3)
        {

            if(partyIdx == 0)
                comm.mFar.asyncSendCopy(ret[0]);
            else if(partyIdx == 1)
                comm.mPrev.recv(ret[1]);
            else if(partyIdx == 2)
                comm.mNext.recv(ret[1]);
        }

        //std::cout << ret[0] << " : " << ret[1] << std::endl;
        return ret;
    }

    i64 Sh4Encryptor::revealRcv(CommPkg &comm, u64 partyIdx, const si64& x)
    {
        i64 s;
        if(partyIdx == 0)
        {
            comm.mPrev.recv(s);     
            return s + x[0];
        }
        else
        {
            comm.mFar.recv(s);
            return s + x[1];
        }
    }
    void Sh4Encryptor::revealSend(CommPkg & comm, u64 partyIdx, const si64 & x)
    {
        if(partyIdx == 0)
        {
            comm.mPrev.asyncSendCopy(x[0]);
            comm.mNext.asyncSendCopy(x[0]);
            comm.mFar.asyncSendCopy(x[0]);
        }
        else if (partyIdx == 1)
        {
            comm.mFar.asyncSendCopy(x[1]);
        }
    }
}
