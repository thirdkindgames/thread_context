#include <assert.h>
#include <stack>
#include <windows.h>

#include "fiber_job_system.h"
#include "memory_pool.h"
#include "page_allocator.h"
#include "system_memory_pool.h"
#include "thread_context.h"

// Program
void main()
{
    // Mem Test
    char* x = new char[ 512 ];                          // Allocates using the default allocator (the system allocator)

    auto pageAllocator = new PageAllocator( 2048 );     // Allocator that doesn't free, just delete the whole allocator when done
    
    ThreadPushMemoryPool( pageAllocator );
    char * y = new char[ 256 ];                         // This is allocated from the pagePool as can be seen by identitying the allocated value    
    int * z = new( &g_systemMemoryPool )int;            // Ignores the 'current' pool and uses the supplied system allocator instead    
    ThreadPopMemoryPool( pageAllocator );

    delete x;                                           // Deletes from the default allocator (the system allocator)

    // Thread Function Table - Uncommenting the next line will use the job thread function table instead so Sleep will call the fiberSleep function instead
    //g_threadContext.threadFunctions = g_fiberJobThreadFunctionTable;      // This line should be called on the job thread immediately after creation
    
    ThreadSleep( 1000 );                                // This call will end up in either StandardThread_Sleep or FiberJobThread_Sleep depending on which function table this thread is using

    delete pageAllocator;
}

// Externals and other CUs to make building simpler
SystemMemoryPool g_systemMemoryPool;

#include "fiber_job_system.cpp"
#include "thread_context.cpp"