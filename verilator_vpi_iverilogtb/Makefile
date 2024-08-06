VERILATOR = verilator
IVERILOG_VPI = iverilog-vpi
IVERILOG = iverilog
VVP = vvp

VERILATOR_ROOT = /usr/share/verilator
VERILATOR_FLAGS = -Wall --cc -CFLAGS -fPIC
CFLAGS = -Wall -g -I$(VERILATOR_ROOT)/include -Iobj_dir -fPIC
CXXFLAGS = $(CFLAGS) -std=c++14 -faligned-new
LDFLAGS = -L$(VERILATOR_ROOT)/include -L./obj_dir

VPI_SRC = and_gate_vpi.c
WRAPPER_SRC = and_gate_vpi_wrapper.cpp
VPI_OUT = and_gate_vpi.vpi
VERILOG_SRC = ../common/and_gate.v
VERILOG_PROXY = and_gate_proxy.v
VERILOG_TB = ../common/and_gate_tb.v
VVP_EXEC = and_gate_tb.vvp

.PHONY: all clean run

all: run
build: $(VVP_EXEC)

obj_dir/Vand_gate.cpp: $(VERILOG_SRC)
	$(VERILATOR) $(VERILATOR_FLAGS) $<

obj_dir/Vand_gate__ALL.o: obj_dir/Vand_gate.cpp
	$(MAKE) -C obj_dir -f Vand_gate.mk

obj_dir/libverilated.a: obj_dir/Vand_gate.cpp
	$(MAKE) -C obj_dir -f Vand_gate.mk

obj_dir/and_gate_vpi_wrapper.o: and_gate_vpi_wrapper.cpp and_gate_vpi_wrapper.h obj_dir/Vand_gate.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(VPI_OUT): $(VPI_SRC) and_gate_vpi_wrapper.h obj_dir/and_gate_vpi_wrapper.o obj_dir/Vand_gate__ALL.o obj_dir/libverilated.a
	$(IVERILOG_VPI) $(CXXFLAGS) $(LDFLAGS) $^ -lverilated -o $@

$(VVP_EXEC): $(VERILOG_PROXY) $(VERILOG_TB) $(VPI_OUT)
	$(IVERILOG) -o $@ -m./$(VPI_OUT) $(VERILOG_PROXY) $(VERILOG_TB)

run: $(VVP_EXEC)
	$(VVP) -M. -m$(basename $(VPI_OUT)) $<

clean:
	rm -rf obj_dir *.o *.vpi $(VVP_EXEC)
