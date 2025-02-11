# Makefile for the project

CXX = g++
CXXFLAGS = -std=c++17
SRCDIR = src
TARGET = Driver

SRCFILES = $(SRCDIR)/Driver.cpp $(SRCDIR)/DataFrame.cpp $(SRCDIR)/DecisionTree.cpp $(SRCDIR)/RandomForest.cpp $(SRCDIR)/Node.cpp

.PHONY: all clean

# Default target
all: $(TARGET)

# Build the target
$(TARGET):
	$(CXX) $(SRCFILES) -o $(TARGET) $(CXXFLAGS)

# Clean target
clean:
	rm -f $(TARGET)
