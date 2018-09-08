#include "util.h"
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <iostream>

using std::FILE;
using std::fread;
using std::fopen;
using std::fclose;
using std::fputc;
using std::fputs;
using std::vector;
using std::map;

namespace bf {
ptr_t ptr = 0, allocated = 0;
bool safe_flag = true;
FILE *output = NULL;
int log_indentation = -1;
map<const char *, ptr_t> strings;
vector<ptr_t> offsets;

void indent() {
    for (int i = 0; i < log_indentation; i++) {
        std::cerr << ' ';
    }
}

void unsafe_while_begin() {
    fputc('[', output);
}

void unsafe_while_end() {
    fputc(']', output);
}

void unsafe_move_delta(int delta) {
    while (delta > 0) {
        fputc('>', output);
        delta--;
    }

    while (delta < 0) {
        fputc('<', output);
        delta++;
    }
}

void move_to(ptr_t p) {
    assert(safe_flag);
    int delta = p - ptr;

    unsafe_move_delta(delta);

    ptr = p;
}

void push_ptr_offset() {
    offsets.push_back(ptr);
    ptr = 0;
}

void pop_ptr_offset() {
    assert(!offsets.empty());
    ptr = offsets.back();
    offsets.pop_back();
}

void in(ptr_t p) {
    move_to(p);
    fputc(',', output);
}

void out(ptr_t p) {
    move_to(p);
    fputc('.', output);
}

void unsafe() {
    safe_flag = false;
}

void safe() {
    safe_flag = true;
}

ptr_t malloc(int size) {
    allocated += size;
    return allocated - size;
}

void memcpy(ptr_t from, ptr_t to, int size, ptr_t buf) {
    DECL_BUF(1)

    if (from > to) {
        for (int i = 0; i < size; i++) {
            copy(from + i, to + i, buf);
        }
    } else {
        for (int i = size - 1; i >= 0; i--) {
            copy(from + i, to + i, buf);
        }
    }
}

void outch(char ch, ptr_t buf) {
    DECL_BUF(1);
    assign(buf, ch);
    out(buf);
}

ptr_t loadstr(const char *str) {
    if (strings.find(str) == strings.end()) {
        int n = strlen(str);
        ptr_t addr = malloc(n + 2);
        assign(addr, 0);

        for (int i = 0; i < n; i++) {
            assign(addr + i + 1, str[i]);
        }

        assign(addr + n + 1, 0);
        strings[str] = addr;
    }

    return strings[str];
}

void printstr(ptr_t p) {
    move_to(p);
    unsafe();
    fputs(">[.>]<[<]", output);
    safe();
}

void outstr(const char *str) {
    printstr(loadstr(str));
}

void outbool(ptr_t p) {
    inc(p, '0');
    out(p);
    dec(p, '0');
}

void outhex(ptr_t p, ptr_t buf) {
    DECL_BUF(3)
    dec(p, 10);
    is_negative(p, buf, buf + 1);
    inc(p, 10);
    bool_not(buf, buf + 1, buf + 2);
    if_begin(buf);
    {
        add_const(p, '0');
        out(p);
        add_const(p, -1 * '0');
    }
    if_end(buf);
    if_begin(buf + 1);
    {
        add_const(p, 'A' - 10);
        out(p);
        add_const(p, 10 - 'A');
    }
    if_end(buf + 1);
}

void load_bytes_from_file(ptr_t p, int size, std::string &fname) {
    FILE *f = fopen(fname.c_str(), "rb");
    vector<char> buf(size);
    fread(&buf[0], sizeof(buf[0]), buf.size(), f);

    for (int i = 0; i < size; i++) {
        assign(p + i, buf[i]);
    }

    fclose(f);
}

void dump_stderr() {
    fputc('#', output);
}

void breakpoint() {
    fputc('B', output);
}

void loop_begin(ptr_t cond) {
    move_to(cond);
    fputc('[', output);
}

void loop_end(ptr_t cond) {
    move_to(cond);
    fputc(']', output);
}

void inc(ptr_t p, int val) {
    move_to(p);

    while (val--) {
        fputc('+', output);
    }
}

void dec(ptr_t p, int val) {
    move_to(p);

    while (val--) {
        fputc('-', output);
    }
}

void add_const(ptr_t p, int val) {
    if (val > 0) {
        inc(p, val);
    } else {
        dec(p, -val);
    }
}

void zero(ptr_t p) {
    loop_begin(p);
    {
        dec(p);
    }
    loop_end(p);
}

void assign(ptr_t p, int val) {
    zero(p);
    add_const(p, val);
}

void if_begin(ptr_t cond) {
    loop_begin(cond);
    zero(cond);
}

void if_end(ptr_t cond) {
    loop_end(cond);
}

void copy_and_erase(ptr_t from, int count, ...) {
    vector<ptr_t> dest(count);
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        dest[i] = va_arg(args, ptr_t);
    }

    va_end(args);

    for (ptr_t to : dest) {
        zero(to);
    }

    loop_begin(from);
    {
        dec(from);

        for (ptr_t to : dest) {
            inc(to);
        }
    }
    loop_end(from);
}

void copy(ptr_t from, ptr_t to, ptr_t buf) {
    DECL_BUF(1);
    copy_and_erase(from, 2, to, buf);
    copy_and_erase(buf, 1, from);
}

void swap(ptr_t a, ptr_t b, ptr_t buf) {
    DECL_BUF(1);
    copy_and_erase(a, 1, buf);
    copy_and_erase(b, 1, a);
    copy_and_erase(buf, 1, b);
}

