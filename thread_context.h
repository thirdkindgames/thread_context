#pragma once

#include "memory_pool.h"

struct Mutex;

struct ThreadFunctionTable
{
    bool (*LockMutex)( Mutex* /*mutex*/, DWORD /*msTimeout*/ );
    void (*Sleep)( DWORD /*ms*/ );
};

struct ThreadContext
{    
    ThreadContext();

    void popMemoryPool(MemoryPool* pool)
    {
        assert( memoryPoolStackIndex > 0 ); // Can't pop the default pool
        if ( memoryPoolStackIndex > 0 ) 
        {
            assert( pool == memoryPoolStack[ memoryPoolStackIndex ] ); // Popping a pool that is not the current top of the stack
            --memoryPoolStackIndex;
        }
    }

    void pushMemoryPool( MemoryPool* pool )
    {
        assert( ( memoryPoolStackIndex + 1 ) < c_maxMemoryPoolStackSize );  // Out of space in the stack
        if ( ( memoryPoolStackIndex + 1 ) < c_maxMemoryPoolStackSize )
        {
            memoryPoolStack[ ++memoryPoolStackIndex ] = pool;
        }
    }

    static size_t const c_maxMemoryPoolStackSize = 16;

    ThreadFunctionTable threadFunctions;
    MemoryPool* memoryPoolStack[ c_maxMemoryPoolStackSize ];
    size_t memoryPoolStackIndex;
};
__declspec(thread) ThreadContext g_threadContext;

// Helper functions to wrap usage of the thread context
__forceinline void ThreadPopMemoryPool( MemoryPool* pool )
{
    g_threadContext.popMemoryPool( pool );
}

__forceinline void ThreadPushMemoryPool( MemoryPool* pool )
{
    g_threadContext.pushMemoryPool( pool );
}

__forceinline void ThreadLockMutex( Mutex* mutex, unsigned long msTimeout )
{
    g_threadContext.threadFunctions.LockMutex( mutex, msTimeout );
}

__forceinline void ThreadSleep( unsigned long msTimeout )
{
    g_threadContext.threadFunctions.Sleep( msTimeout );
}

// Overload defaule new/delete to use the thread context
void* operator new( size_t size )
{
    return g_threadContext.memoryPoolStack[ g_threadContext.memoryPoolStackIndex ]->allocate( size );
}

void operator delete( void* allocation )
{
    return g_threadContext.memoryPoolStack[ g_threadContext.memoryPoolStackIndex ]->free( allocation );
}