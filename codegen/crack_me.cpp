#include "util.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

int main(int argc, char *argv[]) {
    bf::output = fopen("crack_me.bf", "w");
    ptr_t a = malloc(1);
    assign(a, 0);
    loadstr("FLAG: YOUR_BRAIN_ISN'T_FUCKABLE\n");
    outstr("Enter password: ");
    ptr_t c = malloc(1);
    ptr_t b = malloc(1);
    assign(a, 1);
    loop_begin(a);
    {
        in(a);
        is_negative(a, c);
        dec(a, '\n');
        bool_cast(a);
        bool_negate(a);
        bool_or(a, c, b);
        copy_and_erase(b, 1, a);
    }
    loop_end(a);
    outstr("Sorry, your password is incorrect.\n");
    fclose(bf::output);
    return 0;
}

