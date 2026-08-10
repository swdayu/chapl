#define PRH_STRIP_PREFIX
#define PRH_ARRAY_STRIP_PREFIX
#include "arrays.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define magic_bpe_debug magic_debug

struct pair {
    intp l;
    intp r;
};

struct item {
    struct pair pair;
    intp times;
};

struct items {
    struct item *dynamic_array;
};

struct input {
    intp *dynamic_array;
};

struct init_freq {
    intp key; // byte value
    intp value; // item index
};

struct in_index_apper_times {
    intp in_start;
    intp times;
};

struct freq {
    struct pair key; // token pair
    struct in_index_apper_times value;
};

struct bpe {
    intp init_size;
    struct items items;
    struct input input;
};

#if magic_bpe_debug
#include <stdio.h>
void print_item(struct item* items, intp i)
{
    struct pair *pair = &items[i].pair;
    if (pair->l == i)
    {
        printf("%c", (int)pair->r);
    }
    else
    {
        print_item(items, pair->l);
        print_item(items, pair->r);
    }
}
void print_bpe(struct bpe *b)
{
    intp i = 0, l, r;
    struct item *item;
    printf("tokens:\n");
    for (; i < array_len(&b->items); i += 1)
    {
        item = b->items.dynamic_array + i;
        l = item->pair.l;
        r = item->pair.r;
        if (i == l)
        {
            printf("  [%03zi] |%c| times %zi\n", i, (int)r, item->times);
        }
        else
        {
            printf("  [%03zi] %03zi %03zi times %zi |", i, l, r, item->times);
            print_item(b->items.dynamic_array, l);
            print_item(b->items.dynamic_array, r);
            printf("|\n");
        }
    }
    printf("output: |");
    for (i = 0; i < array_len(&b->input); i += 1)
    {
        intp ndx = b->input.dynamic_array[i];
        item = b->items.dynamic_array + ndx;
        l = item->pair.l;
        r = item->pair.r;
        if (ndx == l)
        {
            printf("%c", (int)r);
        }
        else
        {
            printf("[%03zi]", ndx);
        }
    }
    printf("|\n");
}
void print_seri(struct bpe *b)
{
    intp size = array_len(&b->items);
    int width; intp neg_1 = -1;
    if (size <= 0xff)
    {
        width = 2;
        neg_1 = 0xff;
    }
    else if (size <= 0xffff)
    {
        width = 4;
        neg_1 = 0xffff;
    }
    else if (size <= 0xffffffff)
    {
        width = 8;
        neg_1 = 0xffffffff;
    }
    else
    {
        width = 16;
    }
    struct item *item;
    intp i, seri_bytes = array_len(&b->input) * (width / 2);
    printf("result: ");
    if (size <= b->init_size)
    {
        printf("%0*zx ", width, (intp)0);
        seri_bytes += width / 2;
    }
    else
    {
        printf("%0*zx ", width, (intp)(width / 2));
        printf("%0*zx ", width, b->init_size);
        printf("%0*zx ", width, array_len(&b->items));
        seri_bytes += 3 * (width / 2);
        for (i = 0; i < array_len(&b->items); i += 1)
        {
            item = b->items.dynamic_array + i;
            seri_bytes += width / 2;
            if (item->times > 0)
            {
                if (i >= b->init_size)
                {
                    printf("%0*zx ", width, item->pair.l);
                    seri_bytes += width / 2;
                }
                printf("%0*zx ", width, item->pair.r);
            }
            else
            {
                printf("%0*zx ", width, neg_1);
            }
        }
    }
    for (i = 0; i < array_len(&b->input); i += 1)
    {
        intp ndx = b->input.dynamic_array[i];
        if (ndx < b->init_size)
        {
            item = b->items.dynamic_array + ndx;
            printf("%0*zx ", width, item->pair.r);
        }
        else
        {
            printf("%0*zx ", width, ndx);
        }
    }
    printf("bytes %zi\n", seri_bytes);
}
#endif

static struct bpe bpe_init(const char *s)
{
    struct bpe b = {0};
    struct init_freq *kv = NULL;
    struct init_freq *cur_kv;
    intp item_i = 0;
    intp len = s ? strlen(s) : 0;
    for (intp i = 0; i < len; i += 1)
    {
        intp key = s[i];
        cur_kv = hmgetp_null(kv, key);
        if (cur_kv == NULL)
        {
            array_push(&b.items, ((struct item){{item_i, key}, 1}));
            array_push(&b.input, item_i);
            hmput(kv, key, item_i);
            item_i += 1;
        }
        else
        {
            array_push(&b.input, cur_kv->value);
            b.items.dynamic_array[cur_kv->value].times += 1;
        }
    }
    b.init_size = array_len(&b.items);
    hmfree(kv);
    return b;
}

void bpe_enc(const char *s)
{
    struct bpe b = bpe_init(s);
    if (b.input.dynamic_array == NULL) return;
#if magic_bpe_debug
    printf("\n[input: %s] bytes %zi\n", s, strlen(s));
    print_bpe(&b);
#endif
    for (; ;)
    {
        struct freq *kv = NULL;
        intp i = 0, in_len = array_len(&b.input);
        for (; i < in_len - 1; i += 1)
        {
            struct pair key = {b.input.dynamic_array[i], b.input.dynamic_array[i + 1]};
            struct freq *cur_kv = hmgetp_null(kv, key);
            if (cur_kv == NULL)
            {
                hmput(kv, key, ((struct in_index_apper_times){i, 1}));
            }
            else
            {
                cur_kv->value.times += 1;
            }
            if (key.l == key.r && i + 2 < in_len && b.input.dynamic_array[i + 2] == key.l)
            {
                // aaac只需记录aa出现一次（不是二次）、ac出现一次，而
                // abac需要记录ab出现一次、ba出现一次、ac出现一次
                i += 1;
            }
        }
        intp max_times = 0;
        struct freq *max_freq = NULL;
        for (i = 0; i < hmlen(kv); i += 1)
        {
            if (kv[i].value.times > max_times)
            {
                max_times = kv[i].value.times;
                max_freq = kv + i;
            }
        }
        if (max_times <= 1)
        {
        #if magic_bpe_debug
            print_bpe(&b);
        #endif
            hmfree(kv);
            break;
        }
        intp new_token = array_len(&b.items);
        array_push(&b.items, ((struct item){max_freq->key, max_freq->value.times}));
        b.items.dynamic_array[max_freq->key.l].times -= max_freq->value.times;
        b.items.dynamic_array[max_freq->key.r].times -= max_freq->value.times;
        for (i = max_freq->value.in_start; i < array_len(&b.input); i += 1)
        {
            if (i + 1 >= array_len(&b.input))
            {
                break;
            }
            if (b.input.dynamic_array[i] == max_freq->key.l && b.input.dynamic_array[i + 1] == max_freq->key.r)
            {
                b.input.dynamic_array[i] = new_token;
                array_del(&b.input, i + 1);
            }
        }
    #if magic_bpe_debug
        print_bpe(&b);
    #endif
        hmfree(kv);
    }
#if magic_bpe_debug
    print_seri(&b);
#endif
    array_free(&b.input);
    array_free(&b.items);
}
