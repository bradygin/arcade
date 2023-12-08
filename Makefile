# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lGL -lGLU -lglut -lpng

# Target executable
TARGET = brick

# Source files
SOURCES = BrickBreaker.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean