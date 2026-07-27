BUILD_DIR = build/
PROGRAM_NAME = renegade

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))

all: renegade

run: renegade
	./$(BUILD_DIR)/$(PROGRAM_NAME)

renegade: $(OBJ)
	gcc $(OBJ) -o $(BUILD_DIR)/$(PROGRAM_NAME) -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lcjson

build/%.o: src/%.c | build
	gcc -Wall -Wextra -O3 -g -c $< -o $@

build:
	mkdir -p $(BUILD_DIR)

clean: 
	rm -r $(BUILD_DIR)
