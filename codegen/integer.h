#ifndef __XENON_CODEGEN_INTEGER_H__
#define __XENON_CODEGEN_INTEGER_H__

#include "util.h"

namespace bf
{
template<int N>
class CompressedInteger;

template<int N>
class Integer
{   // Little-Endian, each cell contains 4-bit integer
private:
    ptr_t buf, data;
public:
    Integer(ptr_t p = -1)
    {
        if (p < 0)
        {
            p = malloc(size());
        }
        buf = p;
        data = p + buf_size();
    }

    constexpr static int buf_size()
    {
        return 3;
    }

    constexpr static int size()
    {
        return N + buf_size();
    }

    void operator +=(const Integer<N> &other) const
    {
        for (int i = 0; i < N; i++)
        {
            copy(other.data + i, buf, buf + 1);
            add_and_erase(buf, data + i);
        }
        normalize();
    }

    void operator -=(const Integer<N> &other) const
    {
        for (int i = 0; i < N; i++)
        {
            copy(other.data + i, buf, buf + 1);
            bf::negate(buf, buf + 1);
            bf::add_const(buf, 15);
            add_and_erase(buf, data + i);
        }
        bf::inc(data);
        normalize();
    }
    void negate() const
    {
        for (int i = 0; i < N; i++)
        {
            bf::negate(data + i, buf);
            bf::inc(data + i, 15);
        }
        bf::inc(data);
        normalize();
    }
    void normalize(int from = 0) const
    {
        bf::zero(buf);
        for (int i = from; i < N; i++)
        {
            add_and_erase(buf, data + i);
            mul16(data + i, buf, buf + 1);
            div16(buf, buf + 2, buf + 1);
            copy_and_erase(data + i, 1, buf + 1);
            copy(buf + 2, data + i, buf);
            bf::negate(buf + 2, buf);
            add_and_erase(buf + 2, buf + 1);
            div16(buf + 1, buf, buf + 2);
        }
    }
    void operator *=(const Integer<N> &other) const
    {
        for (int i = N - 1; i >= 0; i--)
        {
            copy_and_erase(data + i, 1, other.buf);
            for (int j = i; j < N; j++)
            {
                mul(other.buf, other.data + j - i, other.buf + 1, buf);
                add_and_erase(other.buf + 1, data + j);
            }
            normalize(i);
        }
    }
    void assign(Integer<N> &other) const
    {
        for (int i = 0; i < N; i++)
        {
            copy(other.data + i, data + i, buf);
        }
    }

    void inc() const
    {
        bf::inc(data);
        normalize();
    }

    void dec() const
    {
        for (int i = 0; i < N; i++)
        {
            bf::inc(data + i, 15);
        }
        normalize();
    }

    void print() const
    {
        for (int i = N - 1; i >= 0; i--)
        {
            outhex(data + i, buf);
        }
    }
    void println() const
    {
        print();
        outch('\n', buf);
    }
    void zero() const
    {
        for (int i = 0; i < N; i++)
        {
            bf::zero(data + i);
        }
    }

    ptr_t get_data() const
    {
        return data;
    }

    ptr_t get_buf() const
    {
        return buf;
    }

    void copy_to(Integer<N> other) const
    {
        memcpy(data, other.data, N, buf);
    }

    void assign(long long x) const
    {
        bool neg = x < 0;
        int off = sizeof(x) * 8 - 4;
        for (int i = 0; i < N; i++)
        {
            bf::assign(data + i, x & 15);
            x >>= 4;
            if (neg)
            {
                x |= 15 << off;
            }
        }
    }

    void add_const(long long x) const
    {
        bool neg = x < 0;
        int off = sizeof(x) * 8 - 4;
        for (int i = 0; i < N; i++)
        {
            bf::add_const(data + i, x & 15);
            x >>= 4;
            if (neg)
            {
                x |= 15 << off;
            }
        }
        normalize();
    }

    void is_zero(ptr_t ans) const
    {
        bf::zero(buf);
        for (int i = 0; i < N; i++)
        {
            copy(data + i, buf + 1, buf + 2);
            add_and_erase(buf + 1, buf);
            bool_cast(buf, buf + 1);
        }
        bool_not(buf, ans, buf + 1);
    }

    void is_negative(ptr_t ans) const
    {
        mul16(data + N - 1, buf, buf + 1);
        bf::is_negative(buf, ans, buf + 1);
    }

    void swap(const Integer<N> &n) const
    {
        for (int i = 0; i < N; i++)
        {
            bf::swap(data + i, n.data + i, buf);
        }
    }

    friend ToStrImpl<Integer<N>> ;
    friend CompressedInteger<N> ;
};

template<int N>
struct ToStrImpl<Integer<N>>
{
    static std::string f(Integer<N> )
    {
        std::ostringstream buf;
        // buf << '(' LOG(x.buf) LOG(x.data) LOG(N) << ')';
        return buf.str();
    }
};

template<int N>
void unsigned_cmp(Integer<N> &a, Integer<N> &b, ptr_t ans)
{
    ptr_t not_done_flag_1 = a.get_buf(), not_done_flag_2 = a.get_buf() + 1;
    assign(not_done_flag_1, 1);
    assign(not_done_flag_2, 1);
    assign(ans, 0);
    for (int i = N - 1; i >= 0; i--)
    {
        if_begin(not_done_flag_1);
        {
            ptr_t A = a.get_data() + i, B = b.get_data() + i, buf = b.get_buf();
            copy(B, buf, buf + 1);
            negate(buf, buf + 1);
            add_and_erase(buf, A);
            is_negative(A, buf, buf + 1);
            if_begin(buf);
            {
                assign(ans, -1);
                zero(not_done_flag_2);
            }
            if_end(buf);

            negate(A, buf);
            is_negative(A, buf, buf + 1);
            if_begin(buf);
            {
                assign(ans, 1);
                zero(not_done_flag_2);
            }
            if_end(buf);

            negate(A, buf);
            copy(B, buf, buf + 1);
            add_and_erase(buf, A);
        }
        if_end(not_done_flag_1);
        copy(not_done_flag_2, not_done_flag_1, a.get_buf() + 2);
    }
}

template<int N>
void signed_cmp(Integer<N> &a, Integer<N> &b, ptr_t ans)
{
    add_const(a.get_data() + N - 1, 8);
    add_const(b.get_data() + N - 1, 8);
    a.normalize(N - 1);
    b.normalize(N - 1);
    unsigned_cmp(a, b, ans);
    add_const(a.get_data() + N - 1, 8);
    add_const(b.get_data() + N - 1, 8);
    a.normalize(N - 1);
    b.normalize(N - 1);
}
}

#endif // __XENON_CODEGEN_INTEGER_H__
