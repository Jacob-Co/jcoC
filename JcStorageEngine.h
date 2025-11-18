#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct Jc_Storage_Engine {
  int fd;
  long last_item_offset;
  long block_size;
};
/*
store and retrieve data in files, efficiently using optimal disk blocks

api:
given a file name:

get new block
get block
write block
delete new block

*/

/*
for v 0
db table layout:
int version number
long current tuple offset
data_items*
*/

const long JC_STORAGE_ENGINE_DB_HEADER_SIZE = sizeof(int) + sizeof(long);

struct Jc_Storage_Engine* jc_storage_engine_new(char* name, long block_size) {
  struct Jc_Storage_Engine * storage_engine = malloc(sizeof(struct Jc_Storage_Engine));
  struct stat my_stat;

  storage_engine->block_size = block_size;
  storage_engine->fd = open(name, O_CREAT | O_RDWR | O_FSYNC );
  fstat(storage_engine->fd, &my_stat);

  if (my_stat.st_size == 0) {
    int ver = 0;
    storage_engine->last_item_offset = 0;
    
    write(storage_engine->fd, &ver, sizeof(int));
    write(storage_engine->fd, &storage_engine->last_item_offset, sizeof(long));

    return storage_engine;
  } else {
    lseek(storage_engine->fd, sizeof(int), SEEK_SET);
    read(storage_engine->fd, &storage_engine->last_item_offset, sizeof(long));

    return storage_engine;
  }
}

long jc_storage_engine_write(struct Jc_Storage_Engine* storage_engine, void * data, long offset) {
  if (storage_engine->last_item_offset < offset) {
    lseek(storage_engine->fd, (storage_engine->last_item_offset + 1) * storage_engine->block_size + JC_STORAGE_ENGINE_DB_HEADER_SIZE ,SEEK_SET);
    write(storage_engine->fd, data, storage_engine->block_size);
    return ++storage_engine->last_item_offset;
  } else {
    lseek(storage_engine->fd, offset * storage_engine->block_size + JC_STORAGE_ENGINE_DB_HEADER_SIZE ,SEEK_SET);
    write(storage_engine->fd, data, storage_engine->block_size);
    return offset;
  }
}

long jc_storage_engine_get(struct Jc_Storage_Engine* storage_engine, void* data, long offset) {
  if (offset > storage_engine->last_item_offset) return -1;
  lseek(storage_engine->fd, offset * storage_engine->block_size + JC_STORAGE_ENGINE_DB_HEADER_SIZE, SEEK_SET);
  return read(storage_engine->fd, data, storage_engine->block_size);
}

long jc_storage_engine_delete_last_block(struct Jc_Storage_Engine* storage_engine) {
  /*ftruncate(storage_engine->fd, storage_engine->latest_item_offset * storage_engine->block_size + JC_STORAGE_ENGINE_DB_HEADER_SIZE);*/ 
  return --storage_engine->last_item_offset;
}

long jc_storage_engine_get_last_item_offset(struct Jc_Storage_Engine* storage_engine) {
  return storage_engine->last_item_offset;
}
