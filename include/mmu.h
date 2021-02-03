#pragma once

#include <common/types.h>

namespace myos
{

    struct Chunk
    {
        Chunk *next;
        Chunk *prev;
        bool allocated;
        common::size_t size;
    };

    class MMU
    {

    protected:
        Chunk *first;

    public:
        static MMU *activeMemoryManager;

        MMU(common::size_t first, common::size_t size);
        ~MMU();

        void *malloc(common::size_t size);
        void free(void *ptr);
    };
} // namespace myos