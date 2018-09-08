#include "util.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

int main(int argc, char *argv[]) {
    bf::output = fopen("test_is_negative.bf", "w");
    ptr_t a = malloc(1), b = malloc(1);
    assert(argc > 1);
    int a_val = atoi(argv[1]);
    assign(a, a_val);
    is_negative(a, b);
    outbool(b);
    outch('\n');
    fclose(bf::output);
    return 0;
}

