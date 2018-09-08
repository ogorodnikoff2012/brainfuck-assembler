/*
 * instructions.h
 *
 *  Created on: 24 июл. 2016 г.
 *      Author: xenon
 */

#ifndef __XENON_CODEGEN_INSTRUCTIONS_H__
#define __XENON_CODEGEN_INSTRUCTIONS_H__

#include "util.h"
#include "integer.h"
#include "compressed_integer.h"
#include "byte_array.h"
#include "tree_byte_array.h" // <- MOD
#include <functional>

struct Environment
{
public:
    bf::Integer<4> ra, rb, rc, rd;
    bf::CompressedInteger<4> ci_buf;
    bf::ptr_t buf;
    // bf::ByteArray<65536, 4> data, code, stack;
    bf::TreeByteArray<4> data;
    bf::TreeByteArray<4> code;
    bf::TreeByteArray<4> stack;
    Environment() :
        ra(), rb(), rc(), rd(), ci_buf(), buf(bf::malloc(16)), data(), code(), stack()
    {
    }
};

typedef std::function<void(Environment &)> Operation;

enum Instruction
{
    HALT,
    NOP,
    INB,
    OUTB,
    ASSIGN_A_B,
    ASSIGN_A_C,
    ASSIGN_A_D,
    ASSIGN_A_DP,
    ASSIGN_A_CONST,
    ASSIGN_A_MEM,
    ASSIGN_B_A,
    ASSIGN_B_C,
    ASSIGN_B_D,
    ASSIGN_B_DP,
    ASSIGN_B_CONST,
    ASSIGN_B_MEM,
    ASSIGN_C_A,
    ASSIGN_C_B,
    ASSIGN_C_D,
    ASSIGN_C_DP,
    ASSIGN_C_CONST,
    ASSIGN_C_MEM,
    ASSIGN_DP_A,
    ASSIGN_DP_B,
    ASSIGN_DP_C,
    ASSIGN_DP_D,
    ASSIGN_DP_CONST,
    // ASSIGN_DP_MEM, // PROHIBITED!!!
    ASSIGN_MEM_A,
    ASSIGN_MEM_B,
    ASSIGN_MEM_C,
    ASSIGN_MEM_D,
    // ASSIGN_MEM_DP, // PROHIBITED!!!
    ASSIGN_MEM_CONST,
    ASSIGN_D_A,
    ASSIGN_D_B,
    ASSIGN_D_C,
    ASSIGN_D_DP,
    ASSIGN_D_MEM,
    ASSIGN_D_CONST,
    ADD,
    SUB,
    MUL,
    DIVMOD,
    CMP_UNSIGNED,
    CMP_SIGNED,
    COMP_A,
    COMP_B,
    COMP_C,
    COMP_D,
    COMP_DP,
    DECOMP_A,
    DECOMP_B,
    DECOMP_C,
    DECOMP_D,
    DECOMP_DP,
    JZ,
    JN,
    JP,
    JMP,
    JZ_MEM,
    JN_MEM,
    JP_MEM,
    JMP_MEM,
    PUSH,
    POP,
    SP_MOVE,
    CALL,
    CALL_MEM,
    RET,
    BOOL_NOT,
    BOOL_AND,
    BOOL_OR,
    BOOL_XOR,
    DUMP,
};

void op_halt(Environment &env);
void op_nop(Environment &env);
void op_inb(Environment &env);
void op_outb(Environment &env);
void op_assign_a_b(Environment &env);
void op_assign_a_c(Environment &env);
void op_assign_a_d(Environment &env);
void op_assign_a_dp(Environment &env);
void op_assign_a_const(Environment &env);
void op_assign_a_mem(Environment &env);
void op_assign_b_a(Environment &env);
void op_assign_b_c(Environment &env);
void op_assign_b_d(Environment &env);
void op_assign_b_dp(Environment &env);
void op_assign_b_const(Environment &env);
void op_assign_b_mem(Environment &env);
void op_assign_c_a(Environment &env);
void op_assign_c_b(Environment &env);
void op_assign_c_d(Environment &env);
void op_assign_c_dp(Environment &env);
void op_assign_c_const(Environment &env);
void op_assign_c_mem(Environment &env);
void op_assign_dp_a(Environment &env);
void op_assign_dp_b(Environment &env);
void op_assign_dp_c(Environment &env);
void op_assign_dp_d(Environment &env);
void op_assign_dp_const(Environment &env);
void op_assign_mem_a(Environment &env);
void op_assign_mem_b(Environment &env);
void op_assign_mem_c(Environment &env);
void op_assign_mem_d(Environment &env);
void op_assign_mem_const(Environment &env);
void op_assign_d_a(Environment &env);
void op_assign_d_b(Environment &env);
void op_assign_d_c(Environment &env);
void op_assign_d_dp(Environment &env);
void op_assign_d_mem(Environment &env);
void op_assign_d_const(Environment &env);
void op_add(Environment &env);
void op_sub(Environment &env);
void op_mul(Environment &env);
void op_divmod(Environment &env);
void op_cmp_unsigned(Environment &env);
void op_cmp_signed(Environment &env);
void op_comp_a(Environment &env);
void op_comp_b(Environment &env);
void op_comp_c(Environment &env);
void op_comp_d(Environment &env);
void op_comp_dp(Environment &env);
void op_decomp_a(Environment &env);
void op_decomp_b(Environment &env);
void op_decomp_c(Environment &env);
void op_decomp_d(Environment &env);
void op_decomp_dp(Environment &env);
void op_jz(Environment &env);
void op_jn(Environment &env);
void op_jp(Environment &env);
void op_jmp(Environment &env);
void op_jz_mem(Environment &env);
void op_jn_mem(Environment &env);
void op_jp_mem(Environment &env);
void op_jmp_mem(Environment &env);
void op_push(Environment &env);
void op_pop(Environment &env);
void op_sp_move(Environment &env);
void op_call(Environment &env);
void op_call_mem(Environment &env);
void op_ret(Environment &env);
void op_bool_not(Environment &env);
void op_bool_and(Environment &env);
void op_bool_or(Environment &env);
void op_bool_xor(Environment &env);
void op_dump(Environment &env);

extern const Operation OPS[];
extern const int OPS_SIZE;

#endif /* INSTRUCTIONS_H_ */
