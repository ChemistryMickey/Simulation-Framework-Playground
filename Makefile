CXX := g++
WFLAGS := -Werror -Wall -Wextra -Wpedantic -Wreorder -Wunused-result
INC := -I include -isystem /usr/include/eigen3

CXXFLAGS += $(WFLAGS) -std=c++20 $(INC) -MMD -MP

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

MAIN := main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

TARGET := $(BUILD_DIR)/sim

DEPS := $(OBJS:.o=.d) $(MAIN_OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJS)
	$(CXX) $^ -o $@

$(BUILD_DIR)/main.o: $(MAIN) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(TARGET)

-include $(DEPS)

.PHONY: all clean run

