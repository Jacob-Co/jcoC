#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif
struct Jc_Storage_Engine {
  int fd;
  long current_tuple_offset;
};

struct Jc_Tuple_Id {
  long storage_offset;
  int offset;
};


/*
for v 0
db table layout:
int version number
long current tuple offset
tuples*

*/

struct Jc_Storage_Engine* jc_storage_engine_new(char* name) {
  struct Jc_Storage_Engine * storage_engine = malloc(sizeof(struct Jc_Storage_Engine));
  struct stat my_stat;

  storage_engine->fd = open(name, O_CREAT | O_RDWR | O_FSYNC );
  fstat(storage_engine->fd, &my_stat);

  if (my_stat.st_size == 0) {
    /*initialize db layout with version and first tuple offset*/ 

    return storage_engine;
  } else {
    /* read and set current tuple offset */

    return storage_engine
  }
};

void jc_storage_engine_store(void* data, int size) {
  // fwrite(data, size, size, FILE *)
  
};

