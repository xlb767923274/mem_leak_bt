#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <stdint.h>
#include "membt.h"

#include "tailq.h"

#define BT_BUF_SIZE 4096            // 4k, buffer size for one backtrace

typedef struct bth {
    uint64_t *addr;     // pointer address
    uint64_t blen;      // buffer lenth
    uint64_t bt_time;   // malloc time, use to judge leak
    char th_name[20];   // malloc thread name
    char bt_buf[BT_BUF_SIZE];   // save malloc trace/position
}bth_t;

struct tailq_bth_entry {
	TAILQ_ENTRY(tailq_bth_entry) entry;
	bth_t bth;
};

#endif //__BACKTRACE_H__
