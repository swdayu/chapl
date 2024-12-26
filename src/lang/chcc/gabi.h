#ifndef CHAPL_CHCC_GABI_H
#define CHAPL_CHCC_GABI_H
#include "chcc/chcc.h"

void g(chcc_t *cc, uint32 n);
void gi(chcc_t *cc, uint32 imm32);
byte *ga(chcc_t *cc, uint32 n, byte *addr);
byte *gjmp(chcc_t *cc, byte *addr);
byte *gjcc(chcc_t *cc, bool jmp_when_true, byte *addr);
byte *gjz(chcc_t *cc, byte *addr);
byte *gjnz(chcc_t *cc, byte *addr);

void grel(byte *cur, intv_t *usel);
uint32 genter(chcc_t *cc, fsym_t *f);
void gret(chcc_t *cc, fsym_t *f);

#endif /* CHAPL_CHCC_GABI_H */
