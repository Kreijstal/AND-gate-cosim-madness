#include "Vand_gate.h" // Path to the Verilator-generated header for the and_gate module
#include "verilated_vpi.h" // For Verilator's VPI access (if needed, though direct access is more common for this use case)
#include <memory> // For std::unique_ptr

// Global pointer to the Verilated model instance
static std::unique_ptr<Vand_gate> top_ptr;

// Extern "C" functions are needed for GHDL to find them via VPI or direct foreign language interface
extern "C" {

// Initialization function: Creates an instance of the Verilated module
void and_gate_init() {
    top_ptr = std::make_unique<Vand_gate>();
}

// Function to set input 'a'
void set_a(char val) {
    if (top_ptr) {
        top_ptr->a = (val == '1'); // Assuming 'a' is a single bit (bool or C++ equivalent)
    }
}

// Function to set input 'b'
void set_b(char val) {
    if (top_ptr) {
        top_ptr->b = (val == '1'); // Assuming 'b' is a single bit
    }
}

// Function to evaluate the model (propagate changes)
void and_gate_eval() {
    if (top_ptr) {
        top_ptr->eval();
    }
}

// Function to get output 'y'
char get_y() {
    if (top_ptr) {
        return top_ptr->y ? '1' : '0'; // Assuming 'y' is a single bit
    }
    return 'X'; // Return 'X' if model not initialized or error
}

// Finalization function (optional, but good practice)
void and_gate_finalize() {
    if (top_ptr) {
        top_ptr->final(); // Call Verilator's finalization
        top_ptr.reset();  // Release the object
    }
}

} // extern "C"
