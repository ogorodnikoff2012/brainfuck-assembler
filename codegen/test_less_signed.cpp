#include "util.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using std::fopen;
using std::fclose;

using namespace bf;

int main(int argc, char *argv[]) {
    bf::output = fopen("test_less_signed.bf", "w");
    ptr_t a = malloc(1), b = malloc(1);
    zero(a);
    zero(b);
    assert(argc > 2);
    int a_val = atoi(argv[1]);
    int b_val = atoi(argv[2]);
    add_const(a, a_val);
    add_const(b, b_val);
    ptr_t c = malloc(1);

    /*
     is_negative(a, c);
     outstr("A < 0: ");
     inc(c, '0');
     out(c);
     outch('\n');

     is_negative(b, c);
     outstr("B < 0: ");
     inc(c, '0');
     out(c);
     outch('\n');

     less_1bit(a, b, c);
     outstr("A1 < B1: ");
     inc(c, '0');
     out(c);
     outch('\n');

     less_1bit(b, a, c);
     outstr("B1 < A1: ");
     inc(c, '0');
     out(c);
     outch('\n');

     less_7bit(a, b, c);
     outstr("A7 < B7: ");
     inc(c, '0');
     out(c);
     outch('\n');
     */
    less_signed(a, b, c);
    outstr("less_signed: ");
    inc(c, '0');
    out(c);
    outch('\n');

    less_unsigned(a, b, c);
    outstr("less_unsigned: ");
    inc(c, '0');
    out(c);
    outch('\n');

    fclose(bf::output);
    return 0;
}
