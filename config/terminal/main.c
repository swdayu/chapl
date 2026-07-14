void prh_impl_print_error(int error, int line);
#define prh_prerr(error) prh_impl_print_error((error), __LINE__)

#define PRH_FILE_INCLUDE
#define PRH_PLAT_IMPLEMENTATION
#define PRH_FILE_IMPLEMENTATION
#define PRH_TEST_IMPLEMENTATION
#include "prh_pretest.h"

// terminal.exe
// terminal.exe > pipe_stdout.txt

prh_handle file_error;
prh_handle file_output;

void prh_impl_print_error(int error, int line) {
    prh_print_zero_ended_bytes(file_error, "error ");
    prh_print_r32_dec(file_error, error, 0);
    prh_print_zero_ended_bytes(file_error, " line ");
    prh_print_r32_dec(file_error, line, 0);
    prh_print_zero_ended_bytes(file_error, "\n");
}

int main(int argc, char **argv) {
    prh_set_local_alloc(prh_default_alloc());

    file_error = prh_open_file_write("./stderr.txt");
    file_output = prh_open_file_write("./stdout.txt");
    prh_real_assert(file_error != prh_invalid_handle && file_output != prh_invalid_handle);

    prh_console_setup();

    prh_handle conin = prh_conin_handle();
    prh_handle conout = prh_conout_handle();

    prh_handle old_stdin = prh_stdin_handle();
    prh_handle old_stdout = prh_stdout_handle();
    prh_handle old_stderr = prh_stderr_handle();

#if defined(_CONSOLE)
    prh_print_zero_ended_bytes(file_output, "CONSOLE ");
#else
    prh_print_zero_ended_bytes(file_output, "WINDOWS ");
#endif

    prh_print_zero_ended_bytes(file_output, "conin ");
    prh_print_raw_hex(file_output, conin, 0);
    prh_print_zero_ended_bytes(file_output, " conout ");
    prh_print_raw_hex(file_output, conout, 0);

    prh_print_zero_ended_bytes(file_output, "\nstdin ");
    prh_print_raw_hex(file_output, old_stdin, 0);
    prh_print_zero_ended_bytes(file_output, " ");
    prh_print_raw_hex(file_output, prh_stdin_handle(), 0);

    prh_print_zero_ended_bytes(file_output, "\nstdout ");
    prh_print_raw_hex(file_output, old_stdout, 0);
    prh_print_zero_ended_bytes(file_output, " ");
    prh_print_raw_hex(file_output, prh_stdout_handle(), 0);

    prh_print_zero_ended_bytes(file_output, "\nstderr ");
    prh_print_raw_hex(file_output, old_stderr, 0);
    prh_print_zero_ended_bytes(file_output, " ");
    prh_print_raw_hex(file_output, prh_stderr_handle(), 0);
    prh_print_zero_ended_bytes(file_output, "\n\n");

    prh_r16 string_a[] = {'c', 'o', 'n', 'o', 'u', 't', ':', ' ', '\x4E\x1C', '\x1C\x4E', 'o', 'u', 't', 0x4E1C, '\n'};
    prh_print_wchars(conout, string_a, PRH_ARRAY_SIZE(string_a));
    prh_print_zero_ended_bytes(old_stdout, "stdout hello world\n");
    prh_print_zero_ended_bytes(old_stderr, "stderr hello world\n");

    prh_file_close(file_error);
    prh_file_close(file_output);

    prh_reg a = -100;
    prh_reg width = 8;
    prh_prend("%% percent %% %d %4d %0*d %.16????x %#+.80??x ended", a, a, width, a, a, a);
    printf("printf %03d %08x %+d %+u %#+.8x\n", -100, 100, 100, 100, -100);
    return 0;
}

#if 0
extern void wmainCRTStartup(void);

void wstart_entry(void) {
    wmainCRTStartup();
}
#endif
