/*
 * instructions.cpp
 *
 *  Created on: 24 июл. 2016 г.
 *      Author: xenon
 */

#include "instructions.h"
#include "util.h"

const Operation OPS[] = {
    op_halt, op_nop, op_inb, op_outb, op_assign_a_b, op_assign_a_c, op_assign_a_d,
    op_assign_a_dp, op_assign_a_const, op_assign_a_mem, op_assign_b_a,
    op_assign_b_c, op_assign_b_d, op_assign_b_dp, op_assign_b_const,
    op_assign_b_mem, op_assign_c_a, op_assign_c_b, op_assign_c_d,
    op_assign_c_dp, op_assign_c_const, op_assign_c_mem, op_assign_dp_a,
    op_assign_dp_b, op_assign_dp_c, op_assign_dp_d, op_assign_dp_const,
    op_assign_mem_a, op_assign_mem_b, op_assign_mem_c, op_assign_mem_d,
    op_assign_mem_const, op_assign_d_a, op_assign_d_b, op_assign_d_c,
    op_assign_d_dp, op_assign_d_mem, op_assign_d_const, op_add, op_sub,
    op_mul, op_divmod, op_cmp_unsigned, op_cmp_signed, op_comp_a, op_comp_b,
    op_comp_c, op_comp_d, op_comp_dp, op_decomp_a, op_decomp_b, op_decomp_c,
    op_decomp_d, op_decomp_dp, op_jz, op_jn, op_jp, op_jmp, op_jz_mem,
    op_jn_mem, op_jp_mem, op_jmp_mem, op_push, op_pop, op_sp_move, op_call,
    op_call_mem, op_ret, op_bool_not, op_bool_and, op_bool_or, op_bool_xor, op_dump
};

const int OPS_SIZE = sizeof(OPS) / sizeof(OPS[0]);

using namespace bf;

void op_halt(Environment &) {
}

void op_nop(Environment &) {
}

void op_inb(Environment &env) {
    env.ci_buf.zero();
    in(env.ci_buf.get_data());
}

void op_outb(Environment &env) {
    out(env.ci_buf.get_data());
}

void op_assign_a_b(Environment &env) {
    env.ra.assign(env.rb);
}

void op_assign_a_c(Environment &env) {
    env.ra.assign(env.rc);
}

void op_assign_a_dp(Environment &env) {
    env.ra.assign(*env.data.get_addr_register());
}

template<int K, class ByteArrayImpl>
void get_ci_from_buf(CompressedInteger<K> &ci, ByteArrayImpl &buf, bool ret =
                         false) {
    for (int i = 0; i < ci.size(); i++) {
        buf.get();
        copy_and_erase(buf.get_byte(), 1, ci.get_data() + i);
        buf.inc_addr_ptr();
    }

    if (ret) {
        buf.get_addr_register()->add_const(-ci.size());
        buf.update_addr_ptr();
    }
}

template<int K, class ByteArrayImpl>
void put_ci_in_buf(CompressedInteger<K> &ci, ByteArrayImpl &buf, bool ret =
                       false) {
    for (int i = 0; i < ci.size(); i++) {
        copy(ci.get_data() + i, buf.get_byte(), buf.get_buf());
        buf.put();
        buf.inc_addr_ptr();
    }

    if (ret) {
        buf.get_addr_register()->add_const(-ci.size());
        buf.update_addr_ptr();
    }
}

void op_assign_a_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(env.ra);
}

void op_assign_a_mem(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.data, true);
    env.ci_buf.unpack(env.ra);
}

void op_assign_b_a(Environment &env) {
    env.rb.assign(env.ra);
}

void op_assign_b_c(Environment &env) {
    env.rb.assign(env.rc);
}

void op_assign_b_dp(Environment &env) {
    env.rb.assign(*env.data.get_addr_register());
}

void op_assign_b_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(env.rb);
}

