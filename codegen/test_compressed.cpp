#include "util.h"
#include "integer.h"
#include "compressed_integer.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

int main() {
    bf::output = fopen("test_compressed.bf", "w");
    Integer<4> a;
    CompressedInteger<4> ca;
    assign(a.get_data(), 1);
    assign(a.get_data() + 1, 2);
    assign(a.get_data() + 2, 3);
    assign(a.get_data() + 3, 4);

    outstr("A = 0x");
    a.println();

    ca.pack(a);
    outstr("Packed\n");

    a.zero();
    outstr("A = 0x");
    a.println();

    ca.unpack(a);
    outstr("Unpacked\n");

    outstr("A = 0x");
    a.println();

    fclose(bf::output);
    return 0;
}

