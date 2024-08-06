VERILATOR = verilator
VERILATOR_FLAGS = -Wall --trace --cc --exe
CXX = g++
CXXFLAGS = -Wall -g -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -I obj_dir
LDFLAGS = -L/usr/share/verilator/include

TARGET = and_gate_tb
VERILOG_SOURCES = ../common/and_gate.v
CPP_SOURCES = and_gate_tb.cpp

.PHONY: all clean run

all: run

$(TARGET): $(VERILOG_SOURCES) $(CPP_SOURCES)
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILOG_SOURCES) $(CPP_SOURCES)
	make -C obj_dir -f Vand_gate.mk Vand_gate
build: $(TARGET)

run: $(TARGET)
	obj_dir/Vand_gate

clean:
	rm -rf obj_dir
	rm -f *.o $(TARGET)
