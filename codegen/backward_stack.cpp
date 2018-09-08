#include "util.h"
#include "backward_stack.h"
#include <cstdio>

using std::fputc;
using std::fputs;

namespace bf {

const int STACK_FRAME_SIZE = 4;
int stack_frame_size() {
    return STACK_FRAME_SIZE;
}

void init_stack() {
    for (ptr_t i = -2 * STACK_FRAME_SIZE; i < 0; i++) {
        zero(i);
    }

    assign(-2 * STACK_FRAME_SIZE, 1);
    assign(-STACK_FRAME_SIZE, -1);
}

void move_from_terminator_frame() {
    fputs("-[+", output);

    for (int i = 0; i < STACK_FRAME_SIZE; i++) {
        fputc('<', output);
    }

    fputs("-]+", output);
}

void move_to_terminator_frame() {
    fputs("+[-", output);

    for (int i = 0; i < STACK_FRAME_SIZE; i++) {
        fputc('>', output);
    }

    fputs("+]-", output);
}

void dup_frame() {
    move_to(-STACK_FRAME_SIZE);
    unsafe();
    move_from_terminator_frame();
    push_ptr_offset();
    safe();

    for (int i = 1; i < STACK_FRAME_SIZE; i++) {
        copy(i, i - STACK_FRAME_SIZE, -STACK_FRAME_SIZE);
    }

    zero(0);
    assign(-STACK_FRAME_SIZE, 1);
    unsafe();
    pop_ptr_offset();
    move_to_terminator_frame();
    safe();
}

void append_frame() {
    // TODO
}
}
