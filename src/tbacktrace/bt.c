#include <pthread.h>
#include <execinfo.h>
#include <fcntl.h>
#include <unistd.h>

#include "bt.h"
#include "ncx_slab.h"

TAILQ_HEAD(tailq_bth, tailq_bth_entry);
struct tailq_bth bth_head;

uint64_t current_count;

static void init_tailq_packet(void)
{
    TAILQ_INIT(&bth_head); 
    current_count = 0;
}

#define SIZE 100

static void *monitor_bt(void *arg)
{
    uint64_t cur_time, maybe_leak;
    struct tailq_bth_entry *bte = NULL, *tmp;

    while(1) {
        sleep(TIME_TO_PRINT_BUFSTATE);

        maybe_leak = 0;
        mempool_status();

        cur_time = time(NULL);
        TAILQ_FOREACH_SAFE(bte, &bth_head, entry, tmp) {
            if (cur_time >= bte->bth.bt_time + TIME_TO_MAYBE_MEMLEAK) {
                //dump bth
                fprintf(stdout, "[maybe leak buf] tid:%s, addr: %p, len: %lu, "
                                "malloc time: %lu, current time: %lu\n%s\n", 
                                bte->bth.th_name, bte->bth.addr, bte->bth.blen,
                                bte->bth.bt_time, cur_time, bte->bth.bt_buf);
                maybe_leak++;
            }
        }
        fprintf(stdout, "\n===================================\n"
                        " current malloc buffer count : %lu\n"
                        " maybe leak buffer count: %lu\n"
                        "====================================\n\n",
                        current_count,
                        maybe_leak);
    }

    return NULL;
}

static void insert_to_tailq(struct tailq_bth_entry *p)
{
    current_count++;
    TAILQ_INSERT_TAIL(&bth_head, p, entry);
}

static int remove_from_tailq(void *rm)
{
    if (!rm) 
        return 0;

    struct tailq_bth_entry *p = NULL, *tmp;
    
    TAILQ_FOREACH_SAFE(p, &bth_head, entry, tmp) {
        if (p->bth.addr == (uint64_t *)rm) {
            TAILQ_REMOVE(&bth_head, p, entry);
            mempool_put(p);
            current_count--;
            return 0;
        }
    }

    return -1;
}

static struct tailq_bth_entry * mem_backtrace(void *addr, uint64_t len)
{
    if (!addr || len == 0) {
		perror("mem_backtrace(): invalid param");
        return NULL;
    }

    int j, nptrs, tlen = 0;
    void *buffer[SIZE];
    char **strings;

    struct tailq_bth_entry *p = (struct tailq_bth_entry *)mempool_get(sizeof(*p));
    if (!p) {
		printf("Warnning: mempool_get error\n");
        return NULL;
    }

	p->bth.addr = (uint64_t *)addr;
	p->bth.blen = len;
	p->bth.bt_time = time(NULL);
    pthread_getname_np(pthread_self(), p->bth.th_name, sizeof(p->bth.th_name));

    nptrs = backtrace(buffer, SIZE);
    strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		return NULL;
	}

	for (j = 0; j < nptrs; j++) {
		//printf(" [%02d] %s\n", j, strings[j]);
		snprintf(p->bth.bt_buf + tlen, sizeof(p->bth.bt_buf) - tlen, " [%02d] %s\n", j, strings[j]);
		tlen = strlen(p->bth.bt_buf);
    }

	free(strings);

    return p;
}

void record_bt(void *addr, uint64_t len)
{
    struct tailq_bth_entry *p = mem_backtrace(addr, len);
    if (p) {
        insert_to_tailq(p);
        printf("record_bt(): addr = %p, len = %lu\n", addr, len);
    }
}

void unrecord_bt(void *addr)
{
    remove_from_tailq(addr);
}

void init_bt(void)
{
    pthread_t tid;

    init_mempool(MEMPOOL_SIZE);//exit(-1) if init_mempool() err
    init_tailq_packet();

    // use a thread to monitor the backtrace status
    if (pthread_create(&tid, NULL, monitor_bt, NULL) !=0) {
        perror("Failed to create pthread init_hostpad_ct()\n");
        exit(-1);
    }

    printf("INFO: mem_backtrace() -- init_mempool() ok\n");

    pthread_detach(tid);
}

void print_backtrace(void)
{
    int j, nptrs;
    void *buffer[SIZE];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(-1);
    }

    printf("=========================backtrace() start:%d=========================\n", nptrs);
    for (j = 0; j < nptrs; j++)
        printf("  [%02d] %s\n", j, strings[j]);
    printf("=========================backtrace() end     =========================\n");

	free(strings);
}

