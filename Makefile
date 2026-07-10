# Define compilation flags/toolchain
BUILD ?= Release
CXX := g++
WFLAGS := -Werror -Wall -Wextra -Wpedantic -Wreorder -Wunused-result
INC := -I include -isystem /usr/include/eigen3 -lfmt

cxxflags.common := $(WFLAGS) -std=c++20 $(INC) -MMD -MP
cxxflags.Debug := -fsanitize=address,undefined -g -fPIC -fno-omit-frame-pointer
cxxflags.Release := -O3 -DNDEBUG
CXXFLAGS = ${cxxflags.${BUILD}} ${cxxflags.common}

ldflags.common := 
ldflags.Debug := -fsanitize=address,undefined
ldflags.Release := -flto
LDFLAGS = ${ldflags.${BUILD}} ${ldflags.common}

# Define directories
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

# Define sources
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TESTS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TESTS))

# Define targets
MAIN := main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

TARGET := $(BUILD_DIR)/sim
TEST_TARGET := $(BUILD_DIR)/all_tests

# Define additional dependencies. This is so header file changes cause rebuilds
DEPS := $(OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(TEST_OBJS:.o=.d)

# Define recipes
all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJS)
	$(CXX) $^ -o $@ ${LDFLAGS} $(CXXFLAGS) 

$(BUILD_DIR)/main.o: $(MAIN) | $(BUILD_DIR)
	$(CXX) -c $< -o $@ ${LDFLAGS} $(CXXFLAGS) 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) -c $< -o $@ ${LDFLAGS} $(CXXFLAGS) 

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) -lgtest -lgtest_main -c $< -o $@ ${LDFLAGS} $(CXXFLAGS) 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(TARGET)

test: GTEST_FILTER = *
test: BUILD = Debug
test: $(TEST_TARGET)
	@echo "Building with ${BUILD} and Filters ${GTEST_FILTER}"
	$(TEST_TARGET) --gtest_filter=$(GTEST_FILTER)

$(TEST_TARGET): $(TEST_OBJS) $(OBJS) | $(BUILD_DIR)	
	$(CXX) $^ -lgtest -lgtest_main ${CXXFLAGS} -o $@ ${LDFLAGS}

-include $(DEPS)

.PHONY: all clean run test

