# Compiler and Linker
CXX = g++

TARGET = camera_test.cc

# Flags, Libraries and Includes
CXXFLAGS = -std=c++11 -Wall `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

# Source Files
SRC = $(wildcard *.cpp)

# Object Files
OBJ = $(SRC:.cpp=.o)

# Make Commands
all: 
	$(TARGET)
	$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: 
	%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