void op_assign_b_mem(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.data, true);
    env.ci_buf.unpack(env.rb);
}

void op_assign_c_a(Environment &env) {
    env.rc.assign(env.ra);
}

void op_assign_c_b(Environment &env) {
    env.rc.assign(env.rb);
}

void op_assign_c_dp(Environment &env) {
    env.rc.assign(*env.data.get_addr_register());
}

void op_assign_c_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(env.rc);
}

void op_assign_c_mem(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.data, true);
    env.ci_buf.unpack(env.rc);
}

void op_assign_dp_a(Environment &env) {
    env.data.get_addr_register()->assign(env.ra);
    env.data.update_addr_ptr();
}

void op_assign_dp_b(Environment &env) {
    env.data.get_addr_register()->assign(env.rb);
    env.data.update_addr_ptr();
}

void op_assign_dp_c(Environment &env) {
    env.data.get_addr_register()->assign(env.rc);
    env.data.update_addr_ptr();
}

void op_assign_dp_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(*env.data.get_addr_register());
    env.data.update_addr_ptr();
}

void op_assign_mem_a(Environment &env) {
    env.ci_buf.pack(env.ra);
    put_ci_in_buf(env.ci_buf, env.data, true);
}

void op_assign_mem_b(Environment &env) {
    env.ci_buf.pack(env.rb);
    put_ci_in_buf(env.ci_buf, env.data, true);
}

void op_assign_mem_c(Environment &env) {
    env.ci_buf.pack(env.rc);
    put_ci_in_buf(env.ci_buf, env.data, true);
}

void op_assign_mem_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    put_ci_in_buf(env.ci_buf, env.data, true);
}

void op_add(Environment &env) {
    env.ra += env.rb;
}

void op_sub(Environment &env) {
    env.ra -= env.rb;
}

void op_mul(Environment &env) {
    env.ra *= env.rb;
}

template<int N>
void rotate_right_once(Integer<N> &n, ptr_t buf = -1) {
    DECL_BUF(1);
    copy_and_erase(n.get_data() + N - 1, 1, buf);

    for (int i = N - 2; i >= 0; i--) {
        copy_and_erase(n.get_data() + i, 1, n.get_data() + i + 1);
    }

    copy_and_erase(buf, 1, n.get_data());
}

template<int N>
void rotate_left_once(Integer<N> &n, ptr_t buf = -1) {
    DECL_BUF(1);
    copy_and_erase(n.get_data(), 1, buf);

    for (int i = 1; i < N; i++) {
        copy_and_erase(n.get_data() + i, 1, n.get_data() + i - 1);
    }

    copy_and_erase(buf, 1, n.get_data() + N - 1);
}

template<int N>
void rotate_right(Integer<N> &b, ptr_t buf) {
    zero(buf);
    is_zero(b.get_data() + N - 1, b.get_buf(), b.get_buf() + 1);
    loop_begin(b.get_buf());
    {
        inc(buf);
        rotate_right_once(b, b.get_buf() + 1);
        is_zero(b.get_data() + N - 1, b.get_buf(), b.get_buf() + 1);
    }
    loop_end(b.get_buf());
}

void op_divmod(Environment &env) {
    auto &a = env.ra, &b = env.rb, &c = env.rc;
    b.is_zero(env.buf + 1);
    bool_negate(env.buf + 1, env.buf + 2);
    c.zero();
    if_begin(env.buf + 1);
    {
        rotate_right(b, env.buf + 2);
        inc(env.buf + 2);
        loop_begin(env.buf + 2);
        {
            rotate_right_once(c, c.get_buf());
            unsigned_cmp(a, b, env.buf + 3);
            inc(env.buf + 3);
            bool_cast(env.buf + 3, env.buf + 4);
            loop_begin(env.buf + 3);
            {
                a -= b;
                c.inc();
                unsigned_cmp(a, b, env.buf + 3);
                inc(env.buf + 3);
                bool_cast(env.buf + 3, env.buf + 4);

            }
            loop_end(env.buf + 3);
            dec(env.buf + 2);
            rotate_left_once(b, b.get_buf());
        }
        loop_end(env.buf + 2);
        rotate_right_once(b, b.get_buf());
    }
    if_end(env.buf + 1);
}

