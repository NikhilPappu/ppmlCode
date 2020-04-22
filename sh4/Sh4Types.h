#pragma once
#include "../Common/Defines.h"
#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Common/Matrix.h>

namespace aby4
{
    struct CommPkg {
        oc::Channel mPrev, mNext, mFar;

    };



    struct si64
    {
        using value_type = i64;
        std::array<value_type, 2> mData;

        si64() = default;
        si64(const si64&) = default;
        si64(si64&&) = default;
        si64(const std::array<value_type, 2>& d) :mData(d) {}

        si64& operator=(const si64& copy);
        si64 operator+(const si64& rhs) const;
        si64 operator-(const si64& rhs) const;
        si64 operator*(const int i) const; 

        value_type& operator[](u64 i) { return mData[i]; }
        const value_type& operator[](u64 i) const { return mData[i]; }

    };
 
    inline si64 si64::operator*(const int i) const 
    {
        si64 ret;
        ret.mData[0] = i*mData[0];
        ret.mData[1] = i*mData[1];
        return ret;
    }
    inline si64& si64::operator=(const si64& copy)
    {
        mData[0] = copy.mData[0];
        mData[1] = copy.mData[1];
        return *this;
    }

    inline si64 si64::operator+(const si64& rhs) const
    {
        si64 ret;
        ret.mData[0] = mData[0] + rhs.mData[0];
        ret.mData[1] = mData[1] + rhs.mData[1];
        return ret;
    }

    inline si64 si64::operator-(const si64& rhs) const
    {
        si64 ret;
        ret.mData[0] = mData[0] - rhs.mData[0];
        ret.mData[1] = mData[1] - rhs.mData[1];
        return ret;
    }
}
