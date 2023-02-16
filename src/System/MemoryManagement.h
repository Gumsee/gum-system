#pragma once
#include <malloc.h>

namespace Gum
{
    /**
     * A safer alternative to delete
     */
    template<typename T>
    void _delete(T*& ptr)
    {
        if(ptr != nullptr)
            delete ptr;
        ptr = nullptr;
    }


    inline void* _malloc(size_t nbytes)
    {
        return malloc(nbytes);
    }

    inline void _free(void* ptr)
    {
        free(ptr);
    }
};