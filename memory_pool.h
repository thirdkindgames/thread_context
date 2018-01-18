#pragma once

// General memory pool
struct MemoryPool
{
    virtual void* allocate( size_t size ) = 0;
    virtual void free( void* allocation ) = 0;
};

inline void* operator new( size_t size, MemoryPool* pool )
{
    return pool->allocate( size );
}

void operator delete( void* allocation, MemoryPool* pool )
{
    pool->free( allocation );
}