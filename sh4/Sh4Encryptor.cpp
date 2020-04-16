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
        return ret;
    }

    si64 Sh4Encryptor::remoteInt(u64 partyIdx, CommPkg & comm)
    {

    }

}
