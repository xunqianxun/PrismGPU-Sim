MAKEFLAGS += --no-print-directory
# === 定义 ===
DEBUG ?= 

# === 编译器与选项 ===
CXX := g++
CXXFLAGS += -std=c++17 -O0 -Wall 
CXXFLAGS += -I/usr/local/include/eigen3
CXXFLAGS += -I src
LDFLAGS += -lSDL2

ifeq ($(DEBUG), 1)
CXXFLAGS += -g
endif

# === 路径 ===
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := build

# === 源文件与目标文件 ===
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET := $(BIN_DIR)/app

all: $(TARGET)

# === 规则 ===
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === 构建 ===
run: $(TARGET)
	

# === 清理 ===
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned build files"
