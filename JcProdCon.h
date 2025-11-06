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

struct JcProdCon {
  long data_size;
};
