#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "JcStorageEngine.h"

int compare_bytes(char*, long, char*, long);
int append_test(void);
int re_write_test(void);

int main(void) {
  if (append_test()) {
    return 1;
  }

  if (re_write_test()) {
    return 1;
  }

  /*
  delete
  append write
  delete
  make sure the get offset is adjusted
  */
  return 0;
}

int re_write_test(void) {
  int test_fd;
  long block_size = 4096;
  struct Jc_Storage_Engine* storage_engine = jc_storage_engine_new("test", block_size);

  remove("test");
  test_fd = open("test", O_TMPFILE);
  {
    /*
      testing re-write
      append write
      append write
      write on first
      make sure first equals the rewrite
    */
    void* data_1 = calloc(block_size, 1);
    int test_val_1 = 100;

    void* data_2 = calloc(block_size, 1);
    int test_val_2 = 200;

    void* result_val = malloc(block_size);

    memcpy(data_1, &test_val_1, sizeof(test_val_1));
    memcpy(data_2, &test_val_2, sizeof(test_val_2));

    jc_storage_engine_write(storage_engine, data_1, 0);
    jc_storage_engine_write(storage_engine, data_2, 0);

    jc_storage_engine_get(storage_engine, result_val, 0);

    /* should not be equal with data_1 as this was overwritten */
    if (compare_bytes(data_1, block_size, result_val, block_size)) {
      return 1;
    }

    if (!compare_bytes(data_2, block_size, result_val, block_size)) {
      return 1;
    }
  }

  close(test_fd);
  return 0;
}

int append_test(void) {
  int test_fd;
  long block_size = 4096;
  struct Jc_Storage_Engine* storage_engine = jc_storage_engine_new("test", block_size);

  remove("test");
  test_fd = open("test", O_TMPFILE);

  {
    void* data_append_1 = calloc(block_size, 1);
    int test_val_1 = 100;
    long data_offset_1;
    void* result_val_1 = malloc(block_size);

    void* data_append_2 = calloc(block_size, 1);
    int test_val_2 = 200;
    long data_offset_2;
    void* result_val_2 = malloc(block_size);

    memcpy(data_append_1, &test_val_1, sizeof(test_val_1));
    memcpy(data_append_2, &test_val_2, sizeof(test_val_2));

    data_offset_1 = jc_storage_engine_write(storage_engine, data_append_1, 0);
    data_offset_2 = jc_storage_engine_write(storage_engine, data_append_2, 1);

    jc_storage_engine_get(storage_engine, result_val_1, data_offset_1);
    jc_storage_engine_get(storage_engine, result_val_2, data_offset_2);

    if (!compare_bytes(data_append_1, block_size, result_val_1, block_size)) {
      return 1;
    }

    if (!compare_bytes(data_append_2, block_size, result_val_2, block_size)) {
      return 1;
    }
  }

  close(test_fd);
  return 0;
}

int compare_bytes(char* data_1, long len_1, char* data_2, long len_2) {
  if (len_1 != len_2) {
    return 0;
  }

  {
    long i;
    for (i = 0; i < len_1; i++) {
      if (data_1[i] != data_2[i]) {
        return 0;
      }
    }
  }
  
  return 1;
}
