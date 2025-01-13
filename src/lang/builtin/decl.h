#ifndef CHAPL_BUILTIN_DECL_H
#define CHAPL_BUILTIN_DECL_H
#include "direct/base.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#ifdef __cplusplus
extern "C" {
#endif
// __ARCH_X86__         // x86-pc
// __ARCH_X64__         // x64-pc
// __ARCH_ARM__         // arm, ARMv4
// __ARCH_ARM64__       // aarch64, ARMv8
// __ARCH_32BIT__       // 32-bit arch
// __ARCH_64BIT__       // 64-bit arch
// __ARCH_BITS__        // arch bits
// __LIT_ENDIAN__       // arch little endian
// __BIG_ENDIAN__       // arch big endian
// __OS_FREEBSD__       // freebsd
// __OS_OPENBSD__       // openbsd
// __OS_NETBSD__        // netbsd
// __OS_WINDOWS__       // windows
// __OS_LINUX__         // linux-gnu
// __OS_MACHO__         // apple-darwin
// __MSC__
// __GCC__
#include "builtin/type.h"

#define CHAR_NULL           '\0'    // 00
#define CHAR_AUDI_BELL      '\a'    // 07
#define CHAR_BACKSPACE      '\b'    // 08
#define CHAR_TAB            '\t'    // 09   horizontal tabulation (ht)
#define CHAR_NEWLINE        '\n'    // 0A   line feed (lf), new line (nl), end of line (eol)
#define CHAR_VT             '\v'    // 0B   vertical tabulation (vt)
#define CHAR_FORMFEED       '\f'    // 0C   form feed (ff), new page
#define CHAR_RETURN         '\r'    // 0D   carriage return (cr)
#define CHAR_ESC            '\x1b'  // 1B   escape
#define CHAR_SPACE          ' '     // 20
#define CHAR_EMARK          '!'     // 21
#define CHAR_NOT            '!'     // 21   逻辑非（一元操作）
#define CHAR_DQUOTE         '\"'    // 22
#define CHAR_HASH           '#'     // 23
#define CHAR_DOLLAR         '$'     // 24
#define CHAR_PERCENT        '%'     // 25
#define CHAR_AMPERSAND      '&'     // 26
#define CHAR_AND            '&'     // 26   位与
#define CHAR_SQUOTE         '\''    // 27
#define CHAR_OPEN_PAREN     '('     // 28
#define CHAR_CLOSE_PAREN    ')'     // 29
#define CHAR_ASTER          '*'     // 2A
#define CHAR_PLUS           '+'     // 2B
#define CHAR_ADD            '+'     // 2B   加法
#define CHAR_COMMA          ','     // 2C
#define CHAR_MINUS          '-'     // 2D
#define CHAR_SUB            '-'     // 2D   减法
#define CHAR_DOT            '.'     // 2E   成员选择或导入所有标识符
#define CHAR_SLASH          '/'     // 2F
#define CHAR_DIGIT_0        '0'     // 30
#define CHAR_DIGIT_9        '9'     // 39
#define CHAR_COLON          ':'     // 3A   标签、复合赋值、case/default、切片
#define CHAR_SEMICOLON      ';'     // 3B
#define CHAR_LT             '<'     // 3C   小于
#define CHAR_EQUAL          '='     // 3D
#define CHAR_ASSIGN         '='     // 3D   赋值
#define CHAR_GT             '>'     // 3E   大于
#define CHAR_QMARK          '?'     // 3F
#define CHAR_ATSIGN         '@'     // 40
#define CHAR_ATTRIBUTE      '@'     // 40   属性
#define CHAR_UPPER_A        'A'     // 41
#define CHAR_UPPER_Z        'Z'     // 5A
#define CHAR_OPEN_SQUARE    '['     // 5B
#define CHAR_BSLASH         '\\'    // 5C
#define CHAR_CLOSE_SQUARE   ']'     // 5D
#define CHAR_CARET          '^'     // 5E
#define CHAR_XOR            '^'     // 5E   异或
#define CHAR_UNDERSCORE     '_'     // 5F
#define CHAR_BQUOTE         '`'     // 60
#define CHAR_LOWER_A        'a'     // 61
#define CHAR_LOWER_Z        'z'     // 7A
#define CHAR_OPEN_CURLY     '{'     // 7B
#define CHAR_VERTBAR        '|'     // 7C
#define CHAR_BOR            '|'     // 7C   位或
#define CHAR_CLOSE_CURLY    '}'     // 7D
#define CHAR_TILDE          '~'     // 7E   连接泛型实参（类型实参）
#define CHAR_DEL            '\x7f'  // 7F   delete
#define CHAR_INVALID_UTF    0xFFFD  // replacement character
#define CHAR_MAX_UNICODE    0x0010FFFF
#define CHAR_EOF            0xFFFFFFFF
#define CHAR_BIN_DIGIT      '1'
#define CHAR_DEC_DIGIT      '9'
#define CHAR_HEX_DIGIT      'f'

#define X_X_NAME(a, b) X_NAME(a, b)
#define X_X_MSTR(a) X_MSTR(a)
#define X_NAME(a, b) a##b
#define X_MSTR(a) #a

#define LOG_LEVEL_F 0x00000000 // fatal panic
#define LOG_LEVEL_M 0x01000000 // main
#define LOG_LEVEL_E 0x02000000 // error
#define LOG_LEVEL_I 0x03000000 // information
#define LOG_LEVEL_D 0x04000000 // only for debug, not in release

#undef lang_assert
#undef debug_assert

#ifdef CONFIG_DEBUG
#define debug_assert(e) lang_assert(e)
#else
#define debug_assert(e)
#endif

#define lang_assert(e) ((e) ? ((void)0) : assertfault_(__CURR_FILE__, __LINE__))
#define lang_assert_x(e, n, ...) ((e) ? ((void)0) : assertfaultx_(__CURR_FILE__, X_ARGN_LINE(n), ## __VA_ARGS__))
#define lang_assert_s_x(e, s, n, ...) ((e) ? ((void)0) : assertfaults_(__CURR_FILE__, X_ARGN_LINE(n), (s), ## __VA_ARGS__))
#define lang_assert_1(e, a) lang_assert_x((e), 1, (uint32)(a))
#define lang_assert_2(e, a, b) lang_assert_x((e), 2, (uint32)(a), (uint32)(b))
#define lang_assert_3(e, a, b, c) lang_assert_x((e), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define lang_assert_4(e, a, b, c, d) lang_assert_x((e), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define lang_assert_5(e, a, b, c, d, ee) lang_assert_x((e), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(ee))
#define lang_assert_s(e, s) lang_assert_s_x((e), (s), 0)
#define lang_assert_s_1(e, s, a) lang_assert_s_x((e), (s), 1, (uint32)(a))
#define lang_assert_s_2(e, s, a, b) lang_assert_s_x((e), (s), 2, (uint32)(a), (uint32)(b))
#define lang_assert_s_3(e, s, a, b, c) lang_assert_s_x((e), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define lang_assert_s_4(e, s, a, b, c, d) lang_assert_s_x((e), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define lang_assert_s_5(e, s, a, b, c, d, ee) lang_assert_s_x((e), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(ee))

void assertfault_(uint16 file, uint32 line);
void assertfaultx_(uint16 file, uint32 argn_line, ...);
void logtrace_(errot err, uint32 file_err, uint32 line);
void logtracex_(errot err, uint32 file_err, uint32 argn_line, ...);

#define X_FILE(err) (__CURR_FILE__ << 16)
#define X_ARGN_LINE(argn) (((argn) << 27) | __LINE__)
#define X_LINE(level, argn) (((argn) << 27) | (level) | __LINE__)

#define LOG_FILE(file) ((strid_t)((file) >> 16))
#define LOG_ARGN(line) ((line) >> 27)
#define LOG_LEVEL(line) (((line) >> 24) & 0x7)
#define LOG_LINE(line) ((line) & 0x00FFFFFF)
#define LOG_CHAR(line) "FmEID"[LOG_LEVEL(line)]

#define log_fatal(err) logtrace_((err), X_FILE(err), LOG_LEVEL_F|__LINE__)
#define log_fatal_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_F, (n)), __VA_ARGS__)
#define log_fatal_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_F, (n)), (s), ## __VA_ARGS__)

#define __LOG_LEVEL__ X_X_NAME(__CURR_FILE__, _LOG_LEVEL)

#if (__LOG_LEVEL__ == 'M' || __LOG_LEVEL__ == 'E' || __LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_main(err) logtrace_((err), X_FILE(err), LOG_LEVEL_M|__LINE__)
#define log_main_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_M, (n)), __VA_ARGS__)
#define log_main_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_M, (n)), (s), ## __VA_ARGS__)
#else
#define log_main(err)
#define log_main_x(err, n, ...)
#define log_main_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'E' || __LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_error(err) logtrace_((err), X_FILE(err), LOG_LEVEL_E|__LINE__)
#define log_error_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_E, (n)), __VA_ARGS__)
#define log_error_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_E, (n)), (s), ## __VA_ARGS__)
#else
#define log_error(err)
#define log_error_x(err, n, ...)
#define log_error_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'I' || __LOG_LEVEL__ == 'D')
#define log_info(err) logtrace_((err), X_FILE(err), LOG_LEVEL_I|__LINE__)
#define log_info_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_I, (n)), __VA_ARGS__)
#define log_info_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_I, (n)), (s), ## __VA_ARGS__)
#else
#define log_info(err)
#define log_info_x(err, n, ...)
#define log_info_s_x(err, s, n, ...)
#endif

#if (__LOG_LEVEL__ == 'D') && defined(CONFIG_DEBUG)
#define log_debug(err) logtrace_((err), X_FILE(err), LOG_LEVEL_D|__LINE__)
#define log_debug_x(err, n, ...) logtracex_((err), X_FILE(err), X_LINE(LOG_LEVEL_D, (n)), __VA_ARGS__)
#define log_debug_s_x(err, s, n, ...) logtraces_((err), X_FILE(err), X_LINE(LOG_LEVEL_D, (n)), (s), ## __VA_ARGS__)
#else
#define log_debug(err)
#define log_debug_x(err, n, ...)
#define log_debug_s_x(err, s, n, ...)
#endif

#define log_fatal_1(err, a) log_fatal_x((err), 1, (uint32)(a))
#define log_fatal_2(err, a, b) log_fatal_x((err), 2, (uint32)(a), (uint32)(b))
#define log_fatal_3(err, a, b, c) log_fatal_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_fatal_4(err, a, b, c, d) log_fatal_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_fatal_5(err, a, b, c, d, e) log_fatal_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_fatal_s(err, s) log_fatal_s_x((err), (s), 0)
#define log_fatal_s_1(err, s, a) log_fatal_s_x((err), (s), 1, (uint32)(a))
#define log_fatal_s_2(err, s, a, b) log_fatal_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_fatal_s_3(err, s, a, b, c) log_fatal_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_fatal_s_4(err, s, a, b, c, d) log_fatal_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_fatal_s_5(err, s, a, b, c, d, e) log_fatal_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_main_1(err, a) log_main_x((err), 1, (uint32)(a))
#define log_main_2(err, a, b) log_main_x((err), 2, (uint32)(a), (uint32)(b))
#define log_main_3(err, a, b, c) log_main_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_main_4(err, a, b, c, d) log_main_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_main_5(err, a, b, c, d, e) log_main_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_main_s(err, s) log_main_s_x((err), (s), 0)
#define log_main_s_1(err, s, a) log_main_s_x((err), (s), 1, (uint32)(a))
#define log_main_s_2(err, s, a, b) log_main_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_main_s_3(err, s, a, b, c) log_main_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_main_s_4(err, s, a, b, c, d) log_main_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_main_s_5(err, s, a, b, c, d, e) log_main_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_error_1(err, a) log_error_x((err), 1, (uint32)(a))
#define log_error_2(err, a, b) log_error_x((err), 2, (uint32)(a), (uint32)(b))
#define log_error_3(err, a, b, c) log_error_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_error_4(err, a, b, c, d) log_error_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_error_5(err, a, b, c, d, e) log_error_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_error_s(err, s) log_error_s_x((err), (s), 0)
#define log_error_s_1(err, s, a) log_error_s_x((err), (s), 1, (uint32)(a))
#define log_error_s_2(err, s, a, b) log_error_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_error_s_3(err, s, a, b, c) log_error_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_error_s_4(err, s, a, b, c, d) log_error_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_error_s_5(err, s, a, b, c, d, e) log_error_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_info_1(err, a) log_info_x((err), 1, (uint32)(a))
#define log_info_2(err, a, b) log_info_x((err), 2, (uint32)(a), (uint32)(b))
#define log_info_3(err, a, b, c) log_info_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_info_4(err, a, b, c, d) log_info_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_info_5(err, a, b, c, d, e) log_info_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_info_s(err, s) log_info_s_x((err), (s), 0)
#define log_info_s_1(err, s, a) log_info_s_x((err), (s), 1, (uint32)(a))
#define log_info_s_2(err, s, a, b) log_info_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_info_s_3(err, s, a, b, c) log_info_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_info_s_4(err, s, a, b, c, d) log_info_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_info_s_5(err, s, a, b, c, d, e) log_info_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#define log_debug_1(err, a) log_debug_x((err), 1, (uint32)(a))
#define log_debug_2(err, a, b) log_debug_x((err), 2, (uint32)(a), (uint32)(b))
#define log_debug_3(err, a, b, c) log_debug_x((err), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_debug_4(err, a, b, c, d) log_debug_x((err), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_debug_5(err, a, b, c, d, e) log_debug_x((err), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))
#define log_debug_s(err, s) log_debug_s_x((err), (s), 0)
#define log_debug_s_1(err, s, a) log_debug_s_x((err), (s), 1, (uint32)(a))
#define log_debug_s_2(err, s, a, b) log_debug_s_x((err), (s), 2, (uint32)(a), (uint32)(b))
#define log_debug_s_3(err, s, a, b, c) log_debug_s_x((err), (s), 3, (uint32)(a), (uint32)(b), (uint32)(c))
#define log_debug_s_4(err, s, a, b, c, d) log_debug_s_x((err), (s), 4, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d))
#define log_debug_s_5(err, s, a, b, c, d, e) log_debug_s_x((err), (s), 5, (uint32)(a), (uint32)(b), (uint32)(c), (uint32)(d), (uint32)(e))

#undef ERROR
#undef BUILTIN_NUM_ERRORS
#undef BUILTIN_LAST_ERROR

typedef enum {
    ERROR = 1,
#define ERROR_MAPPING(ID, STR) ERROR_##ID,
#include "builtin/error.h"
    BUILTIN_NUM_ERRORS,
    BUILTIN_LAST_ERROR = 0xFF,
} builtin_error_enum_t;

#if CONFIG_RT_ERROR_STRING
extern const byte* builtin_errors_g[BUILTIN_NUM_ERRORS];
#define builtin_error(id) ((errot)builtin_errors_g[id])
#else
#define builtin_error(id) (((id) > ERROR) ? (((id) << 1) | ERROR) : (id))
#endif

#if __LIT_ENDIAN__
inline uint16 host_16_to_le(uint16 n) { return n; }
inline uint32 host_24_to_le(uint32 n) { return n; }
inline uint32 host_32_to_le(uint32 n) { return n; }
inline uint64 host_64_to_le(uint64 n) { return n; }
inline uint16 le_16_to_host(uint16 n) { return n; }
inline uint32 le_24_to_host(uint32 n) { return n; }
inline uint32 le_32_to_host(uint32 n) { return n; }
inline uint64 le_64_to_host(uint64 n) { return n; }
inline uint16 host_16_to_be(uint16 n) { return ((n & 0xff) << 8) | (n >> 8); }
inline uint32 host_24_to_be(uint32 n) { return ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16); }
inline uint32 host_32_to_be(uint32 n) { return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24); }
inline uint64 host_64_to_be(uint64 n) { return (((uint64)host_32_to_be(n & 0xffffffff)) << 32) | host_32_to_be(n >> 32); }
inline uint16 be_16_to_host(uint16 n) { return host_16_to_be(n); }
inline uint32 be_24_to_host(uint32 n) { return host_24_to_be(n); }
inline uint32 be_32_to_host(uint32 n) { return host_32_to_be(n); }
inline uint64 be_64_to_host(uint64 n) { return host_64_to_be(n); }
#else
inline uint16 host_16_to_le(uint16 n) { return ((n & 0xff) << 8) | (n >> 8); }
inline uint32 host_24_to_le(uint32 n) { return ((n & 0xff) << 16) | (n & 0xff00) | ((n & 0xff0000) >> 16); }
inline uint32 host_32_to_le(uint32 n) { return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24); }
inline uint64 host_64_to_le(uint64 n) { return (((uint64)host_32_to_le(n & 0xffffffff)) << 32) | host_32_to_le(n >> 32); }
inline uint16 le_16_to_host(uint16 n) { return host_16_to_le(n); }
inline uint32 le_24_to_host(uint32 n) { return host_24_to_le(n); }
inline uint32 le_32_to_host(uint32 n) { return host_32_to_le(n); }
inline uint64 le_64_to_host(uint64 n) { return host_64_to_le(n); }
inline uint16 host_16_to_be(uint16 n) { return n; }
inline uint32 host_24_to_be(uint32 n) { return n; }
inline uint32 host_32_to_be(uint32 n) { return n; }
inline uint64 host_64_to_be(uint64 n) { return n; }
inline uint16 be_16_to_host(uint16 n) { return n; }
inline uint32 be_24_to_host(uint32 n) { return n; }
inline uint32 be_32_to_host(uint32 n) { return n; }
inline uint64 be_64_to_host(uint64 n) { return n; }
#endif

