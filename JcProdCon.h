#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
struct Jc_Prod_Cons {
  long data_item_size;

  void (*consumer_function)(void*);

  int prod_count; 

  void* data_item_ptr;
  char is_data_item_filled;
  pthread_mutex_t data_item_mtx;
  pthread_cond_t data_item_mtx_condition_ready;
  pthread_cond_t data_item_consumer_waiting;

  pthread_t threads[3];

  pthread_mutex_t cons_condition_mtx;
  pthread_cond_t cons_condition;
};

void* _consume(void*); 

struct Jc_Prod_Cons* jc_prod_cons_new(long prod_data_size, void(*consumer_function)(void*)) {
  struct Jc_Prod_Cons* prod_cons = malloc(sizeof(struct Jc_Prod_Cons));
  prod_cons->data_item_ptr = malloc(sizeof(prod_data_size));
  prod_cons->data_item_size = prod_data_size;
  prod_cons->is_data_item_filled = 0;
  prod_cons->consumer_function = consumer_function;
  prod_cons->prod_count = 3;

  pthread_mutex_init(&prod_cons->data_item_mtx, NULL);
  pthread_cond_init(&prod_cons->data_item_mtx_condition_ready, NULL);
  pthread_mutex_init(&prod_cons->cons_condition_mtx, NULL);
  pthread_cond_init(&prod_cons->cons_condition, NULL);

  pthread_create(&prod_cons->threads[0], NULL, _consume, (void*) prod_cons);
  pthread_create(&prod_cons->threads[1], NULL, _consume, (void*) prod_cons);
  pthread_create(&prod_cons->threads[2], NULL, _consume, (void*) prod_cons);

  return prod_cons;
} 

void* _consume(void* ptr) {
  struct Jc_Prod_Cons* prod_cons = (struct Jc_Prod_Cons*) ptr;
  while(1) {
    void* consumer_data_item;
    pthread_mutex_lock(&prod_cons->data_item_mtx);

    while(!prod_cons->is_data_item_filled) {
      pthread_cond_signal(&prod_cons->data_item_consumer_waiting);
      pthread_cond_wait(&prod_cons->data_item_mtx_condition_ready, &prod_cons->data_item_mtx);
    }

    consumer_data_item = malloc(prod_cons->data_item_size);
    memcpy(consumer_data_item, prod_cons->data_item_ptr, prod_cons->data_item_size);
    prod_cons->is_data_item_filled = 0;

    pthread_mutex_unlock(&prod_cons->data_item_mtx);

    prod_cons->consumer_function(consumer_data_item);
    free(consumer_data_item);
  }

  return NULL;
}

void jc_prod_cons_produce(struct Jc_Prod_Cons* prod_cons, void* data_item) {
  pthread_mutex_lock(&prod_cons->data_item_mtx);
  while(prod_cons->is_data_item_filled) {
    pthread_cond_wait(&prod_cons->data_item_consumer_waiting, &prod_cons->data_item_mtx);
  }

  memcpy(prod_cons->data_item_ptr, data_item, prod_cons->data_item_size);
  prod_cons->is_data_item_filled = 1;

  pthread_cond_signal(&prod_cons->data_item_mtx_condition_ready);

  pthread_mutex_unlock(&prod_cons->data_item_mtx);
}
