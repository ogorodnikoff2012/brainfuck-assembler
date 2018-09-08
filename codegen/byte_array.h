#ifndef __XENON_CODEGEN_BYTE_ARRAY_H__
#define __XENON_CODEGEN_BYTE_ARRAY_H__

#include "util.h"
#include "integer.h"

using std::fputs;

namespace bf
{
template<int N, int M>
class ByteArray
{
private:
    ptr_t buf, byte, addr_register_ptr, terminator, addr_line, data;
    Integer<M> *addr_register;
    void move_from_terminator_to_addr_ptr() const
    {
        fputs("-[+>-]+", output);
    }
    void move_from_addr_ptr_to_data() const
    {
        for (int i = 0; i < N; i++)
        {
            fputc('>', output);
        }
    }
    void move_from_data_to_addr_ptr() const
    {
        for (int i = 0; i < N; i++)
        {
            fputc('<', output);
        }
    }
    void move_from_addr_ptr_to_terminator() const
    {
        fputs("+[-<+]-", output);
    }
    void move_from_terminator_to_data() const
    {
        move_from_terminator_to_addr_ptr();
        move_from_addr_ptr_to_data();
    }
    void move_from_data_to_terminator() const
    {
        move_from_data_to_addr_ptr();
        move_from_addr_ptr_to_terminator();
    }
public:
    ByteArray(ptr_t p = -1)
    {
        if (p < 0)
        {
            p = malloc(size());
        }
        buf = p;
        byte = buf + buf_size();
        addr_register_ptr = byte + 1;
        addr_register = new Integer<M>(addr_register_ptr);
        terminator = addr_register_ptr + Integer<M>::size();
        addr_line = terminator + 1;
        data = addr_line + N;
        assign(terminator, -1);
        assign(addr_line, 1);
    }

    constexpr static int size()
    {
        return 2 * N + Integer<M>::size() + 2 + buf_size();
    }

    constexpr static int buf_size()
    {
        return 2;
    }

    ptr_t get_byte() const
    {
        return byte;
    }

    ptr_t get_buf() const
    {
        return buf;
    }

    ptr_t get_data() const
    {
        return data;
    }

    void get() const
    {
        zero(byte);
        zero(buf);
        move_to(terminator);
        unsafe();
        move_from_terminator_to_data();
        fputs("[-", output);
        move_from_data_to_terminator();
        safe();
        inc(buf);
        inc(byte);
        move_to(terminator);
        unsafe();
        move_from_terminator_to_data();
        fputc(']', output);
        move_from_data_to_terminator();
        safe();

        loop_begin(buf);
        dec(buf);
        move_to(terminator);
        unsafe();
        move_from_terminator_to_data();
        fputc('+', output);
        move_from_data_to_terminator();
        safe();
        loop_end(buf);
    }

    void put() const
    {
        zero(buf);
        move_to(terminator);
        unsafe();
        move_from_terminator_to_data();
        fputs("[-]", output);
        move_from_data_to_terminator();
        safe();

        loop_begin(byte);
        dec(byte);
        inc(buf);
        move_to(terminator);
        unsafe();
        move_from_terminator_to_data();
        fputc('+', output);
        move_from_data_to_terminator();
        safe();
        loop_end(byte);

        copy_and_erase(buf, 1, byte);
    }

    void update_addr_ptr() const
    {
        move_to(terminator);
        unsafe();
        move_from_terminator_to_addr_ptr();
        fputc('-', output);
        move_from_addr_ptr_to_terminator();
        safe();
        assign(addr_line, 1);

        for (int i = 0; i < M; i++)
        {
            copy(addr_register->get_data() + i, buf, buf + 1);
            loop_begin(buf);
            dec(buf);
            move_to(terminator);
            unsafe();
            move_from_terminator_to_addr_ptr();
            fputc('-', output);
            for (int j = 0; j < (1 << (4 * i)); j++)
            {
                fputc('>', output);
            }
            fputc('+', output);
            move_from_addr_ptr_to_terminator();
            safe();
            loop_end(buf);
        }
    }

    void inc_addr_ptr() const
    {

        addr_register->inc();

        move_to(terminator);
        unsafe();
        move_from_terminator_to_addr_ptr();
        fputs("->+", output);
        move_from_addr_ptr_to_terminator();
        safe();

    }

    void dec_addr_ptr() const
    {

        addr_register->dec();

        move_to(terminator);
        unsafe();
        move_from_terminator_to_addr_ptr();
        fputs("-<+", output);
        move_from_addr_ptr_to_terminator();
        safe();

    }

    Integer<M> *get_addr_register() const
    {
        return addr_register;
    }

    friend ToStrImpl<ByteArray<N, M>> ;
};

template<int N, int M>
struct ToStrImpl<ByteArray<N, M>>
{
    static std::string f(ByteArray<N, M> x)
    {
        std::ostringstream buf;
        // buf << '(' LOG(x.buf) LOG(x.byte) LOG(x.addr_register_ptr) LOG(*x.addr_register) LOG(x.terminator) LOG(x.addr_line) LOG(x.data) LOG(N) LOG(M) << ')';
        return buf.str();
    }
};
}

#endif // __XENON_CODEGEN_BYTE_ARRAY_H__