void cmp_stuff(Environment &env) {
    env.ra.assign(1);
    is_negative(env.buf + 1, env.buf + 2, env.buf + 3);
    if_begin(env.buf + 2);
    {
        env.ra.assign(-1);
    }
    if_end(env.buf + 2);
    is_zero(env.buf + 1, env.buf + 2, env.buf + 3);
    if_begin(env.buf + 2);
    {
        env.ra.zero();
    }
    if_end(env.buf + 2);
}

void op_cmp_unsigned(Environment &env) {
    unsigned_cmp(env.ra, env.rb, env.buf + 1);
    cmp_stuff(env);
}

void op_cmp_signed(Environment &env) {
    signed_cmp(env.ra, env.rb, env.buf + 1);
    cmp_stuff(env);
}

void op_comp_a(Environment &env) {
    env.ci_buf.pack(env.ra);
}

void op_comp_b(Environment &env) {
    env.ci_buf.pack(env.rb);
}

void op_comp_c(Environment &env) {
    env.ci_buf.pack(env.rc);
}

void op_comp_d(Environment &env) {
    env.ci_buf.pack(env.rd);
}

void op_comp_dp(Environment &env) {
    env.ci_buf.pack(*env.data.get_addr_register());
}

void op_decomp_a(Environment &env) {
    env.ci_buf.unpack(env.ra);
}

void op_decomp_b(Environment &env) {
    env.ci_buf.unpack(env.rb);
}

void op_decomp_c(Environment &env) {
    env.ci_buf.unpack(env.rc);
}

void op_decomp_d(Environment &env) {
    env.ci_buf.unpack(env.rd);
}

void op_decomp_dp(Environment &env) {
    env.ci_buf.unpack(*env.data.get_addr_register());
    env.data.update_addr_ptr();
}

void jmp_ci_buf(Environment &env) {
    env.ci_buf.unpack(*env.code.get_addr_register());
    env.code.update_addr_ptr();
}

void op_jz(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    get_ci_from_buf(env.ci_buf, env.code);
    if_begin(env.buf + 1);
    {
        jmp_ci_buf(env);
    }
    if_end(env.buf + 1);
}

void op_jn(Environment &env) {
    env.ra.is_negative(env.buf + 1);
    get_ci_from_buf(env.ci_buf, env.code);
    if_begin(env.buf + 1);
    {
        jmp_ci_buf(env);
    }
    if_end(env.buf + 1);
}

void op_jp(Environment &env) {
    env.ra.is_negative(env.buf + 2);
    env.ra.is_zero(env.buf + 3);
    bool_or(env.buf + 2, env.buf + 3, env.buf + 1, env.buf + 4);
    bool_negate(env.buf + 1, env.buf + 2);
    get_ci_from_buf(env.ci_buf, env.code);
    if_begin(env.buf + 1);
    {
        jmp_ci_buf(env);
    }
    if_end(env.buf + 1);
}

void op_jmp(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    jmp_ci_buf(env);
}

void op_jz_mem(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    if_begin(env.buf + 1);
    {
        op_jmp_mem(env);
    }
    if_end(env.buf + 1);
}

void op_jn_mem(Environment &env) {
    env.ra.is_negative(env.buf + 1);
    if_begin(env.buf + 1);
    {
        op_jmp_mem(env);
    }
    if_end(env.buf + 1);
}

void op_jp_mem(Environment &env) {
    env.ra.is_negative(env.buf + 2);
    env.ra.is_zero(env.buf + 3);
    bool_or(env.buf + 2, env.buf + 3, env.buf + 1, env.buf + 4);
    bool_negate(env.buf + 1, env.buf + 2);
    if_begin(env.buf + 1);
    {
        op_jmp_mem(env);
    }
    if_end(env.buf + 1);
}