#define HOST_BYTE(a) ((byte)((a)&0xff))
#define HOST_16_LE_BYTES(a) HOST_BYTE(a), HOST_BYTE((a)>>8)
#define HOST_24_LE_BYTES(a) HOST_16_LE_BYTES(a), HOST_BYTE((a)>>16)
#define HOST_32_LE_BYTES(a) HOST_16_LE_BYTES(a), HOST_16_LE_BYTES((a)>>16)
#define HOST_64_LE_BYTES(a) HOST_32_LE_BYTES(a), HOST_32_LE_BYTES((a)>>32)
#define HOST_16_BE_BYTES(a) HOST_BYTE((a)>>8), HOST_BYTE(a)
#define HOST_24_BE_BYTES(a) HOST_BYTE((a)>>16), HOST_16_BE_BYTES(a)
#define HOST_32_BE_BYTES(a) HOST_16_BE_BYTES((a)>>16), HOST_16_BE_BYTES(a)
#define HOST_64_BE_BYTES(a) HOST_32_BE_BYTES((a)>>32), HOST_32_BE_BYTES(a)

inline byte *host_16_to_lp(uint16 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    return p + 2;
}

inline byte *host_24_to_lp(uint32 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
    return p + 3;
}

inline byte *host_32_to_lp(uint32 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
    p[3] = HOST_BYTE(n >> 24);
    return p + 4;
}

