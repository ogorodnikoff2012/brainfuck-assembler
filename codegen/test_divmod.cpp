#include "util.h"
#include "instructions.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>

using namespace std;
using namespace bf;

int main(int argc, char *argv[]) {
    assert(argc > 2);
    bf::output = fopen("test_divmod.bf", "w");
    Environment env;
    int a = stoll(argv[1]), b = stoll(argv[2]);
    env.ra.assign(a);
    env.rb.assign(b);
    op_divmod(env);
    env.ra.println();
    env.rb.println();
    env.rc.println();
    fclose(bf::output);
    return 0;
}
