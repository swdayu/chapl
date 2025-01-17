#ifndef CHAPL_CHCC_GABI_H
#define CHAPL_CHCC_GABI_H
#include "chcc/chcc.h"

// 目标文件和共享文件中全局符号的地址就是它所在文件的文件偏移，当然这个地址不是有效地址，
// 不能真正写到引用该符号的位置。当在目标文件和共享文件中生成对这些全局符号的引用时，每
// 个引用的符号都需要创建一个重定位条目，并且引用的位置不会填入符号的地址，只是串连起对
// 这个符号的引用链条。当目标文件和共享文件链接到可执行文件时，这些符号才会得到自己真正
// 的内存虚拟地址，这时才将符号的真实地址填入到被引用的位置。当目标文件和共享文件引用一
// 个外部符号时，在其重定位条目中，其符号的索引值会设为未定义。
//
// 当可执行文件静态链接一个文件时，读取文件的重定位条目，对于定义的符号直接进行重定位，
// 对于未定义的符号，查找是否已经被可执行文件链接了，如果是也直接进行重定位，如果暂时还
// 没有链接到这个符号，则要等可执行文件链接完所有文件后，再最后进行一次重定位，如果此时
// 还有没找到的静态链接符号，需要报错。如果找到的符号是可执行文件动态链接过来的，这个符
// 号需要按下面的动态链接符号的重定位方式处理。
//
// 当可执行文件动态链接一个文件时，

void g(chcc_t *cc, uint32 n);
void gi(chcc_t *cc, uint32 imm32);
byte *ga(chcc_t *cc, uint32 n, byte *addr);
byte *gjmp(chcc_t *cc, byte *addr);
byte *gjcc(chcc_t *cc, bool jmp_when_true, byte *addr);
byte *gjz(chcc_t *cc, byte *addr);
byte *gjnz(chcc_t *cc, byte *addr);

void grel(byte *cur, int96 *usel);
uint32 genter(chcc_t *cc, fsym_t *f);
void gret(chcc_t *cc, fsym_t *f);

#endif /* CHAPL_CHCC_GABI_H */