inline byte *host_64_to_lp(uint64 n, byte *p) {
    p[0] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[2] = HOST_BYTE(n >> 16);
    p[3] = HOST_BYTE(n >> 24);
    p[4] = HOST_BYTE(n >> 32);
    p[5] = HOST_BYTE(n >> 40);
    p[6] = HOST_BYTE(n >> 48);
    p[7] = HOST_BYTE(n >> 56);
    return p + 8;
}

inline byte *host_16_to_bp(uint16 n, byte *p) {
    p[1] = HOST_BYTE(n);
    p[0] = HOST_BYTE(n >> 8);
    return p + 2;
}

inline byte *host_24_to_bp(uint32 n, byte *p) {
    p[2] = HOST_BYTE(n);
    p[1] = HOST_BYTE(n >> 8);
    p[0] = HOST_BYTE(n >> 16);
    return p + 3;
}

inline byte *host_32_to_bp(uint32 n, byte *p) {
    p[3] = HOST_BYTE(n);
    p[2] = HOST_BYTE(n >> 8);
    p[1] = HOST_BYTE(n >> 16);
    p[0] = HOST_BYTE(n >> 24);
    return p + 4;
}

inline byte *host_64_to_bp(uint64 n, byte *p) {
    p[7] = HOST_BYTE(n);
    p[6] = HOST_BYTE(n >> 8);
    p[5] = HOST_BYTE(n >> 16);
    p[4] = HOST_BYTE(n >> 24);
    p[3] = HOST_BYTE(n >> 32);
    p[2] = HOST_BYTE(n >> 40);
    p[1] = HOST_BYTE(n >> 48);
    p[0] = HOST_BYTE(n >> 56);
    return p + 8;
}

