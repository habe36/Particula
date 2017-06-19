/*

 Functions for barrier syncronization for multithreading.

 $Author: hiroshi $
 $Date: 2007/03/19 13:17:20 $
 $Revision: 1.2 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

 Ref. Steve Kleiman et al. Programming with Threads.
 
*/

#ifndef __H_PTHREAD_BARRIER_H__
#define __H_PTHREAD_BARRIER_H__

#include <pthread.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct barrier_struct {
  pthread_mutex_t lock;			/* Mutex lock for the entire structure */
  int n_clients;				/* Number of threads to wait for at barrier */
  int n_waiting;				/* Number of threads have called barrier_wait */
  int phase;					/* Flag to separate waiters from fast workers */
  int sum;						/* Sum of arguments passed to barrier_wait */
  int result;					/* Answer to be returned by barrier_wait */
  pthread_cond_t wait_cv;		/* Clients wait on condition var. to proceed */
} *barrier_t;

/*!
Create & initialize a barrier with the given number of client threads 
 */
barrier_t barrier_init(int n_clients);

/*!
Destroy a barrier 
 */
void barrier_destroy(barrier_t barrier);

/*!
Wait until the required number of threads enter the barrier 
 */
int barrier_wait(barrier_t barrier, int increment);


#ifdef __cplusplus
}
#endif

#endif /* __H_PTHREAD_BARRIER_H__ */
