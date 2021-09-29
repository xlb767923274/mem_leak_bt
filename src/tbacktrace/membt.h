#ifndef __MEMBT_H__
#define __MEMBT_H__

#include <stdint.h>

#define MEMPOOL_SIZE (1024*1024*50) // 50M, mempool size
#define TIME_TO_MAYBE_MEMLEAK  (60)     // interval time to dump buffer
#define TIME_TO_PRINT_BUFSTATE (20)     // interval time to dump buffer

void init_bt(void);
void record_bt(void *addr, uint64_t len);
void unrecord_bt(void *addr);

void print_backtrace(void);

#endif //__MEMBT_H__
