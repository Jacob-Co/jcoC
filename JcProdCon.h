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

struct Jc_Prod_Con {
  long prod_data_size;
  void (*consumer_function)(void*);
  /* add queue for data items*/
};

struct Jc_Prod_Con jc_prod_con_new(long prod_data_size, void(*consumer_function)(void*)) {
  struct Jc_Prod_Con prod_con = {0};
  prod_con.prod_data_size = prod_data_size;
  prod_con.consumer_function = consumer_function;
  /* initialize queue for data items*/
  return prod_con;
} 


