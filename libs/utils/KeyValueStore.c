#include "KeyValueStore.h"
#include "utils/utils.h"
#include <stdlib.h>

KeyValueStore* kvstore_create() {
  KeyValueStore *store = malloc(sizeof(KeyValueStore));
  store->capacity = 16;
  store->count = 0;
  store->keys = malloc(store->capacity * sizeof(char *));
  store->values = malloc(store->capacity * sizeof(char *));
  return store;
}

void kvstore_add(KeyValueStore *store, const char *key, const char *value) {
  if (store->count >= store->capacity) {
    store->capacity *= 2;
    store->keys = realloc(store->keys, store->capacity * sizeof(char *));
    store->values = realloc(store->values, store->capacity * sizeof(char *));
  }

  store->keys[store->count] = strdup(key);
  store->values[store->count] = strdup(value);
  store->count++;
}

void kvstore_free(KeyValueStore *store) {
  if (!store)
    return;

  for (size_t i = 0; i < store->count; i++) {
    free(store->keys[i]);
    free(store->values[i]);
  }
  free(store->keys);
  free(store->values);
  free(store);
}
