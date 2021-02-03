#include <mmu.h>

using namespace myos;
using namespace myos::common;

MMU *MMU::activeMemoryManager = 0;

MMU::MMU(size_t start, size_t size)
{
    activeMemoryManager = this;
    if (size < sizeof(Chunk))
        first = 0;
    else
    {
        first = (Chunk *)start;
        first->allocated = false;
        first->prev = 0;
        first->next = 0;
        first->size = size - sizeof(Chunk);
    }
}
MMU::~MMU()
{
    if (activeMemoryManager == this)
        activeMemoryManager = 0;
}

void *MMU::malloc(common::size_t size)
{
    Chunk *result = 0;
    for (auto chunk = first; chunk != 0 && !result; chunk = chunk->next)
        if (chunk->size > size && !chunk->allocated)
            result = chunk;

    if (!result)
        return 0;

    if (result->size >= size + sizeof(Chunk) + 1)

    {
        auto temp = (Chunk *)((size_t)result + sizeof(Chunk) + size);
        temp->allocated = false;
        temp->size = result->size - size - sizeof(Chunk);
        temp->prev = result;
        temp->next = result->next;
        if (temp->next != 0)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;
    }

    result->allocated = true;

    return (void *)(((size_t)result) + sizeof(Chunk));
}

void MMU::free(void *ptr)
{
    auto *chunk = (Chunk *)((size_t)ptr - sizeof(Chunk));

    chunk->allocated = false;

    if (chunk->prev != 0 && !chunk->prev->allocated)
    {
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(Chunk);
        if (chunk->next != 0)
            chunk->next->prev = chunk->prev;

        chunk = chunk->prev;
    }

    if (chunk->next != 0 && !chunk->next->allocated)
    {
        chunk->size += chunk->next->size + sizeof(Chunk);
        chunk->next = chunk->next->next;
        if (chunk->next != 0)
            chunk->next->prev = chunk;
    }
}
