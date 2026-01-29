# Variables
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -O2
TARGET   = my_program
SRC      = main.cpp Delivery.cpp ProcessStats.cpp
OBJ      = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)
