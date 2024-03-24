# Compiler and flags
CXX = arm-linux-gnueabi-g++
CXXFLAGS = -std=c++11 -static-libstdc++
LDFLAGS = -lpthread

# Source directories
SRC_DIR = src
LIB_DIR = lib

# Build directory
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
LIB_SOURCES = $(wildcard $(LIB_DIR)/*.cpp)

# Target
TARGET = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%, $(SOURCES))

# Default rule
all: $(TARGET)

# Rule to build the target from source files
$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(LIB_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIB_SOURCES)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean