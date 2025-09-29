# Compiler and flags
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -MMD -MP -g
INCLUDES := -Isrc/libs -Isrc/jansson/src
LDFLAGS := -lcurl
TARGET := weather_app

# Directories
SRC_DIR := src
LIBS_DIR := $(SRC_DIR)/libs
JANSSON_DIR := $(SRC_DIR)/jansson/src
BUILD_DIR := build

# Jansson source files
JANSSON_SOURCES := $(wildcard $(JANSSON_DIR)/*.c)
JANSSON_OBJECTS := $(JANSSON_SOURCES:$(JANSSON_DIR)/%.c=$(BUILD_DIR)/jansson/%.o)

# App source files
LIBS_SOURCES := $(wildcard $(LIBS_DIR)/*.c)
LIBS_OBJECTS := $(LIBS_SOURCES:$(LIBS_DIR)/%.c=$(BUILD_DIR)/libs/%.o)

# Main source
MAIN_OBJECT := $(BUILD_DIR)/src/main.o

# All objects
ALL_OBJECTS := $(JANSSON_OBJECTS) $(LIBS_OBJECTS) $(MAIN_OBJECT)

# Dependency files
DEPS := $(ALL_OBJECTS:.o=.d)

# Default target
all: $(TARGET)

# Create the executable
$(TARGET): $(ALL_OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(ALL_OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete!"

# Compile Jansson objects
$(BUILD_DIR)/jansson/%.o: $(JANSSON_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile library objects
$(BUILD_DIR)/libs/%.o: $(LIBS_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile main object
$(BUILD_DIR)/src/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Clean complete!"

# Debug build with extra debugging symbols
debug: CFLAGS += -DDEBUG -O0 -ggdb3
debug: $(TARGET)

# Release build with optimization
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y libcurl4-openssl-dev build-essential

# Print variables for debugging
print-%:
	@echo $* = $($*)

# Create directory structure
init:
	@mkdir -p $(BUILD_DIR)/{jansson,libs,src}

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  run       - Build and run the application"
	@echo "  clean     - Remove build files"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized version"
	@echo "  install-deps - Install required dependencies"
	@echo "  help      - Show this help"

# Include dependency files
-include $(DEPS)

# Phony targets
.PHONY: all clean run debug release install-deps help init print-%