#include <string.h>
#include <stdlib.h>

#define JC_VERSION 0
#define JC_HEADER_SIZE (sizeof(int) * 3) 
#define JC_OFFSET_SIZE (int) sizeof(int)

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
  update tuple
*/

struct Jc_Page* jc_page_new (int page_size) {
  char * jc_page = malloc(page_size);
  int jc_version = JC_VERSION;
  int tuple_count = 0;
  memcpy(jc_page, &(jc_version), sizeof(int));
  memcpy(jc_page + sizeof(int), &(tuple_count), sizeof(int));
  memcpy(jc_page + sizeof(int) + sizeof(int), &(page_size), sizeof(int));

  return (struct Jc_Page*) jc_page;
}

int jc_page_tuple_count(struct Jc_Page* jc_page) {
  return (int) (*((char*)jc_page) + sizeof(int));
}

static char * get_start_of_last_tuple(struct Jc_Page* jc_page) {
  char * jc_page_ptr = (char*) jc_page;
  int tuple_count = jc_page_tuple_count(jc_page);
  int last_offset = (int) *(jc_page_ptr + JC_HEADER_SIZE + (sizeof(int) * (tuple_count - 1)));
  return jc_page_ptr + last_offset;
}

static char * get_end_of_offsets(struct Jc_Page * jc_page) {
  char * jc_page_ptr = (char*) jc_page;
  int tuple_count = jc_page_tuple_count(jc_page);
  return jc_page_ptr + JC_HEADER_SIZE + (sizeof(int) * tuple_count);
}

static int jc_page_tuple_remaining_space(struct Jc_Page* jc_page) {
  return get_start_of_last_tuple(jc_page) - get_end_of_offsets(jc_page);
}


void * jc_page_get_tuple(struct Jc_Page* jc_page, int position) {
  char * jc_page_ptr = (char*) jc_page;
  int tuple_count = jc_page_tuple_count(jc_page);
  int tuple_offset;
  if (position >= tuple_count) {
    return NULL;
  }
  tuple_offset = (int) *(jc_page_ptr + JC_HEADER_SIZE + (position * sizeof(int)));
  return jc_page_ptr + tuple_offset;
}

void * jc_page_add_tuple(struct Jc_Page* jc_page, void* data, int size) {
  if (size + JC_OFFSET_SIZE > jc_page_tuple_remaining_space(jc_page)) {
    return NULL;
  } else {
    char * jc_page_ptr = (char*) jc_page;
    int tuple_size_incremented = jc_page_tuple_count(jc_page) + 1;
    char * start_of_new_tuple = get_start_of_last_tuple(jc_page) - size;
    char * end_of_offsets = get_end_of_offsets(jc_page);
    int offset_to_new_tuple = start_of_new_tuple - jc_page_ptr;

    memcpy(jc_page_ptr + sizeof(int), &tuple_size_incremented, sizeof(int));
    memcpy(end_of_offsets, &offset_to_new_tuple, sizeof(int));
    return memcpy(start_of_new_tuple, data, size);
  }
}
