#ifndef __XENON_CODEGEN_UTIL_H__
#define __XENON_CODEGEN_UTIL_H__

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#define DECL_BUF(x) int buf_size = x; \
                    static ptr_t buf_d = malloc(buf_size); \
                    if (buf < 0) { \
                        buf = buf_d; \
                    }
namespace bf
{
typedef int ptr_t;

extern FILE *output;
extern int log_indentation;

void indent();

template<class T>
struct ToStrImpl;

template<class T>
std::string to_str(T x)
{
    return ToStrImpl<T>::f(x);
}

template<class T>
struct ToStrImpl
{
    static std::string f(T x)
    {
        std::ostringstream buf;
        buf << x;
        return buf.str();
    }
};

template<>
struct ToStrImpl<char>
{
    static std::string f(char x)
    {
        std::ostringstream buf;
        buf << '\'';
        if (x < ' ')
        {
            buf << '\\' << (int)x;
        }
        else
        {
            buf << x;
        }
        buf << '\'';
        return buf.str();
    }
};

template<>
struct ToStrImpl<const char *>
{
    static std::string f(const char *x)
    {
        std::ostringstream buf;
        buf << '"';
        while (*x)
        {
            if (*x < ' ')
            {
                buf << '\\' << (int)*x;
            }
            else
            {
                buf << *x;
            }
            x++;
        }
        buf << '"';
        return buf.str();
    }
};

void unsafe_move_delta(int delta);
void unsafe_while_begin();
void unsafe_while_end();
void move_to(ptr_t p);
void push_ptr_offset();
void pop_ptr_offset();

void in(ptr_t p);
void out(ptr_t p);
void unsafe();
void safe();

ptr_t malloc(int size);
void memcpy(ptr_t from, ptr_t to, int size, ptr_t buf = -1); // DECL_BUF(1)
void outch(char ch, ptr_t buf = -1); // DECL_BUF(1)
ptr_t loadstr(const char *str);
void printstr(ptr_t p);
void outstr(const char *str);
void outbool(ptr_t p);
void outhex(ptr_t p, int buf = -1); // DECL_BUF(3)

void load_bytes_from_file(ptr_t p, int size, std::string &fname);

void dump_stderr();
void breakpoint();

void loop_begin(ptr_t cond);
void loop_end(ptr_t cond);

void inc(ptr_t p, int val = 1);
void dec(ptr_t p, int val = 1);
void add_const(ptr_t p, int val);

void zero(ptr_t p);
void assign(ptr_t p, int val);

template <class TBArray>
void load_bytes_from_file(const TBArray& array, int size, const std::string& fname) {
    FILE* f = fopen(fname.c_str(), "rb");
    char *buf = (char *)std::malloc(size);
    fread(buf, 1, size, f);
    for (int i = 0; i < size; ++i) {
        assign(array.get_phys_addr(i), buf[i]);
    }

    std::free(buf);

    fclose(f);
}

void if_begin(ptr_t cond);
void if_end(ptr_t cond);

void copy_and_erase(ptr_t from, int count, ...);
void copy(ptr_t from, ptr_t to, ptr_t buf = -1); // DECL_BUF(1)
void swap(ptr_t a, ptr_t b, ptr_t buf = -1); // DECL_BUF(1)

void div2(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)
void mul2(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)
void div16(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)
void mul16(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)

void add_and_erase(ptr_t from, ptr_t to);
void negate(ptr_t p, ptr_t buf = -1); // DECL_BUF(1)

void bool_cast(ptr_t p, ptr_t buf = -1); // DECL_BUF(1)
void bool_or(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(2)
void bool_and(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(2)
void bool_xor(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(2)
void bool_not(ptr_t a, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)
void bool_negate(ptr_t p, ptr_t buf = -1); // DECL_BUF(1)

void is_negative(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(2)
void is_zero(ptr_t val, ptr_t ans, ptr_t buf = -1); // DECL_BUF(1)
void less_7bit(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(3)
void less_1bit(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(4)
void less_signed(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); //DECL_BUF(5)
void less_unsigned(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); //DCLBUF(5)

void mul(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf = -1); // DECL_BUF(2)
}

#endif // __XENON_CODEGEN_UTIL_H__
