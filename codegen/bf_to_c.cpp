#include <cstdio>
#include <cstring>

using namespace std;

void indent(int indent_level) {
    while (indent_level--) {
        printf("    ");
    }
}

bool debug = false;

bool plus_minus(int c) {
    return c == '+' || c == '-';
}

bool less_greater(int c) {
    return c == '<' || c == '>';
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        }
    }

    printf("#include <stdio.h>\n"
           "\n"
           "#define SIZE (1 << 20) /* 1 MiB */\n"
           "char mem[SIZE];\n"
           "int idx = 0;\n"
           "\n"
           "void dump(void) {\n"
           "    int i;\n"
           "    fprintf(stderr, \"\\033[32;40m\");\n"
           "    for (i = -16; i < 16; i++) {\n"
           "        fprintf(stderr, \"%%4d\", idx + i);\n"
           "    }\n"
           "    fprintf(stderr, \"\\033[39;49m\");\n"
           "    fprintf(stderr, \"\\n\");\n"
           "    for (i = -16; i < 16; i++) {\n"
           "        fprintf(stderr, \"%%4hhX\", mem[(idx + i) & (SIZE - 1)]);\n"
           "    }\n"
           "    fprintf(stderr, \"\\n\");\n"
           "}\n"
           "\n"
           "void dump_stderr(void) {\n"
           "    int i;\n"
           "    for (i = 0; i < 8 * 12; i++) {\n"
           "        fputc(mem[i], stderr);\n"
           "    }\n"
           "}\n"
           "\n"
           "void put_and_flush(char c) {\n"
           "    fputc(c, stdout);\n"
           "    fflush(stdout);\n"
           "}\n"
           "\n"
           "int main(void) {\n");

    int ch = 0, prev_ch = 0, delta = 0;
    int indent_level = 1;

    do {
        ch = fgetc(stdin);

        if (plus_minus(prev_ch) && !plus_minus(ch)) {
            indent(indent_level);
            printf("mem[idx] += %d;\n", delta);
            delta = 0;
        }

        if (less_greater(prev_ch) && !less_greater(ch)) {
            indent(indent_level);
            printf("idx += %d;\n", delta);
            delta = 0;
            indent(indent_level);
            printf("idx &= SIZE - 1;\n");
        }

        if (ch == '+' || ch == '>') {
            delta++;
        }

        if (ch == '-' || ch == '<') {
            delta--;
        }

        if (ch == '[') {
            indent(indent_level);
            printf("while (mem[idx])\n");
            indent(indent_level);
            printf("{\n");
            indent_level++;
        }

        if (ch == ']') {
            indent_level--;
            indent(indent_level);
            printf("}\n");
        }

        if (ch == ',') {
            indent(indent_level);
            printf("mem[idx] = fgetc(stdin);\n");
        }

        if (ch == '.') {
            indent(indent_level);
            printf("fputc(mem[idx], stdout);\n");
        }

        if (ch == 'B') {
            indent(indent_level);
            printf("/* Breakpoint */\n");
        }

        if (ch == '!' && debug) {
            indent(indent_level);
            printf("dump();\n");
        }

        if (ch == '#' && debug) {
            indent(indent_level);
            printf("dump_stderr();\n");
        }

        prev_ch = ch;
    } while (ch != EOF);

    printf("    return 0;\n}\n");
    return 0;
}