void op_jmp_mem(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.data, true);
    env.ci_buf.unpack(*env.code.get_addr_register());
    env.code.update_addr_ptr();
}

void op_push(Environment &env) {
    put_ci_in_buf(env.ci_buf, env.stack);
}

void op_pop(Environment &env) {
    env.stack.get_addr_register()->add_const(-env.ci_buf.size());
    env.stack.update_addr_ptr();
    get_ci_from_buf(env.ci_buf, env.stack, true);
}

void op_sp_move(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(env.ra);
    *env.stack.get_addr_register() += env.ra;
    env.stack.update_addr_ptr();
}

void op_call(Environment &env) {
    env.code.get_addr_register()->add_const(env.ci_buf.size());
    env.ci_buf.pack(*env.code.get_addr_register());
    op_push(env);
    env.code.get_addr_register()->add_const(-env.ci_buf.size());
    op_jmp(env);
}

void op_call_mem(Environment& env) {
    env.ci_buf.pack(*env.code.get_addr_register());
    op_push(env);
    op_jmp_mem(env);
}

void op_ret(Environment &env) {
    op_pop(env);
    env.ci_buf.unpack(*env.code.get_addr_register());
    env.code.update_addr_ptr();
}

void op_bool_not(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    env.ra.zero();
    copy_and_erase(env.buf + 1, 1, env.ra.get_data());
}

void op_bool_and(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    env.rb.is_zero(env.buf + 2);
    bool_negate(env.buf + 1, env.buf + 3);
    bool_negate(env.buf + 2, env.buf + 3);
    env.ra.zero();
    bool_and(env.buf + 1, env.buf + 2, env.ra.get_data(), env.buf + 3);
}

void op_bool_or(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    env.rb.is_zero(env.buf + 2);
    bool_negate(env.buf + 1, env.buf + 3);
    bool_negate(env.buf + 2, env.buf + 3);
    env.ra.zero();
    bool_or(env.buf + 1, env.buf + 2, env.ra.get_data(), env.buf + 3);
}

void op_assign_a_d(Environment &env) {
    env.ra.assign(env.rd);
}

void op_assign_b_d(Environment &env) {
    env.rb.assign(env.rd);
}

void op_assign_c_d(Environment &env) {
    env.rc.assign(env.rd);
}

void op_assign_dp_d(Environment &env) {
    env.data.get_addr_register()->assign(env.rd);
    env.data.update_addr_ptr();
}

void op_assign_mem_d(Environment &env) {
    env.ci_buf.pack(env.rd);
    put_ci_in_buf(env.ci_buf, env.data, true);
}

void op_assign_d_a(Environment &env) {
    env.rd.assign(env.ra);
}

void op_assign_d_b(Environment &env) {
    env.rd.assign(env.rb);
}

void op_assign_d_c(Environment &env) {
    env.rd.assign(env.rc);
}

void op_assign_d_dp(Environment &env) {
    env.rd.assign(*env.data.get_addr_register());
}

void op_assign_d_mem(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.data, true);
    env.ci_buf.unpack(env.rd);
}

void op_assign_d_const(Environment &env) {
    get_ci_from_buf(env.ci_buf, env.code);
    env.ci_buf.unpack(env.rd);
}

void op_bool_xor(Environment &env) {
    env.ra.is_zero(env.buf + 1);
    env.rb.is_zero(env.buf + 2);
    // bool_negate(env.buf + 1, env.buf + 3);
    // bool_negate(env.buf + 2, env.buf + 3);
    env.ra.zero();
    bool_xor(env.buf + 1, env.buf + 2, env.ra.get_data(), env.buf + 3);
}

void op_dump(Environment &env) {
    env.ra.println();
    env.rb.println();
    env.rc.println();
    env.rd.println();
}
