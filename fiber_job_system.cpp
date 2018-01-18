#include "fiber_job_system.h"

#include "mutex.h"

// Job specific
bool FiberJobThread_LockMutex( Mutex* mutex, unsigned long msTimeout )
{
    bool result = mutex->lock( 0 ); // Try and get the mutex immediately
    if ( !result )
    {
        // Implementation specific but needs to switch the thread to another fiber if it cannot lock the mutex rather than waiting 
        // so other work can run until the mutex is available
        result = Job_WaitOnMutex( mutex, msTimeout );   
    }
    return false;
}

void FiberJobThread_Sleep( unsigned long msTimeout )
{
    // Yield the fiber for the provided time in milliseconds
    Job_Sleep( msTimeout );
}

ThreadFunctionTable g_fiberJobThreadFunctionTable = 
{
    &FiberJobThread_LockMutex,
    &FiberJobThread_Sleep,
};

// Dummy implementations of job work just to indicate different program flow
void Job_Sleep( unsigned long )
{
    OutputDebugString( "Yielding fiber" );
}

bool Job_WaitOnMutex( Mutex*, unsigned long )
{
    OutputDebugString( "Yielding fiber until mutex is available" );
    return false;
}