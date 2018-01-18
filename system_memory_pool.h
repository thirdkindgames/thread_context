#pragma once

#include "memory_pool.h"

struct SystemMemoryPool : public MemoryPool
{
    void* allocate( size_t size ) override
    {
        return VirtualAlloc( nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
    }
    
    void free( void* allocation ) override
    {
        VirtualFree( allocation, 0, MEM_RELEASE );
    }
};
extern SystemMemoryPool g_systemMemoryPool;