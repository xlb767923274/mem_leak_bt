This project does not need terminate the process, by recording the time of memory allocation to determine whether a memory leak occurs, instead of report the memory leak that occurs only after the program terminates like most memleak detection tools (asan, MEMWATCH...) . 

This is quite useful for some server programs that never exit，or leak problems that require a long time pressure test to reproduce.

# Run Demo:

1. make

   make asan=1

   make arm=1 // arm-linux-gnueabihf-

2. ./bin/test_main

3. see the log print, as follows:

   ```
   mw-server:~/Test/backtrace$ ./bin/test_main 
   INFO: mem_backtrace() -- init_mempool() ok
   record_bt(): addr = 0x7ffff0000b20, len = 20
   record_bt(): addr = 0x7fffe8000b20, len = 20
   record_bt(): addr = 0x7fffec000b20, len = 20
   record_bt(): addr = 0x7fffe0000b20, len = 20
   record_bt(): addr = 0x7fffd8000b20, len = 20
   
   ===================================
    current malloc buffer count : 5
    maybe leak buffer count: 0
   ====================================
   
   
   ===================================
    current malloc buffer count : 5
    maybe leak buffer count: 0
   ====================================
   
   [maybe leak buf] tid:thread0, addr: 0x7ffff0000b20, len: 20, malloc time: 1632897346, current time: 1632897406
    [00] ./bin/test_main() [0x401773]
    [01] ./bin/test_main(record_bt+0x23) [0x4018b2]
    [02] ./bin/test_main(mymalloc+0x31) [0x401278]
    [03] ./bin/test_main(test3+0x19) [0x4012be]
    [04] ./bin/test_main(test2+0xe) [0x4012e9]
    [05] ./bin/test_main() [0x40134d]
    [06] /lib/x86_64-linux-gnu/libpthread.so.0(+0x76db) [0x7ffff7bbb6db]
    [07] /lib/x86_64-linux-gnu/libc.so.6(clone+0x3f) [0x7ffff78e471f]
   
   
   ===================================
    current malloc buffer count : 5
    maybe leak buffer count: 1
   ====================================
   
   
   ===================================
    current malloc buffer count : 0
    maybe leak buffer count: 0
   ====================================
   mw-server:~/Test/backtrace$
   ```

# Integration into other projects:

1. compile: -I/lib -L/lib -lbt
2. include file "membt.h"
3. insert init_bt() in main() or init funtion
4. insert record_bt() in malloc position
5. insert unrecord_bt() in free position

# Adjustable parameters:

You can adjust these parameters to suit your environment



src/tbacktrace/membt.h

#define MEMPOOL_SIZE (1024 * 1024 * 50)     // 50M, mempool ，used for saving backtrace

#define TIME_TO_MAYBE_MEMLEAK  (60)     // seconds ： Memory not released after this time

#define TIME_TO_PRINT_BUFSTATE (20)     // seconds： Interval time to dump log

# others

mempool : https://github.com/dcshi/ncx_mempool.git
