/*
producer write & forget

user will create a producer

produce(void *)
ie read page id 4321
consumer will be called by producer, then consumer will call consumer
  with the void*

Interface, create producer (callback*, function*)

create consumer of producer (producer*, function*)

*/

#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
struct Jc_Prod_Cons {
  long data_item_size;

  void (*consumer_function)(void*);

  int prod_count; 

  void* data_item;
  char is_data_item_valid;
  pthread_mutex_t data_item_mtx;

  pthread_t threads[3];

  pthread_cond_t cons_condition;
  pthread_mutex_t cons_condition_mtx;
};

void* _consume(void*); 

struct Jc_Prod_Cons* jc_prod_cons_new(long prod_data_size, void(*consumer_function)(void*)) {
  struct Jc_Prod_Cons* prod_cons = malloc(sizeof(struct Jc_Prod_Cons));
  prod_cons->data_item_size = prod_data_size;
  prod_cons->is_data_item_valid = 0;
  prod_cons->consumer_function = consumer_function;
  prod_cons->prod_count = 3;

  pthread_mutex_init(&prod_cons->data_item_mtx, NULL);
  pthread_mutex_init(&prod_cons->cons_condition_mtx, NULL);
  pthread_cond_init(&prod_cons->cons_condition, NULL);

  pthread_create(&prod_cons->threads[0], NULL, _consume, (void*) &prod_cons);
  pthread_create(&prod_cons->threads[1], NULL, _consume, (void*) &prod_cons);
  pthread_create(&prod_cons->threads[2], NULL, _consume, (void*) &prod_cons);

  return prod_cons;
} 

void* _consume(void* ptr) {
  struct Jc_Prod_Cons* prod_cons = (struct Jc_Prod_Cons*) ptr;
  while(1) {
    {
      pthread_mutex_lock(&prod_cons->cons_condition_mtx);
      pthread_cond_wait(&prod_cons->cons_condition, &prod_cons->cons_condition_mtx);
      pthread_mutex_unlock(&prod_cons->cons_condition_mtx);
    }

    pthread_mutex_lock(&prod_cons->data_item_mtx);

    if (!prod_cons->is_data_item_valid) {
      pthread_mutex_unlock(&prod_cons->data_item_mtx);
      continue;
    }

    {
      void * consumer_data_item = malloc(prod_cons->data_item_size);
      prod_cons->is_data_item_valid = 0;
      pthread_mutex_unlock(&prod_cons->data_item_mtx);

      prod_cons->consumer_function(consumer_data_item);
      free(consumer_data_item);
    }
  }

  return NULL;
}

void produce(struct Jc_Prod_Cons* prod_cons, void* data_item) {
  

  pthread_mutex_lock(&prod_cons->data_item_mtx);
  memcpy(prod_cons->data_item, data_item, prod_cons->data_item_size);
  prod_cons->is_data_item_valid = 1;
  pthread_mutex_unlock(&prod_cons->data_item_mtx);

  pthread_mutex_lock(&prod_cons->cons_condition_mtx);
  pthread_cond_signal(&prod_cons->cons_condition);
  pthread_mutex_unlock(&prod_cons->cons_condition_mtx);
  /*
  todo: 
    add unlock here for data item
    signal consumer
    block producer until consumer signals back
  */

}

