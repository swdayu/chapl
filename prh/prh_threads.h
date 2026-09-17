// prh_threads.h - v0.01 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

#undef PRH_IMPL_WIN32_THREADS
#undef PRH_IMPL_POSIX_THREADS
#undef PRH_IMPL_SDL3_THREADS

#if defined(PRH_WIN32_THREADS)
#define PRH_IMPL_WIN32_THREADS
#elif defined(PRH_POSIX_THREADS)
#define PRH_IMPL_POSIX_THREADS
#elif defined(PRH_SDL3_THREADS)
#define PRH_IMPL_SDL3_THREADS
#endif

#if defined(PRH_IMPL_SDL3_THREADS)
#ifndef PRH_IMPL_THREADS_INCLUDED_H
#define PRH_IMPL_THREADS_INCLUDED_H

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   INTERFACE
////
////

#include "prh_include.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // PRH_IMPL_THREADS_INCLUDED_H

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   IMPLEMENTATION
////
////

#if defined(PRH_THREADS_IMPLEMENTATION)
#include <SDL3/SDL_thread.h>

static int SDLCALL prh_impl_sdl_thread_function(void *data)
{

}

void *prh_impl_sdl_create_thread(void *userdata, prh_reg stacksize, prh_reg thread_extra_size)
{
    SDL_PropertiesID properties = SDL_CreateProperties();
    prh_assert(properties != 0);

    prh_boolret(SDL_SetPointerProperty(properties, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, prh_impl_sdl_thread_function));
    prh_boolret(SDL_SetPointerProperty(properties, SDL_PROP_THREAD_CREATE_USERDATA_POINTER, userdata));
    prh_boolret(SDL_SetNumberProperty(properties, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, stacksize));

    void *thread = SDL_CreateThreadWithProperties(properties);
    prh_assert(thread != prh_null);

    SDL_DestroyProperties(properties);
    return thread;
}

// void SDL_WaitThread(SDL_Thread *thread, int *status);
//
// 等待线程结束。参数 status 填入线程函数返回的值，如果线程已分离或无效，则为 -1。可为 NULL。分离的
// 线程会一直保持，直到此函数清理它们。不这样做会造成资源泄漏。一旦线程通过此函数被清理，引用它的
// SDL_Thread 即告失效，不应再被引用。因此，只有一个线程可以对另一个线程调用 SDL_WaitThread()。
//
// 你不能对已在 SDL_DetachThread() 调用中使用过的线程调用 SDL_WaitThread()。只能二选一使用这两个函数，
// 两者都用则行为未定义。注意，线程指针由此函数释放，之后不再有效。线程安全：从任何线程调用此函数都
// 是安全的，但只能有一个线程等待某个特定线程结束。
//
// void SDL_DetachThread(SDL_Thread *thread);
//
// 让线程在退出时自行清理，无需干预。无法回收分离线程的返回码。如果需要返回码，不要分离线程，改用
// SDL_WaitThread()。给此函数传 NULL 是安全的。线程安全：从任何线程调用此函数都是安全的。
//
// 线程可以被"分离（detach）"，以表示它不应一直等到另一个线程对其调用 SDL_WaitThread()。分离线程对
// 长时间运行、不需要与之同步或进一步管理的线程很有用。分离的线程结束时，它就自行消失了。线程一旦被
// 分离，通常应假定 SDL_Thread 不再适合被引用，因为它会在分离的线程退出时立即失效，而不是保持到有人
// 调用 SDL_WaitThread() 最终清理它。因此，不要对同一线程分离超过一次。
//
// 如果线程在传给 SDL_DetachThread() 时已经退出，它将不再等待 SDL_WaitThread() 的调用并立即清理。分
// 离一个可能要调用 SDL_WaitThread() 的线程是不安全的。你不能对已分离的线程调用 SDL_WaitThread()。
// 只能二选一使用这两个函数，两者都用则行为未定义。

int prh_impl_sdl_join_thread(void *thread)
{
    int exit_code = 0;
    SDL_WaitThread((SDL_Thread *)thread, &exit_code);
    return exit_code;
}

#endif // PRH_THREADS_IMPLEMENTATION
#endif // PRH_IMPL_SDL3_THREADS

// FULL VERSION HISTORY
//
//   0.01 (2026-09-17) initial release for basic code
//

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Godelder Brother
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/