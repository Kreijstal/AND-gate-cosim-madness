// Wrap C headers in extern "C" for C++ compatibility
#define PLI_DLLISPEC
extern "C" {
#include "vpi_user.h"
}

#include "and_gate.cpp" // CXXRTL-generated model

// Global pointer to the CXXRTL model instance
static cxxrtl_design::p_and__gate* cxxrtl_dut = nullptr;

// VPI System Function Implementation
// Corresponds to $cxxrtl_and_gate(a, b) in Verilog
static PLI_INT32 cxxrtl_and_gate_calltf(PLI_BYTE8* user_data) {
    (void)user_data; // Suppress unused parameter warning

    vpiHandle systf_handle, args_iter, arg_handle;
    s_vpi_value arg_val;
    int a_in, b_in, y_out;

    // Get handle to the system function instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    if (!systf_handle) {
        vpi_printf("ERROR: $cxxrtl_and_gate VPI handle is NULL\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }

    // Get iterator for arguments
    args_iter = vpi_iterate(vpiArgument, systf_handle);
    if (!args_iter) {
        vpi_printf("ERROR: $cxxrtl_and_gate could not get arguments iterator\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }

    // Get first argument (a)
    arg_handle = vpi_scan(args_iter);
    if (!arg_handle) {
        vpi_printf("ERROR: $cxxrtl_and_gate missing argument 'a'\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }
    arg_val.format = vpiIntVal;
    vpi_get_value(arg_handle, &arg_val);
    a_in = arg_val.value.integer;

    // Get second argument (b)
    arg_handle = vpi_scan(args_iter);
    if (!arg_handle) {
        vpi_printf("ERROR: $cxxrtl_and_gate missing argument 'b'\n");
        vpi_control(vpiFinish, 1);
        return 0;
    }
    arg_val.format = vpiIntVal;
    vpi_get_value(arg_handle, &arg_val);
    b_in = arg_val.value.integer;

    // Instantiate the CXXRTL model (only once)
    if (!cxxrtl_dut) {
        cxxrtl_dut = new cxxrtl_design::p_and__gate;
        vpi_printf("INFO: $cxxrtl_and_gate Instantiated CXXRTL model.\n");
    }

    // Set inputs and evaluate
    cxxrtl_dut->p_a.set<bool>(a_in != 0);
    cxxrtl_dut->p_b.set<bool>(b_in != 0);
    cxxrtl_dut->step();

    // Get the output
    y_out = cxxrtl_dut->p_y.get<bool>() ? 1 : 0;

    // Return output value
    arg_val.format = vpiIntVal;
    arg_val.value.integer = y_out;
    vpi_put_value(systf_handle, &arg_val, NULL, vpiNoDelay);

    return 0;
}

// VPI Registration Function
static void cxxrtl_and_gate_register() {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname = (PLI_BYTE8*)"$cxxrtl_and_gate";
    tf_data.calltf = cxxrtl_and_gate_calltf;
    tf_data.compiletf = NULL;
    tf_data.sizetf = NULL;
    tf_data.user_data = NULL;

    if (!vpi_register_systf(&tf_data)) {
        vpi_printf("ERROR: Failed to register VPI system function $cxxrtl_and_gate\n");
    } else {
        vpi_printf("INFO: Registered VPI system function $cxxrtl_and_gate\n");
    }
}

void (*vlog_startup_routines[])() = {
    cxxrtl_and_gate_register,
    0
};
