#ifndef GLENNS_METRO_UI_H
#define GLENNS_METRO_UI_H

#define PRINT_LINE(offset_x, y, length)                                        \
  do {                                                                         \
    for (int _i = 0; _i < (length); _i++)                                      \
      tb_set_cell((offset_x) + _i, (y), VERTICAL_LINE, TB_WHITE, TB_DEFAULT);  \
  } while (0)

#define PRINT_TOP_LINE(offset_x, y, length)                                    \
  tb_set_cell(offset_x, y, LEFT_TOP_CORNER, TB_WHITE, TB_DEFAULT);             \
  PRINT_LINE(offset_x + 1, y, length);                                         \
  tb_set_cell(offset_x + length, y, RIGHT_TOP_CORNER, TB_WHITE, TB_DEFAULT);

#define PRINT_MIDDLE_LINE(offset_x, y, length)                                 \
  tb_set_cell(offset_x, y, VERTICAL_RIGHT, TB_WHITE, TB_DEFAULT);              \
  PRINT_LINE(offset_x + 1, y, length);                                         \
  tb_set_cell(offset_x + length, y, VERTICAL_LEFT, TB_WHITE, TB_DEFAULT);

#define PRINT_BOTTOM_LINE(offset_x, y, length)                                 \
  tb_set_cell(offset_x, y, LEFT_BOTTOM_CORNER, TB_WHITE, TB_DEFAULT);          \
  PRINT_LINE(offset_x + 1, y, length);                                         \
  tb_set_cell(offset_x + length, y, RIGHT_BOTTOM_CORNER, TB_WHITE, TB_DEFAULT);

#define PRINT_LINE_WITH_CORNER(offset_x, y, length, corner_left, corner_right) \
  tb_set_cell(offset_x, y, corner_left, TB_WHITE, TB_DEFAULT);                 \
  PRINT_LINE(offset_x + 1, y, length);                                         \
  tb_set_cell(offset_x + length, y, corner_right, TB_WHITE, TB_DEFAULT);
#include "utils/LinkedList.h"

// typedef struct {
//   char *key;
//   char *value;
// } KeyValuePair;

typedef void (*ui_city_selection_update)(const char *city_name);

void add_city(const char *name);
void start_ui(ui_city_selection_update get_city_data);
void test_print_cities();
void free_cities();
void ui_add_city_data(LinkedList* city_data);

#endif // GLENNS_METRO_UI_H
