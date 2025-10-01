#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H
#include <stddef.h>


typedef struct KeyValueStore KeyValueStore;

typedef struct KeyValueStore {
  char **keys;
  char **values;
  size_t count;
  size_t capacity;
} KeyValueStore;

void kvstore_add(KeyValueStore *store, const char *key, const char *value);
void kvstore_free(KeyValueStore *store);
KeyValueStore* kvstore_create();

#endif