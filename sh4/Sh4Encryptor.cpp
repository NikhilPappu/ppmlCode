#include "Sh4Encryptor.h"
#include <libOTe/Tools/Tools.h>

namespace aby4
{
     
    si64 Sh4Encryptor::localInt(CommPkg & comm, i64 val)
    {
        si64 ret;
        ret[0] = mShareGen.getShare();

        if(mPartyIdx == 0)
        {
            ret[1] = 0;

            comm.mPrev.asyncSendCopy(val + ret[0]);
            comm.mNext.asyncSendCopy(val + ret[0]);
            comm.mFar.asyncSendCopy(val + ret[0]);
        }
        else
        {
            comm.mFar.recv(ret[1]);
            ret[1] = val + ret[1];
            comm.mPrev.asyncSendCopy(ret[1]);
            comm.mNext.asyncSendCopy(ret[1]);
        }
        //std::cout << ret[0] << " : " << ret[1] << std::endl;
        return ret;
    }

    Sh4Task Sh4Encryptor::localInt(Sh4Task dep, i64 val, si64& dest)
    {
         
        return dep.then([this, val, &dest](CommPkg& comm, Sh4Task& self) {
            dest[0] = mShareGen.getShare();
            dest[1] = 0;

            if(mPartyIdx == 0) 
            {
                    comm.mPrev.asyncSendCopy(val + dest[0]);
                    comm.mNext.asyncSendCopy(val + dest[0]);
                    comm.mFar.asyncSendCopy(val + dest[0]);
            }
            else
            {
                    comm.mFar.recv(dest[1]);
                    dest[1] = val + dest[1];
                    comm.mPrev.asyncSendCopy(dest[1]);
                    comm.mNext.asyncSendCopy(dest[1]);
            }
        });

    }

    Sh4Task Sh4Encryptor::remoteInt(Sh4Task dep, u64 sPartyIdx, si64& dest)
    {
        return dep.then([this, sPartyIdx, &dest](CommPkg& comm, Sh4Task& self){
            dest[0] = mShareGen.getShare();

            if(sPartyIdx == 0)
            {
                comm.mFar.recv(dest[1]); 
            }
            else if(sPartyIdx == 1)
            {
                if(mPartyIdx == 0)
                    comm.mPrev.asyncSendCopy(dest[0]);
                else if(mPartyIdx == 2)
                    comm.mPrev.recv(dest[1]);
                else if(mPartyIdx == 3)
                    comm.mNext.recv(dest[1]);
            }

            else if(sPartyIdx == 2)
            {
                if(mPartyIdx == 0)
                    comm.mNext.asyncSendCopy(dest[0]);
                else if(mPartyIdx == 1)
                    comm.mNext.recv(dest[1]);
                else if(mPartyIdx == 3)
                    comm.mPrev.recv(dest[1]);
            }
            
            else if(sPartyIdx == 3)
            {

                if(mPartyIdx == 0)
                    comm.mFar.asyncSendCopy(dest[0]);
                else if(mPartyIdx == 1)
                    comm.mPrev.recv(dest[1]);
                else if(mPartyIdx == 2)
                comm.mNext.recv(dest[1]);
            }                 
        });
    }

    si64 Sh4Encryptor::remoteInt(u64 sPartyIdx, CommPkg & comm)
    {

        si64 ret;
        ret[0] = mShareGen.getShare();

        if(sPartyIdx == 0)
        {
            comm.mFar.recv(ret[1]); 
        }
        else if(sPartyIdx == 1)
        {
            if(mPartyIdx == 0)
                comm.mPrev.asyncSendCopy(ret[0]);
            else if(mPartyIdx == 2)
                comm.mPrev.recv(ret[1]);
            else if(mPartyIdx == 3)
                comm.mNext.recv(ret[1]);
        }

        else if(sPartyIdx == 2)
        {
            if(mPartyIdx == 0)
                comm.mNext.asyncSendCopy(ret[0]);
            else if(mPartyIdx == 1)
                comm.mNext.recv(ret[1]);
            else if(mPartyIdx == 3)
                comm.mPrev.recv(ret[1]);
        }
        
        else if(sPartyIdx == 3)
        {

            if(mPartyIdx == 0)
                comm.mFar.asyncSendCopy(ret[0]);
            else if(mPartyIdx == 1)
                comm.mPrev.recv(ret[1]);
            else if(mPartyIdx == 2)
                comm.mNext.recv(ret[1]);
        }

        //std::cout << ret[0] << " : " << ret[1] << std::endl;
        return ret;
                
            
    }

    i64 Sh4Encryptor::revealRcv(CommPkg &comm, const si64& x)
    {
        i64 s;
        if(mPartyIdx == 0)
        {
            comm.mPrev.recv(s);     
            return s - x[0];
        }
        else
        {
            comm.mFar.recv(s);
            return x[1] - s;
        }
    }
    void Sh4Encryptor::revealSend(CommPkg & comm, const si64 & x)
    {
        if(mPartyIdx == 0)
        {
            comm.mPrev.asyncSendCopy(x[0]);
            comm.mNext.asyncSendCopy(x[0]);
            comm.mFar.asyncSendCopy(x[0]);
        }
        else if (mPartyIdx == 1)
        {
            comm.mFar.asyncSendCopy(x[1]);
        }
    }

    Sh4Task Sh4Encryptor::revealRcv(Sh4Task dep, const si64& x, i64& dest)
    {
        return dep.then([this, &x, &dest](CommPkg& comm, Sh4Task& self){
                   
            if(this->mPartyIdx == 0)
            {
                comm.mPrev.recv(dest);     
                dest = dest - x[0];
            }
            else
            {
                comm.mFar.recv(dest);
                dest = x[1] - dest;
            }
        });
    }
    Sh4Task Sh4Encryptor::revealSend(Sh4Task dep, const si64 & x)
    {
        return dep.then([this, &x](CommPkg& comm, Sh4Task& self){
            if(this->mPartyIdx == 0)
            {
                comm.mPrev.asyncSendCopy(x[0]);
                comm.mNext.asyncSendCopy(x[0]);
                comm.mFar.asyncSendCopy(x[0]);
            }
            else if (mPartyIdx == 1)
            {
                comm.mFar.asyncSendCopy(x[1]);
            }
        });
    }

}
