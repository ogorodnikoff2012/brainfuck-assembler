#ifndef __XENON_CODEGEN_BACKWARD_STACK_H__
#define __XENON_CODEGEN_BACKWARD_STACK_H__

#include "util.h"

namespace bf
{
int stack_frame_size();

void init_stack();

void dup_frame();
void append_frame();

void push_frame();
void pop_frame();

void select_deep_frame();
void reset_deep_frame();
void dup_deep_frame();
}

#endif
