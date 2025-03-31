#include "vpi_user.h"
#include "verilated.h"
#include "Vand_gate.h" // Include the Verilator-generated header for the AND gate

// Global pointer to the Verilator model instance
static Vand_gate* verilator_dut = nullptr;
// Global VerilatedContext for timing control (optional but good practice)
static VerilatedContext* contextp = nullptr;

// VPI System Function Implementation
// Corresponds to $verilator_and_gate(a, b) in Verilog
static PLI_INT32 verilator_and_gate_calltf(PLI_BYTE8* user_data) {
    (void)user_data; // Suppress unused parameter warning

    vpiHandle systf_handle, args_iter, arg_handle;
    s_vpi_value arg_val;
    int a_in, b_in, y_out;

    // Get handle to the system function instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    if (!systf_handle) {
        vpi_printf("ERROR: $verilator_and_gate VPI handle is NULL\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }

    // Get iterator for arguments
    args_iter = vpi_iterate(vpiArgument, systf_handle);
    if (!args_iter) {
        vpi_printf("ERROR: $verilator_and_gate could not get arguments iterator\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }

    // --- Get Input Arguments ---
    // Get first argument (a)
    arg_handle = vpi_scan(args_iter);
    if (!arg_handle) {
        vpi_printf("ERROR: $verilator_and_gate missing argument 'a'\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }
    arg_val.format = vpiIntVal;
    vpi_get_value(arg_handle, &arg_val);
    a_in = arg_val.value.integer;

    // Get second argument (b)
    arg_handle = vpi_scan(args_iter);
    if (!arg_handle) {
        vpi_printf("ERROR: $verilator_and_gate missing argument 'b'\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }
    arg_val.format = vpiIntVal;
    vpi_get_value(arg_handle, &arg_val);
    b_in = arg_val.value.integer;

    // Free the iterator (though not strictly necessary for scan)
    // vpi_free_object(args_iter); // Might cause issues if called too early

    // --- Instantiate and Evaluate Verilator Model ---
    // Ensure Verilator model and context are initialized (only once)
    if (!verilator_dut) {
        contextp = new VerilatedContext;
        // Verilated::traceEverOn(true); // Optional: Enable VCD tracing
        verilator_dut = new Vand_gate{contextp}; // Use context
        vpi_printf("INFO: $verilator_and_gate Instantiated Verilator model.\n");
    }

    // Set inputs on the Verilator model
    verilator_dut->a = a_in;
    verilator_dut->b = b_in;

    // Evaluate the Verilator model
    verilator_dut->eval();

    // Get the output from the Verilator model
    y_out = verilator_dut->y;

    // --- Return Output Value ---
    // Set the return value for the Verilog system function
    arg_val.format = vpiIntVal;
    arg_val.value.integer = y_out;
    vpi_put_value(systf_handle, &arg_val, NULL, vpiNoDelay);

    // Optional: Print debug info
    // vpi_printf("VPI: $verilator_and_gate(a=%d, b=%d) -> y=%d\n", a_in, b_in, y_out);

    return 0; // Return value for PLI_INT32 function (0 indicates success)
}

// VPI Registration Function
// Called by Icarus Verilog at startup
static void verilator_and_gate_register() {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysFunc; // System function ($)
    tf_data.sysfunctype = vpiIntFunc; // Returns an integer
    tf_data.tfname = (PLI_BYTE8*)"$verilator_and_gate"; // Verilog name
    tf_data.calltf = verilator_and_gate_calltf; // C++ function to call
    tf_data.compiletf = NULL; // No compile-time check function
    tf_data.sizetf = NULL; // No size function needed for int return
    tf_data.user_data = NULL; // No user data

    if (!vpi_register_systf(&tf_data)) {
        vpi_printf("ERROR: Failed to register VPI system function $verilator_and_gate\n");
    } else {
        vpi_printf("INFO: Registered VPI system function $verilator_and_gate\n");
    }
}

// Verilator Model Cleanup Function (called at end of simulation)
static PLI_INT32 cleanup_callback(p_cb_data cb_data) {
    (void)cb_data; // Suppress unused parameter warning
    if (verilator_dut) {
        verilator_dut->final(); // Call Verilator's final cleanup
        delete verilator_dut;
        verilator_dut = nullptr;
        delete contextp;
        contextp = nullptr;
        vpi_printf("INFO: $verilator_and_gate cleaned up Verilator model.\n");
    }
    return 0;
}

// Register cleanup callback
static void register_cleanup() {
    s_cb_data cb_data_s;
    cb_data_s.reason = cbEndOfSimulation;
    cb_data_s.cb_rtn = cleanup_callback;
    cb_data_s.obj = NULL;
    cb_data_s.time = NULL;
    cb_data_s.value = NULL;
    cb_data_s.user_data = NULL;
    vpi_register_cb(&cb_data_s);
}


// Entry point for Icarus Verilog VPI loading
// The functions listed here are called at simulation startup.
void (*vlog_startup_routines[])() = {
    verilator_and_gate_register,
    register_cleanup,
    0 // Null terminator
};

// Provide dummy sc_time_stamp() for Verilator libraries that might need it,
// especially when not explicitly linking SystemC.
#ifdef _WIN32 // Or a more specific check if needed
double sc_time_stamp() { return 0; } // Remove extern "C"
#endif
