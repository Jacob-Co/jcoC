#include "JcProdCon.h"
#include <stdlib.h>
#include <unistd.h>

struct Arg {
  int timeout;
  int pos;
  int value;
  int* values;
};

void place_value(void*);

int main(void) {
  struct Jc_Prod_Cons* prod_cons = jc_prod_cons_new(sizeof(struct Arg), place_value);
  int* my_values = malloc(sizeof(int) * 3);
  struct Arg* a1 = malloc(sizeof(struct Arg));
  struct Arg* a2 = malloc(sizeof(struct Arg));
  struct Arg* a3 = malloc(sizeof(struct Arg));
  struct Arg* a4 = malloc(sizeof(struct Arg));
  a1->timeout = 10; 
  a1->pos = 0;
  a1->value = 0;
  a1->values = my_values;

  a2->timeout = 20;
  a2->pos = 1;
  a2->value = 1;
  a2->values = my_values;

  a3->timeout = 20;
  a3->pos = 2;
  a3->value = 2;
  a3->values = my_values;

  a4->timeout = 1;
  a4->pos = 0;
  a4->value = 100;
  a4->values = my_values;

  jc_prod_cons_produce(prod_cons, (void *)a1);
  jc_prod_cons_produce(prod_cons, (void *)a2);
  jc_prod_cons_produce(prod_cons, (void *)a3);
  jc_prod_cons_produce(prod_cons, (void *)a4);

  sleep(30);

  if (my_values[0] != 100) {
    return 1;
  }

  if (my_values[1] != 1) {
    return 1;
  }

  if (my_values[2] != 2) {
    return 1;
  }

  return 0;
}

void place_value(void* arg) {
  struct Arg* arg_arg = (struct Arg*) arg;
  sleep(arg_arg->timeout);
  arg_arg->values[arg_arg->pos] = arg_arg->value;
}
