#ifndef __XENON_CODEGEN_TREE_BYTE_ARRAY_H__
#define __XENON_CODEGEN_TREE_BYTE_ARRAY_H__

#include "util.h"
#include "integer.h"

using std::fputs;

namespace bf
{
template<int DEPTH, int LEAF_SIZE>
class TreeByteNode {
private:
    ptr_t addr, io, buf, data;
    using Child=TreeByteNode<DEPTH - 1, LEAF_SIZE>;
public:
    TreeByteNode(ptr_t p = -1) {
        if (p < 0) {
            p = malloc(size());
        }
        addr = p;
        io = addr + addr_size();
        buf = io + 1;
        data = buf + buf_size();
    }

    static constexpr int chunk_metadata_size() {
        return 2;
    }

    static constexpr int size() {
        return addr_size() + 1 + buf_size() + data_size();
    }

    static constexpr int data_chunk_size() {
        return TreeByteNode<DEPTH - 1, LEAF_SIZE>::size() + chunk_metadata_size();
    }

    static constexpr int data_size() {
        return LEAF_SIZE * data_chunk_size();
    }

    static constexpr int addr_size() {
        return DEPTH;
    }

    static constexpr int buf_size() {
        return 2;
    }

    ptr_t get_io() const {
        return io;
    }
    ptr_t get_addr() const {
        return addr;
    }

    void get() const {
        for (int i = 0; i < LEAF_SIZE; ++i) {
            ptr_t chunkPtr = data + i * data_chunk_size();
            ptr_t childPtr = chunkPtr + chunk_metadata_size();
            assign(chunkPtr, 1);
            assign(chunkPtr + 1, 1);

            dec(addr, i);
            copy(addr, buf, buf + 1);
            bool_cast(buf, buf + 1);
            bool_negate(buf, buf + 1);
            if_begin(buf);
            {
                assign(chunkPtr, 0);
                Child child(childPtr);
                for (int j = 1; j < DEPTH; ++j) {
                    copy(addr + j, child.get_addr() + j - 1, buf + 1);
                }
            }
            if_end(buf);
            inc(addr, i);
        }

        assign(data + 1, 0);

        move_to(data);
        unsafe();
        unsafe_while_begin();
        unsafe_move_delta(data_chunk_size());
        unsafe_while_end();
        unsafe_move_delta(chunk_metadata_size());
        push_ptr_offset();
        safe();

        {
            Child child(0);
            child.get();
            move_to(0);
        }

        unsafe();
        pop_ptr_offset();
        unsafe_move_delta(-1);
        unsafe_while_begin();
        unsafe_move_delta(-data_chunk_size());
        unsafe_while_end();
        unsafe_move_delta(1 - chunk_metadata_size());
        safe();

        for (int i = 0; i < LEAF_SIZE; ++i) {
            ptr_t chunkPtr = data + i * data_chunk_size();
            ptr_t childPtr = chunkPtr + chunk_metadata_size();

            dec(addr, i);
            copy(addr, buf, buf + 1);
            bool_cast(buf, buf + 1);
            bool_negate(buf, buf + 1);
            if_begin(buf);
            {
                Child child(childPtr);
                copy(child.get_io(), io, buf + 1);
            }
            if_end(buf);
            inc(addr, i);
        }
    }

    void put() const {
        for (int i = 0; i < LEAF_SIZE; ++i) {
            ptr_t chunkPtr = data + i * data_chunk_size();
            ptr_t childPtr = chunkPtr + chunk_metadata_size();
            assign(chunkPtr, 1);
            assign(chunkPtr + 1, 1);

            dec(addr, i);
            copy(addr, buf, buf + 1);
            bool_cast(buf, buf + 1);
            bool_negate(buf, buf + 1);
            if_begin(buf);
            {
                assign(chunkPtr, 0);
                Child child(childPtr);
                for (int j = 1; j < DEPTH; ++j) {
                    copy(addr + j, child.get_addr() + j - 1, buf + 1);
                }
                copy(io, child.get_io(), buf + 1);
            }
            if_end(buf);
            inc(addr, i);
        }

        assign(data + 1, 0);

        move_to(data);
        unsafe();
        unsafe_while_begin();
        unsafe_move_delta(data_chunk_size());
        unsafe_while_end();
        unsafe_move_delta(chunk_metadata_size());
        push_ptr_offset();
        safe();

        {
            Child child(0);
            child.put();
            move_to(0);
        }

        unsafe();
        pop_ptr_offset();
        unsafe_move_delta(-1);
        unsafe_while_begin();
        unsafe_move_delta(-data_chunk_size());
        unsafe_while_end();
        unsafe_move_delta(1 - chunk_metadata_size());
        safe();
    }

