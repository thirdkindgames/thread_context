#include "system_memory_pool.h"

#include "mutex.h"

// Standard thread function table
bool StandardThread_LockMutex( Mutex* mutex, unsigned long msTimeout )
{
    return mutex->lock( msTimeout );
}

void StandardThread_Sleep( unsigned long msTimeout )
{
    Sleep( ( DWORD )msTimeout );
}

ThreadFunctionTable g_standardThreadFunctionTable = 
{
    &StandardThread_LockMutex,
    &StandardThread_Sleep,
};

ThreadContext::ThreadContext()
    : threadFunctions( g_standardThreadFunctionTable )
    , memoryPoolStackIndex( 0 )
{
    memoryPoolStack[ memoryPoolStackIndex ] = &g_systemMemoryPool;
}