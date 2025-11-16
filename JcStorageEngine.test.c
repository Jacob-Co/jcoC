#define _GNU_SOURCE 
#include <fcntl.h>
#include <unistd.h>
#include "JcStorageEngine.h"

int main(void) {
  int test_fd = open("test", O_TMPFILE );
  struct Jc_Storage_Engine* storage_engine = jc_storage_engine_new("test");

  /*
  test append writes
  append write
  append write
  then get both and make sure values match


  testing re-write
  append write
  append write
  write on first
  make sure first equals the rewrite

  delete
  append write
  delete
  make sure the get offset is adjusted
  */

  close(test_fd);
  return 0;
}
