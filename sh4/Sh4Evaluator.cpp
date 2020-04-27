#include "Sh4Evaluator.h"
#include <cryptoTools/Crypto/PRNG.h>
#include <iomanip>
#include <cryptoTools/Common/Log.h>
#include <iostream>

using namespace oc;
namespace aby4
{
 
	si64 Sh4Evaluator::asyncMul(CommPkg& comm, const si64& A, const si64& B)
	{
        si64 C;
        C[0] = mShareGen.getShare();
        //std::cout << A[0] << std::endl; 

        if(mPartyIdx == 0)
        {
            i64 lmProd = A[0]*B[0];
            //std::cout << lmProd << std::endl; 

            i64 v = 0;
            comm.mPrev.asyncSendCopy(lmProd);
            comm.mNext.asyncSendCopy(v);
            comm.mFar.asyncSendCopy(v);
            C[1] = 0;
        }
        else
        {
            // ma= a + lma
            // c = a*b
            // mc - lmc = (ma - lma)*(mb - lmb)
            // mc = ma*mb - ma*lmb - mb*lma + lmProd + lmc

            i64 lmProd;
            comm.mFar.recv(lmProd);
		    C[1] = - A[1] * B[0]
			       - A[0] * B[1]
                   + lmProd
			       + C[0];
            //std::cout << A[1]*B[0]<< std::endl;   
            i64 C2, C3;
            comm.mPrev.asyncSendCopy(C[1]);
            comm.mNext.asyncSendCopy(C[1]);
            comm.mPrev.recv(C2);
            comm.mNext.recv(C3);
            C[1] = C[1] + C2 + C3 + A[1]*B[1];
        }

        return C;

	}

}
