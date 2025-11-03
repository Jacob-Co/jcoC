#include <stdlib.h>
#include <string.h>

struct Jc_Darr {
  int data_size;
  long size;
  long max_size;
  char* data_start;
};

struct Jc_Darr jc_darr_new(int data_size) {
  char* data_start = malloc(data_size);

  struct Jc_Darr darr = {0};
  darr.data_size = data_size;
  darr.max_size = 1;
  darr.data_start = data_start;

  return darr;
}

void jc_darr_push(struct Jc_Darr* darr, char* item) {
  if (darr->size == darr->max_size) {
    /* update (double size cap) data_start */
    {
      long curr_bytes = darr->size * darr->data_size;
      long new_bytes = curr_bytes * 2;
      char* new_data_start = malloc(new_bytes);

      memcpy(new_data_start, darr->data_start, curr_bytes);
      free(darr->data_start);
      darr->data_start = new_data_start;
    }
    
    /* update max size */
    {
      long new_max_size = darr->max_size * 2;
      darr->max_size = new_max_size;
    }
  }

  /* copy item into data_start */
  memcpy(darr->data_start + (darr->data_size * darr->size), item, darr->data_size);
  darr->size += 1;
}

