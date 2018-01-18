#pragma once

#include "thread_context.h" 

extern ThreadFunctionTable g_fiberJobThreadFunctionTable;

// Note: This is not an implementation of a fiber job system, just the bare bones to show how such a system would
// work with the thread context.

void Job_Sleep( unsigned long msTimeout );
bool Job_WaitOnMutex( Mutex* m, unsigned long msTimeout );