void div2(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    zero(buf);
    zero(ans);
    loop_begin(val);
    {
        dec(val, 2);
        inc(ans);
        inc(buf, 2);
    }
    loop_end(val);
    copy_and_erase(buf, 1, val);
}

void div16(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    zero(buf);
    zero(ans);
    loop_begin(val);
    {
        dec(val, 16);
        inc(ans);
        inc(buf, 16);
    }
    loop_end(val);
    copy_and_erase(buf, 1, val);
}

void mul2(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    copy_and_erase(val, 3, ans, ans, buf);
    copy_and_erase(buf, 1, val);
}

void mul16(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    zero(buf);
    zero(ans);
    loop_begin(val);
    {
        dec(val);
        inc(ans, 16);
        inc(buf);
    }
    loop_end(val);
    copy_and_erase(buf, 1, val);
}

void add_and_erase(ptr_t from, ptr_t to) {
    loop_begin(from);
    {
        dec(from);
        inc(to);
    }
    loop_end(from);
}

void negate(ptr_t p, ptr_t buf) {
    DECL_BUF(1);
    zero(buf);
    loop_begin(p);
    {
        dec(p);
        dec(buf);
    }
    loop_end(p);
    copy_and_erase(buf, 1, p);
}

void bool_cast(ptr_t p, ptr_t buf) {
    DECL_BUF(1);
    zero(buf);
    if_begin(p);
    {
        inc(buf);
    }
    if_end(p);
    copy_and_erase(buf, 1, p);
}

void bool_or(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(2);
    zero(ans);
    copy(a, ans, buf);
    copy(b, buf, buf + 1);
    add_and_erase(buf, ans);
    bool_cast(ans, buf);
}

void bool_not(ptr_t a, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    copy(a, ans, buf);
    negate(ans, buf);
    inc(ans);
}

void bool_negate(ptr_t a, ptr_t buf) {
    DECL_BUF(1);
    negate(a, buf);
    inc(a);
}

void bool_and(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(2);
    assign(ans, -2);
    copy(a, buf, buf + 1);
    add_and_erase(buf, ans);
    copy(b, buf, buf + 1);
    add_and_erase(buf, ans);
    if_begin(ans);
    {
        assign(buf, 1);
    }
    if_end(ans);
    bool_not(buf, ans, buf + 1);
}

void bool_xor(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(2);
    copy(a, ans, buf);
    copy(b, buf, buf + 1);
    negate(buf, buf + 1);
    add_and_erase(buf, ans);
    bool_cast(ans, buf);
}

void is_negative(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(2);
    mul2(val, ans, buf);
    div2(ans, buf, buf + 1);
    copy(val, buf + 1, ans);
    negate(buf, ans);
    add_and_erase(buf, buf + 1);
    zero(ans);
    if_begin(buf + 1);
    {
        inc(ans);
    }
    if_end(buf + 1);
}

void is_zero(ptr_t val, ptr_t ans, ptr_t buf) {
    DECL_BUF(1);
    copy(val, buf, ans);
    assign(ans, 1);
    if_begin(buf);
    {
        zero(ans);
    }
    if_end(buf);
}

void less_7bit(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(3);
    mul2(a, ans, buf + 2);
    div2(ans, buf, buf + 2);
    mul2(b, ans, buf + 2);
    div2(ans, buf + 1, buf + 2);
    negate(buf + 1, buf + 2);
    add_and_erase(buf + 1, buf);
    is_negative(buf, ans, buf + 1);
}

void less_1bit(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(4);
    is_negative(a, ans, buf);
    is_negative(b, buf, buf + 1);
    bool_not(ans, buf + 1, buf + 2);
    bool_and(buf + 1, buf, ans, buf + 2);
}

void less_signed(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(5);
    ptr_t A = ans;
    ptr_t B = buf;
    ptr_t C = buf + 1;
    dump_stderr();
    less_1bit(a, b, A, buf);
    dump_stderr();
    less_1bit(b, a, B, buf + 1);
    dump_stderr();
    less_7bit(a, b, C, buf + 2);
    dump_stderr();

    /*
     outstr("DEBUG(");
     outbool(A);
     outbool(B);
     outbool(C);
     outstr(")\n");
     */

    bool_or(B, C, buf + 2, buf + 3);
    copy_and_erase(buf + 2, 1, B);
    bool_negate(A, buf + 1);
    bool_and(A, B, C, buf + 2);
    copy_and_erase(C, 1, ans);
}

void less_unsigned(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(5);
    ptr_t A = ans;
    ptr_t B = buf;
    ptr_t C = buf + 1;
    less_1bit(b, a, A, buf);
    less_1bit(a, b, B, buf + 1);
    less_7bit(a, b, C, buf + 2);

    /*
     outstr("DEBUG(");
     outbool(A);
     outbool(B);
     outbool(C);
     outstr(")\n");
     */

    bool_or(B, C, buf + 2, buf + 3);
    copy_and_erase(buf + 2, 1, B);
    bool_negate(A, buf + 1);
    bool_and(A, B, C, buf + 2);
    copy_and_erase(C, 1, ans);
}

void mul(ptr_t a, ptr_t b, ptr_t ans, ptr_t buf) {
    DECL_BUF(2);
    copy(a, buf, ans);
    zero(ans);
    loop_begin(buf);
    {
        dec(buf);
        copy_and_erase(b, 1, buf + 1);
        loop_begin(buf + 1);
        {
            dec(buf + 1);
            inc(b);
            inc(ans);
        }
        loop_end(buf + 1);
    }
    loop_end(buf);
}
}
