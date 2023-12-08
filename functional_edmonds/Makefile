CC = g++
CCFLAGS = -std=c++17 -Iinclude

SRC_DIR = src
OBJ_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) $(CCFLAGS) -c $@.cpp -o $(OBJ_DIR)/$@.o
	$(CC) $(CCFLAGS) $^ $(OBJ_DIR)/$@.o -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CCFLAGS) -c $< -o $@
	
run: clean all
	./$(EXEC)

setup:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(EXEC)

.PHONY: all setup run clean