    static ptr_t get_phys_offset(unsigned int virt_addr) {
        unsigned int child_index = virt_addr % LEAF_SIZE;
        unsigned int relative_addr = virt_addr / LEAF_SIZE;
        return addr_size() + 1 + buf_size() + child_index * data_chunk_size() + chunk_metadata_size() + Child::get_phys_offset(relative_addr);
    }
};

template <int LEAF_SIZE>
class TreeByteNode<1, LEAF_SIZE> {
private:
    ptr_t addr, io, buf, data;
public:
    static ptr_t get_phys_offset(unsigned int virt_addr) {
        unsigned int index = virt_addr % LEAF_SIZE;
        return addr_size() + 1 + buf_size() + index;
    }
    TreeByteNode(ptr_t p = -1) {
        if (p < 0) {
            p = malloc(size());
        }
        addr = p;
        io = addr + addr_size();
        buf = io + 1;
        data = buf + buf_size();
    }

    static constexpr int size() {
        return addr_size() + 1 + buf_size() + data_size();
    }

    static constexpr int data_size() {
        return LEAF_SIZE;
    }

    static constexpr int addr_size() {
        return 1;
    }

    static constexpr int buf_size() {
        return 2;
    }

    ptr_t get_io() const {
        return io;
    }

    ptr_t get_addr() const {
        return addr;
    }

    void get() const {
        for (int i = 0; i < data_size(); ++i) {
            dec(addr, i);
            copy(addr, buf, buf + 1);
            bool_cast(buf, buf + 1);
            bool_negate(buf, buf + 1);
            if_begin(buf);
            {
                copy(data + i, io, buf + 1);
            }
            if_end(buf);
            inc(addr, i);
        }
    }

    void put() const {
        for (int i = 0; i < data_size(); ++i) {
            dec(addr, i);
            copy(addr, buf, buf + 1);
            bool_cast(buf, buf + 1);
            bool_negate(buf, buf + 1);
            if_begin(buf);
            {
                copy(io, data + i, buf + 1);
            }
            if_end(buf);
            inc(addr, i);
        }
    }
};

template<int DEPTH>
class TreeByteArray
{
private:
    ptr_t addr_register_ptr, data_ptr;
    using Data = TreeByteNode<DEPTH, 16>;
    using Addr = Integer<DEPTH>;
    Data* data;
    Addr* addr;
public:
    TreeByteArray(ptr_t p = -1) {
        if (p < 0) {
            p = malloc(size());
        }
        addr_register_ptr = p;
        data_ptr = addr_register_ptr + Integer<DEPTH>::size();
        addr = new Addr(addr_register_ptr);
        data = new Data(data_ptr);
    }

    ~TreeByteArray() {
        delete addr;
        delete data;
    }

    constexpr static int size() {
        return Integer<DEPTH>::size() + TreeByteNode<DEPTH, 16>::size();
    }

    constexpr static int buf_size() {
        return Integer<DEPTH>::buf_size();
    }

    ptr_t get_byte() const {
        return data->get_io();
    }

    ptr_t get_buf() const {
        return addr->get_buf();
    }

    void get() const {
        data->get();
    }

    void put() const {
        data->put();
    }

    void update_addr_ptr() const {
        for (int i = 0; i < DEPTH; ++i) {
            copy(addr->get_data() + i, data->get_addr() + i);
        }
    }

    void inc_addr_ptr() const {
        addr->add_const(1LL);
        update_addr_ptr();
    }

    void dec_addr_ptr() const {
        addr->add_const(-1LL);
        update_addr_ptr();
    }

    Integer<DEPTH>* get_addr_register() const {
        return addr;
    }

    ptr_t get_phys_addr(unsigned int virt_addr) const {
        return data_ptr + Data::get_phys_offset(virt_addr);
    }
};
}

#endif // __XENON_CODEGEN_TREE_BYTE_ARRAY_H__
