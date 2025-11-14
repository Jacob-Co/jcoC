struct Jc_Storage_Engine {
  int fd;
  long current_storage_offset;
};

struct Jc_Tuple_Id {
  long storage_offset;
  int offset;
};

struct Jc_Storage_Engine* jc_storage_engine_new(char* name) {
};

