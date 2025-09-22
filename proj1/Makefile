CC := gcc

# Kataloger
SRC_DIRS := src 
BUILD_DIR := build

# Flaggor
CFLAGS := -std=c99 -Wall -Wextra -MMD -MP $(addprefix -I,$(SRC_DIRS))
LDFLAGS := -flto -Wl,--gc-sections
LIBS := -lcurl

# Hitta alla .c-filer i katalogerna rekursivt
SRC := $(shell find -L $(SRC_DIRS) -type f -name '*.c')

# Mappa .c → .o i build/
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# Binärens namn
BIN := main

# Standardmål
all: $(BIN)
	@echo "Build complete."

# Länka ihop allt
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@ $(LIBS)

# Generisk regel för att bygga objektfiler
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Hjälpmål
run: $(BIN)
	./$(BIN)

clean:
	@rm -rf $(BUILD_DIR) $(BIN)

print:
	@echo "Källfiler: $(SRC)"
	@echo "Objektfiler: $(OBJ)"
	@echo "Dependency-filer: $(DEP)"

# Inkludera auto-genererade beroenden
-include $(DEP)

.PHONY: all run clean print