inline uint16 lp_16_to_host(byte *p) {
    return (((uint16)p[1]) << 8) | p[0];
}

inline uint32 lp_24_to_host(byte *p) {
    return (((uint32)p[2]) << 16) | (((uint32)p[1]) << 8) | p[0];
}

inline uint32 lp_32_to_host(byte *p) {
    return (((uint32)p[3]) << 24) | (((uint32)p[2]) << 16) | (((uint32)p[1]) << 8) | p[0];
}

inline uint64 lp_64_to_host(byte *p) {
    return
        (((uint64)p[7]) << 56) | (((uint64)p[6]) << 48) |
        (((uint64)p[5]) << 40) | (((uint64)p[4]) << 32) |
        (((uint64)p[3]) << 24) | (((uint64)p[2]) << 16) |
        (((uint64)p[1]) << 8)  | p[0];
}

inline uint16 bp_16_to_host(byte *p) {
    return (((uint16)p[0]) << 8) | p[1];
}

inline uint32 bp_24_to_host(byte *p) {
    return (((uint32)p[0]) << 16) | (((uint32)p[1]) << 8) | p[2];
}

inline uint32 bp_32_to_host(byte *p) {
    return (((uint32)p[0]) << 24) | (((uint32)p[1]) << 16) | (((uint32)p[2]) << 8) | p[3];
}

