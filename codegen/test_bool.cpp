#include "util.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

void out_bool(ptr_t p) {
    inc(p, '0');
    out(p);
    dec(p, '0');
}

void test_not(ptr_t a, int a_val, ptr_t res) {
    assign(a, a_val);
    out_bool(a);
    outch(' ');
    bool_not(a, res);
    out_bool(res);
    outch('\n');
}

void test_and(ptr_t a, int a_val, ptr_t b, int b_val, ptr_t res) {
    assign(a, a_val);
    assign(b, b_val);
    out_bool(a);
    outch(' ');
    out_bool(b);
    outch(' ');
    bool_and(a, b, res);
    out_bool(res);
    outch('\n');
}

void test_or(ptr_t a, int a_val, ptr_t b, int b_val, ptr_t res) {
    assign(a, a_val);
    assign(b, b_val);
    out_bool(a);
    outch(' ');
    out_bool(b);
    outch(' ');
    bool_or(a, b, res);
    out_bool(res);
    outch('\n');
}

void test_xor(ptr_t a, int a_val, ptr_t b, int b_val, ptr_t res) {
    assign(a, a_val);
    assign(b, b_val);
    out_bool(a);
    outch(' ');
    out_bool(b);
    outch(' ');
    bool_xor(a, b, res);
    out_bool(res);
    outch('\n');
}

int main() {
    bf::output = fopen("test_bool.bf", "w");
    ptr_t a = malloc(1), b = malloc(1), c = malloc(1);

    outstr("NOT:\n");
    test_not(a, 0, c);
    test_not(a, 1, c);

    outstr("AND:\n");
    test_and(a, 0, b, 0, c);
    test_and(a, 0, b, 1, c);
    test_and(a, 1, b, 0, c);
    test_and(a, 1, b, 1, c);

    outstr("OR:\n");
    test_or(a, 0, b, 0, c);
    test_or(a, 0, b, 1, c);
    test_or(a, 1, b, 0, c);
    test_or(a, 1, b, 1, c);

    outstr("XOR:\n");
    test_xor(a, 0, b, 0, c);
    test_xor(a, 0, b, 1, c);
    test_xor(a, 1, b, 0, c);
    test_xor(a, 1, b, 1, c);

    fclose(bf::output);
    return 0;
}

