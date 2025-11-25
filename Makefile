CC = gcc

CFLAGS = -fdiagnostics-color=always \
         -g \
         -Wall \
         -Wextra \
         -Wpedantic \
         -ansi \
         -I./include

LDFLAGS = -lm

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

DEPS = $(OBJECTS:.o=.d)


TARGET = $(BIN_DIR)/aerodromo_indexing

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(TARGET)
	@./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild

-include $(DEPS)
