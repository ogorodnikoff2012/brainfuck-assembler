#include "util.h"
#include "integer.h"
#include "compressed_integer.h"
#include "byte_array.h"
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>

#include "instructions.h"

using namespace std;
using namespace bf;

int get_file_size(std::string filename) { // path to file
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(), "rb");
    fseek(p_file, 0, SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

void print_help(const string& prog_name) {
    fprintf(stderr, "Usage: %s [-o output] input\n", prog_name.c_str());
}

int vec_main(vector<string> &args) {
    int input_file = 1;
    string output_filename = "a.bf";

    if (args.size() < 2) {
        print_help(args[0]);
        return 1;
    }

    if (args[input_file] == "-o") {
        if (args.size() != 4) {
            print_help(args[0]);
            return 1;
        }
        output_filename = args[2];
        input_file = 3;
    } else {
        if (args.size() != 2) {
            print_help(args[0]);
            return 1;
        }
    }

    bf::output = fopen(output_filename.c_str(), "w");
    Environment env;
    /*
     Integer<4> ra, rb, rc, sp;
     CompressedInteger<4> ci_buf;
     ptr_t buf = malloc(16);
     ByteArray<4096, 4> data, code;
     */
    size_t filesize = get_file_size(args[input_file]);
    load_bytes_from_file(env.code, filesize, args[input_file]);

    vector<bool> used(256, false);
    FILE *f = fopen(args[input_file].c_str(), "rb");
    vector<unsigned char> buf(filesize);
    if (fread(&buf[0], sizeof(buf[0]), buf.size(), f) != filesize) {
        fprintf(stderr, "Error while reading %s! Aborting\n", args[input_file].c_str());
    }

    fprintf(stderr, "File size: %lu bytes\n", filesize);

    for (size_t i = 0; i < buf.size(); ++i) {
        used[buf[i]] = true;
    }

    fclose(f);


    used[NOP] = true;

    assign(env.buf, NOP);
    loop_begin(env.buf);
    {
        // DEBUG ONLY
        /*
         mul16(env.buf, env.buf + 1, env.buf + 2);
         div16(env.buf + 1, env.buf + 2, env.buf + 3);
         copy(env.buf + 2, env.buf + 1, env.buf + 3);
         bf::negate(env.buf + 2, env.buf + 3);
         add_and_erase(env.buf + 2, env.buf);
         div16(env.buf, env.buf + 2, env.buf + 3);
         outhex(env.buf + 2, env.buf + 3);
         outhex(env.buf + 1, env.buf + 3);
         add_and_erase(env.buf + 1, env.buf);
         assign(env.buf + 1, '\n');
         out(env.buf + 1);
         // */

        for (int i = 0; i < OPS_SIZE; i++) {
            if (used[i]) {
                // fprintf(stderr, "Opcode #%d\n", i);
                breakpoint();
                dec(env.buf, i);
                copy(env.buf, env.buf + 15, env.buf + 1);
                bool_cast(env.buf + 15, env.buf + 1);
                bool_negate(env.buf + 15, env.buf + 1);
                if_begin(env.buf + 15);
                {
                    OPS[i](env);
                }
                if_end(env.buf + 15);
                inc(env.buf, i);
            }
        }

        env.code.get();
        copy(env.code.get_byte(), env.buf, env.buf + 1);
        env.code.inc_addr_ptr();
    }
    loop_end(env.buf);

    fclose(bf::output);
    /*
     #define LOG_ADDR(x) fprintf(stderr, "%s: %d\n", #x, x);
     LOG_ADDR(env.ra.get_buf());
     LOG_ADDR(env.ra.get_data());
     LOG_ADDR(env.rb.get_buf());
     LOG_ADDR(env.rb.get_data());
     LOG_ADDR(env.rc.get_buf());
     LOG_ADDR(env.rc.get_data());
     LOG_ADDR(env.rd.get_buf());
     LOG_ADDR(env.rd.get_data());

     LOG_ADDR(env.ci_buf.get_data());
     LOG_ADDR(env.buf);

     LOG_ADDR(env.code.get_data());
     LOG_ADDR(env.data.get_data());
     LOG_ADDR(env.stack.get_data());
     #undef LOG_ADDR
     */
    return 0;
}

int main(int argc, char *argv[]) {
    vector<string> args(argc);

    for (int i = 0; i < argc; i++) {
        args[i] = string(argv[i]);
    }

    return vec_main(args);
}

