#ifndef __XENON_CODEGEN_COMPRESSED_INTEGER_H__
#define __XENON_CODEGEN_COMPRESSED_INTEGER_H__

#include "util.h"
#include "integer.h"

namespace bf
{
template<int N>
class CompressedInteger
{
private:
    ptr_t data;
public:
    CompressedInteger(ptr_t p = -1)
    {
        if (p < 0)
        {
            p = malloc(size());
        }
        data = p;
    }
    constexpr static int size()
    {
        return (N + 1) >> 1;
    }
    ptr_t get_data() const
    {
        return data;
    }
    void pack(Integer<N> x) const
    {
        for (int i = 0; i < N / 2; i++)
        {
            mul16(x.data + 2 * i + 1, x.buf, x.buf + 1);
            copy(x.data + 2 * i, x.buf + 1, x.buf + 2);
            add_and_erase(x.buf + 1, x.buf);
            copy_and_erase(x.buf, 1, data + i);
        }
        if (N % 2 == 1)
        {
            copy(x.data + N - 1, data + N / 2, x.buf);
        }
    }
    void unpack(Integer<N> x) const
    {
        x.zero();
        for (int i = 0; i < (N + 1) / 2; i++)
        {
            copy(data + i, x.data + 2 * i, x.buf);
        }
        x.normalize();
    }
    void zero() const
    {
        for (int i = 0; i < size(); i++)
        {
            bf::zero(data + i);
        }
    }
    friend ToStrImpl<CompressedInteger<N>> ;
};

template<int N>
struct ToStrImpl<CompressedInteger<N>>
{
    static std::string f(CompressedInteger<N> x)
    {
        std::ostringstream buf;
        // buf << '(' LOG(x.data) LOG(N) << ')';
        return buf.str();
    }
};

}

#endif

