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
struct Jc_Prod_Con {
  long data_item_size;

  void (*consumer_function)(void*);

  int prod_count; 

  void* data_item;

  pthread_t threads[3];
};

void* _consume(void*); 

struct Jc_Prod_Con* jc_prod_con_new(long prod_data_size, void(*consumer_function)(void*)) {
  struct Jc_Prod_Con* prod_con = malloc(sizeof(struct Jc_Prod_Con));
  prod_con->data_item_size = prod_data_size;
  prod_con->consumer_function = consumer_function;
  prod_con->prod_count = 3;

  pthread_create(&prod_con->threads[0], NULL, _consume, (void*) &prod_con);
  pthread_create(&prod_con->threads[1], NULL, _consume, (void*) &prod_con);
  pthread_create(&prod_con->threads[2], NULL, _consume, (void*) &prod_con);

  return prod_con;
} 

void* _consume(void* ptr) {
  while(1) {
    struct Jc_Prod_Con* prod_con = (struct Jc_Prod_Con*) ptr;
    /*
    todo: 
      add wait here for a producer signal 
      lock data item mtx
    */

    void * consumer_data_item = malloc(prod_con->data_item_size);

    /*
    unlock data item mtx 
    */
    prod_con->consumer_function(consumer_data_item);
    free(consumer_data_item);
  }

  return NULL;
}

void produce(struct Jc_Prod_Con* prod_con, void* data_item) {
  /*
  todo: add lock here for data item
  */
  memcpy(prod_con->data_item, data_item, prod_con->data_item_size);
  /*
  todo: 
    add unlock here for data item
    signal consumer
    block producer until consumer picks this up
  */

}

