#pragma once

// Dummy implementation of a muxtex that fails to lock in the timeout
struct Mutex
{
    bool lock( unsigned long msTimeout )
    {
        return false;
    }
};