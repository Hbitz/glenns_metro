#include "Cities.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------Internal function definitions----------------

const char *Cities_list = "Stockholm:59.3293:18.0686\n"
                          "Göteborg:57.7089:11.9746\n"
                          "Malmö:55.6050:13.0038\n"
                          "Uppsala:59.8586:17.6389\n"
                          "Västerås:59.6099:16.5448\n"
                          "Örebro:59.2741:15.2066\n"
                          "Linköping:58.4109:15.6216\n"
                          "Helsingborg:56.0465:12.6945\n"
                          "Jönköping:57.7815:14.1562\n"
                          "Norrköping:58.5877:16.1924\n"
                          "Lund:55.7047:13.1910\n"
                          "Gävle:60.6749:17.1413\n"
                          "Sundsvall:62.3908:17.3069\n"
                          "Umeå:63.8258:20.2630\n"
                          "Luleå:65.5848:22.1567\n"
                          "Kiruna:67.8558:20.2253\n";

void Cities_parse_list(Cities *_Cities, const char *list);

//..--------------------------------------------------------

int Cities_init(Cities *_Cities) {
  memset(_Cities, 0, sizeof(Cities));

  Cities_parse_list(_Cities, Cities_list);

  Cities_print(_Cities);

  Meteo_init(&_Cities->meteo, "https://api.open-meteo.com");

  return 0;
}

const char *Cities_get_list() { return Cities_list; }

void Cities_print(Cities *_Cities) {
  City *current = _Cities->head;
  if (current == NULL) {
    printf("No Cities to print\n");
    return;
  }

  printf("Available cities:\n");
  do {
    printf("  %s (%.4f, %.4f)\n", current->name, current->latitude,
           current->longitude);
    current = current->next;

  } while (current != NULL);
  printf("\n");
}

void Cities_parse_list(Cities *_Cities, const char *list) {
  char *list_copy = strdup(list);
  if (list_copy == NULL) {
    printf("Failed to allocate memory for list copy\n");
    return;
  }

  char *ptr = list_copy;
  char *name = NULL;
  char *lat_str = NULL;
  char *lon_str = NULL;

  do {
    if (name == NULL) {
      name = ptr;
    } else if (lat_str == NULL) {
      if (*(ptr) == ':') {
        lat_str = ptr + 1;
        *(ptr) = '\0';
      }
    } else if (lon_str == NULL) {
      if (*(ptr) == ':') {
        lon_str = ptr + 1;
        *(ptr) = '\0';
      }
    } else {
      if (*(ptr) == '\n') {
        *(ptr) = '\0';

        // printf("City: <%s>, Latitude: <%s>, Longitude: <%s>\n", name,
        // lat_str, lon_str);

        Cities_add(_Cities, name, atof(lat_str), atof(lon_str), NULL);

        name = NULL;
        lat_str = NULL;
        lon_str = NULL;
      }
    }

    ptr++;

  } while (*(ptr) != '\0');

  free(list_copy); // Free the duplicated string
  printf("Finished parsing City list\n");
}

int Cities_add(Cities *_Cities, char *_Name, float _Latitude, float _Longitude,
               City **_City) {
  if (!_Cities || !_Name) {
    printf("Invalid parameters to Cities_add\n");
    return -1;
  }

  City *new_City = (City *)malloc(sizeof(City));
  if (new_City == NULL) {
    printf("Failed to allocate memory for new City\n");
    return -1;
  }

  // Duplicate the name to avoid issues with string lifetimes
  new_City->name = strdup(_Name);
  if (new_City->name == NULL) {
    printf("Failed to allocate memory for city name\n");
    free(new_City);
    return -1;
  }

  new_City->latitude = _Latitude;
  new_City->longitude = _Longitude;

  new_City->prev = NULL;
  new_City->next = NULL;

  if (_Cities->tail == NULL) {
    _Cities->head = new_City;
    _Cities->tail = new_City;
  } else {
    new_City->prev = _Cities->tail;
    _Cities->tail->next = new_City;
    _Cities->tail = new_City;
  }

  if (_City != NULL)
    *(_City) = new_City;

  return 0;
}

int Cities_get(Cities *_Cities, const char *_Name, City **_CityPtr) {
  if (!_Cities || !_Name) {
    printf("Invalid parameters to Cities_get\n");
    return -1;
  }

  City *current = _Cities->head;
  if (current == NULL)
    return -1;

  do {
    // Case-insensitive comparison for better user experience
    if (strcmp(current->name, _Name) == 0) {
      if (_CityPtr != NULL)
        *(_CityPtr) = current;

      return 0;
    }

    current = current->next;

  } while (current != NULL);

  return -1;
}

void Cities_remove(Cities *_Cities, City *_City) {
  if (!_Cities || !_City) {
    printf("Invalid parameters to Cities_remove\n");
    return;
  }

  if (_City->next == NULL && _City->prev == NULL) // I am alone
  {
    _Cities->head = NULL;
    _Cities->tail = NULL;
  } else if (_City->prev == NULL) // I am first
  {
    _Cities->head = _City->next;
    _City->next->prev = NULL;
  } else if (_City->next == NULL) // I am last
  {
    _Cities->tail = _City->prev;
    _City->prev->next = NULL;
  } else // I am in the middle
  {
    _City->prev->next = _City->next;
    _City->next->prev = _City->prev;
  }

  // Free the duplicated name and the city structure
  free(_City->name);
  free(_City);
}

int Cities_getTemperature(Cities *_Cities, City *_City) {
  if (!_Cities || !_City) {
    printf("Invalid parameters to Cities_getTemperature\n");
    return -1;
  }

  int result = Meteo_getTemperature(&_Cities->meteo, _City->latitude,
                                    _City->longitude, _City->name);
  if (result != 0) {
    printf("Weather API request failed for %s\n", _City->name);
  }
  return result;
}

void Cities_dispose(Cities *_Cities) {
  if (!_Cities)
    return;

  City *current = _Cities->head;
  while (current != NULL) {
    City *next = current->next;
    free(current->name); // Free the duplicated name
    free(current);       // Free the city structure
    current = next;
  }

  _Cities->head = NULL;
  _Cities->tail = NULL;
}
