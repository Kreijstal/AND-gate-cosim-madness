# Makefile for SystemC AND Gate Simulation

# Compiler and Linker
CXX = g++
SYSTEMC_INCLUDE_DIR = /usr/include/systemc # Assuming systemc.h is here
SYSTEMC_LIB_DIR = /usr/lib/x86_64-linux-gnu # Path where libsystemc.so was found
SYSTEMC_LIB = -lsystemc

# Flags
CXXFLAGS = -I$(SYSTEMC_INCLUDE_DIR)
LDFLAGS = -L$(SYSTEMC_LIB_DIR) $(SYSTEMC_LIB) -lm

# Target executable
TARGET = and_gate_sim

# Source files
SRCS = and_gate_tb.cpp # and_gate.cpp is included by and_gate_tb.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o sc_trace.*

.PHONY: all run clean
