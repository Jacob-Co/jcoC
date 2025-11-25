#include <string.h>
#define JC_HEADER_SIZE (sizeof(int) * 3) 

struct Jc_Page {
  char data;
  /*
  int version
  int tupleCount
  int pageSize
  -- start of offset --
  int offset2tuple 1
  int offset2tuple 2

  tuple 2
  tuple 1
  */
};


/*
new jc page
get tuple count
get tuple
insert tuple
*/

struct Jc_Page* jc_page_new (void* jc_page) {
  return (struct Jc_Page*) jc_page;
}

int jc_page_tuple_count(struct Jc_Page* jc_page) {
  return (int) (*((char*)jc_page) + sizeof(int));
}

void * jc_page_get_tuple(struct Jc_Page* jc_page, int position) {
  return (char*) jc_page + JC_HEADER_SIZE + (position * sizeof(int));
}

int jc_page_add_tuple(struct Jc_Page* jc_page, void* data, unsigned long size) {
  void* dest = (char*) jc_page + JC_HEADER_SIZE + (jc_page_tuple_count(jc_page) * sizeof(int));

  memcpy(jc_page, data, size);
  return 1;
}
