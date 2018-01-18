#pragma once

#include "memory_pool.h"

struct PageAllocator : public MemoryPool
{
    PageAllocator( size_t pageSize )
        : allocated( 0 )
        , data( nullptr )
    {
        data = new unsigned char[ pageSize ];
        if ( data )
        {
            capacity = pageSize;
        }
    }

    ~PageAllocator()
    {
        delete [] data;
    }

    void* allocate( size_t size ) override
    {
        void* result = nullptr;
        if ( ( capacity - allocated ) > size )
        {
            result = data + allocated;
            allocated += size;
        }
        return result;
    }
    
    void free( void* allocation ) override
    {
        assert( allocation >= data && allocation < ( data + capacity ) );   // Check not deleting from the wrong pool, not expecting to delete allocations from here
    }

private:
    size_t allocated;
    size_t capacity;
    unsigned char* data;
};