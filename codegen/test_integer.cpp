#include "util.h"
#include "integer.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

int main() {
    bf::output = fopen("test_integer.bf", "w");
    Integer<4> a, b;
    assign(a.get_data(), 5);
    assign(b.get_data(), 15);

    outstr("A = 0x");
    a.println();

    outstr("B = 0x");
    b.println();

    a += b;
    outstr("A + B = 0x");
    a.println();

    outstr("A + 2 * B = 0x");
    a += b;
    a.println();

    outstr("============\n");

    a.zero();
    assign(a.get_data(), 3);
    outstr("A = 0x");
    a.println();

    assign(b.get_data(), 5);
    outstr("B = 0x");
    b.println();

    a *= b;
    outstr("A * B = 0x");
    a.println();

    fclose(bf::output);
    return 0;
}