inline uint64 bp_64_to_host(byte *p) {
    return
        (((uint64)p[0]) << 56) | (((uint64)p[1]) << 48) |
        (((uint64)p[2]) << 40) | (((uint64)p[3]) << 32) |
        (((uint64)p[4]) << 24) | (((uint64)p[5]) << 16) |
        (((uint64)p[6]) << 8)  | p[7];
}

#if __ARCH_32BIT__
#define host_96_to_le(n)    host_32_to_le(n)
#define host_96_to_be(n)    host_32_to_be(n)
#define le_96_to_host(n)    le_32_to_host(n)
#define be_96_to_host(n)    be_32_to_host(n)
#define host_96_to_lp(n, p) host_32_to_lp((n), (p))
#define host_96_to_bp(n, p) host_32_to_bp((n), (p))
#define lp_96_to_host(p)    lp_32_to_host(p)
#define bp_96_to_host(p)    bp_32_to_host(p)
#elif __ARCH_64BIT__
#define host_96_to_le(n)    host_64_to_le(n)
#define host_96_to_be(n)    host_64_to_be(n)
#define le_96_to_host(n)    le_64_to_host(n)
#define be_96_to_host(n)    be_64_to_host(n)
#define host_96_to_lp(n, p) host_64_to_lp((n), (p))
#define host_96_to_bp(n, p) host_64_to_bp((n), (p))
#define lp_96_to_host(p)    lp_64_to_host(p)
#define bp_96_to_host(p)    bp_64_to_host(p)
#endif

// (~pow_2_sub_1) 相当于 (-pow_2)
#define ROUND_POW2(T, n, pow_2_sub_1) (((n) + (pow_2_sub_1)) & (~((T)(pow_2_sub_1))))
#define ROUND_PAD(n, pow_2_sub_1) ((((pow_2_sub_1) + 1) - ((n) & (pow_2_sub_1))) & (pow_2_sub_1))
inline uint32 round_up(uint32 n, uint32 a) { return ROUND_POW2(uint32, n, a); }
inline uint64 round_up_x(uint64 n, uint64 a) { return ROUND_POW2(uint64, n, a); }
inline uint96 round_up_v(uint96 n, uint96 a) { return ROUND_POW2(uint96, n, a); }

#ifdef __cplusplus
}
#endif
#endif /* CHAPL_BUILTIN_DECL_H */
