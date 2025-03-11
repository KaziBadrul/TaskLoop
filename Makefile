# Compiler
CXX = g++

# Directories
SRC_DIR = sources
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Source files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/Database.cpp $(SRC_DIR)/Group.cpp \
       $(SRC_DIR)/NormalUser.cpp $(SRC_DIR)/sqlite3.c $(SRC_DIR)/Task.cpp \
       $(SRC_DIR)/Timer.cpp $(SRC_DIR)/User.cpp $(SRC_DIR)/Color.cpp

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp, $(SRCS))) \
       $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter %.c, $(SRCS)))

# Output executable
TARGET = $(BIN_DIR)/my_program

# Compiler flags
CXXFLAGS = -I$(INCLUDE_DIR) -std=c++11

# Rule to build the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Rule to compile .c files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Create build and bin directories if they don't exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)