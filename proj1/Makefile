CC := gcc

# Kataloger
SRC_DIRS := src 
BUILD_DIR := build

# Flaggor
CFLAGS := -std=c99 -Wall -Wextra -MMD -MP $(addprefix -I,$(SRC_DIRS)) -Isrc/libs -Isrc/jansson/src
LDFLAGS := -flto -Wl,--gc-sections
LIBS := -lcurl

# Hitta alla .c-filer i katalogerna rekursivt
SRC := $(shell find -L $(SRC_DIRS) -type f -name '*.c')

# Mappa .c → .o i build/
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# Binärens namn
BIN := main


# Ensure jansson_config.h exists before building
src/jansson/src/jansson_config.h: src/jansson/src/jansson_config.h.in
	sed -e 's/@json_inline@/inline/g' \
	    -e 's/@json_have_long_long@/1/g' \
	    -e 's/@json_have_atomic_builtins@/1/g' \
	    -e 's/@json_have_sync_builtins@/1/g' \
	    $< > $@

# Standardmål
all: src/jansson/src/jansson_config.h $(BIN)
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