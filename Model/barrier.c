/*

 Functions for barrier syncronization for multithreading.

 $Author: hiroshi $
 $Date: 2007/03/21 01:30:56 $
 $Revision: 1.3 $
 
 Copyrights 2006,2007 All Rights Reserved by HABE, aka ABE Hiroshi, JAPAN.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "barrier.h"

/* Create & initialize a barrier with the given number of client threads */

barrier_t
barrier_init(int n_clients)
{
  barrier_t barrier =
    (barrier_t) malloc(sizeof (struct barrier_struct));
  if (barrier != NULL) {
    barrier->n_clients = n_clients;
    barrier->n_waiting = 0; barrier->phase = 0; barrier->sum = 0;
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->wait_cv, NULL);
  }
  return (barrier);
}
/* Destroy a barrier */
void
barrier_destroy(barrier_t barrier)
{
  pthread_mutex_destroy(&barrier->lock);
  pthread_cond_destroy(&barrier->wait_cv);
  free(barrier);
}


/* Wait until the required number of threads enter the barrier */
int
barrier_wait(barrier_t barrier, int increment)
{
  int my_phase;
  pthread_mutex_lock(&barrier->lock);
  my_phase = barrier->phase;
  barrier->sum += increment;
  barrier->n_waiting++;
  if (barrier->n_waiting == barrier->n_clients) {
    barrier->result = barrier->sum;
    barrier->sum = 0;
    barrier->n_waiting = 0;
    barrier->phase = 1 - my_phase;
    pthread_cond_broadcast(&barrier->wait_cv);
  }
  /* Wait for the end of this synchronization phase */
  while (barrier->phase == my_phase) {
    pthread_cond_wait(&barrier->wait_cv, &barrier->lock);
  }
  pthread_mutex_unlock(&barrier->lock);
  return (barrier->result);
}

