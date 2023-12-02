CXX = clang++
CXXFLAGS = -std=c++17 -Iinclude
BUILD_DIR = build
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
EXEC = main

ifeq ($(OS),Windows_NT)
	CLEAN_CMD = if exist $(SRC_DIR)\*.o (del /f /q $(SRC_DIR)\*.o) && if exist main.o (del /f /q main.o) && if exist $(EXEC) (del /f /q $(EXEC)) # probably gonna have to rewrite this
	MKDIR = mkdir
	RUN_COMMAND = $(BUILD_DIR)/main.exe
else
	CLEAN_CMD = rm -rf
	MKDIR = mkdir -p
	RUN_COMMAND = ./$(BUILD_DIR)/main
endif

all: setup $(EXEC)

run: clean all
	$(RUN_COMMAND)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BUILD_DIR)/$(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

setup:
	$(MKDIR) $(BUILD_DIR)

clean:
	$(CLEAN_CMD) $(BUILD_DIR)

.PHONY: all run setup clean
