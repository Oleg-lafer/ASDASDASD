# Makefile for Fortuna PRNG Project using OpenSSL

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LDFLAGS = -lssl -lcrypto

# Source files and object files
SRC = Fortuna.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = fortuna

# Default target to compile the program
all: $(EXEC)

# Rule to link the object files into the final executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Rule to compile the source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the compiled files
clean:
	rm -f $(OBJ) $(EXEC)

# Rule to run the program after building
run: $(EXEC)
	./$(EXEC)
