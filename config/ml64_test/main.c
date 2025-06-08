void *asm_curr_rsp(void);

typedef struct {
    void *rsp;
    intp depth;
    intp max_depth;
} stack_item_t;

typedef struct {
    void *coro;
    void *coro_rsp;
    void *real_rsp;
    uptr count;
    stack_item_t item[1024];
} stack_info_t;

intp max_stack_depth = 0;
stack_info_t stack_info;

void update_max_stack_depth(struct coro *coro) {
    static void *initial_rsp = null;
    intp curr_depth;
    void *rsp = asm_curr_rsp();
    if (initial_rsp == null) {
        initial_rsp = coro->rsp;
        stack_info.coro = coro;
        stack_info.coro_rsp = (void *)coro->rsp;
        stack_info.real_rsp = rsp;
        stack_info.count = 0;
    }
    curr_depth = ((byte *)initial_rsp - (byte *)rsp);
    if (curr_depth > max_stack_depth) {
        max_stack_depth = curr_depth;
    }
    if (stack_info.count < 1024) {
        stack_info.item[stack_info.count].rsp = rsp;
        stack_info.item[stack_info.count].depth = curr_depth;
        stack_info.item[stack_info.count].max_depth = max_stack_depth;
        stack_info.count++;
    }
}

void print_stack_info(void) {
    uptr i; void* rsp;
    printf("Stack info: coro %p rsp %p coro_rsp %p\n", stack_info.coro, stack_info.real_rsp, stack_info.coro_rsp);
    for (i = 0; i < stack_info.count; i++) {
        rsp = stack_info.item[i].rsp;
        printf("  [%lld]: rsp %p depth %lld max_depth %lld left %d\n", i, rsp,
            stack_info.item[i].depth, stack_info.item[i].max_depth, (int)((byte*)rsp - (byte*)stack_info.coro - sizeof(struct coro)));
    }
}
