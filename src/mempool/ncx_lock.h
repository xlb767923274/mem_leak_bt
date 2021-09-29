#ifndef _NCX_LOCK_H_
#define _NCX_LOCK_H_

#include <pthread.h>

//私有内存则定义空
/*
#define ncx_shmtx_lock(x)   { }
#define ncx_shmtx_unlock(x) { }

typedef struct {
	ncx_uint_t spin;	
} ncx_shmtx_t;
*/

//多线程使用时
#define ncx_lock_init(x)    { pthread_mutex_init(x, NULL); }
#define ncx_shmtx_lock(x)   { pthread_mutex_lock(x); }
#define ncx_shmtx_unlock(x) { pthread_mutex_unlock(x); }

typedef pthread_mutex_t ncx_shmtx_t;


#endif
