#ifndef magic_lang_solo_h
#define magic_lang_solo_h
#include "prelude.h"
#include "coro.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *impl;
    void *a[7];
} solo_struct;

void solo_create(solo_struct *main, coroproc proc, int stack_size, void *userdata);
void solo_recreate(solo_struct *main, coroproc proc, void *userdata);
bool solo_call(solo_struct *main);
void solo_finish(solo_struct *main);

#ifdef __cplusplus
}
#endif
#endif /* magic_lang_solo_h */
