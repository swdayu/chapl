#define PRH_SOCK_INCLUDE
#define PRH_SOCK_IMPLEMENTATION
#define PRH_TEST_IMPLEMENTATION
#include "prh_include.h"

typedef struct {
    prh_socket tcp;
} prh_tcp_control;

void prh_tcp_error_handling(prh_tcp_control *c) {

}

prh_yield *prh_tcp_routine(prh_co_struct *co, prh_reg yield) {
    prh_socket *tcp = &((prh_tcp_control *)co)->tcp;
    switch (yield) {
    prh_co_start:
        break;
    prh_co_error:
        prh_tcp_error_handling((prh_tcp_control *)co);
        break;
    }
    return prh_co_end;
}

int main(int argc, char **argv) {
    prh_main_init();
    prh_ehub_init(1);
    prh_listen *l = prh_tcp_listen("127.0.0.1", 8888, 0, 1);
    prh_tcp_server(l, prh_tcp_routine, sizeof(prh_tcp_control));
    prh_tcp_accept(l);
    prh_ehub_join();
    return 0;
}
