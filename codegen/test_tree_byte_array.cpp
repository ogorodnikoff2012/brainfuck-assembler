#include "util.h"
#include "integer.h"
#include "tree_byte_array.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

#define N 4

int main() {
    bf::output = fopen("test_tree_byte_array.bf", "w");
    Integer<N> a;
    TreeByteArray<4> ba;

    for (int i = 0; i < N; i++) {
        assign(a.get_data() + i, i + 1);
    }

    outstr("A = 0x");
    a.println();

    ba.get_addr_register()->zero();
    ba.update_addr_ptr();

    for (int i = 0; i < N; i++) {
        copy(a.get_data() + i, ba.get_byte(), a.get_buf());
        ba.put();
        ba.inc_addr_ptr();
    }

    outstr("Packed\n");

    a.zero();
    outstr("A = 0x");
    a.println();

    ba.get_addr_register()->zero();
    ba.update_addr_ptr();

    for (int i = 0; i < N; i++) {
        ba.get();
        copy(ba.get_byte(), a.get_data() + i, ba.get_buf());
        ba.inc_addr_ptr();
    }

    outstr("Unpacked\n");

    outstr("A = 0x");
    a.println();

    fclose(bf::output);
    return 0;
}

