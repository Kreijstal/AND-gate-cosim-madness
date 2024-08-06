#include "vpi_user.h"

// Function to compute the AND gate output
static int and_gate_vpi_calltf(PLI_BYTE8* user_data) {
    vpiHandle systf_handle, args_iter, arg_handle;
    s_vpi_value value;
    int a, b, y;

    // Get system function instance handle and argument iterator
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    args_iter = vpi_iterate(vpiArgument, systf_handle);

    // Get first argument (a)
    arg_handle = vpi_scan(args_iter);
    value.format = vpiIntVal;
    vpi_get_value(arg_handle, &value);
    a = value.value.integer;

    // Get second argument (b)
    arg_handle = vpi_scan(args_iter);
    value.format = vpiIntVal;
    vpi_get_value(arg_handle, &value);
    b = value.value.integer;

    // Compute AND operation
    y = a & b;

    // Set the output value
    value.format = vpiIntVal;
    value.value.integer = y;
    vpi_put_value(systf_handle, &value, NULL, vpiNoDelay);

    vpi_free_object(args_iter);
    return 0;
}

// Register the VPI system task
void and_gate_vpi_register(void) {
    s_vpi_systf_data tf_data;
    tf_data.type = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname = "$and_gate";
    tf_data.calltf = and_gate_vpi_calltf;
    tf_data.compiletf = NULL;
    tf_data.sizetf = NULL;
    tf_data.user_data = NULL;
    vpi_register_systf(&tf_data);
}

// Entry point for the VPI module
void (*vlog_startup_routines[])(void) = {
    and_gate_vpi_register,
    0
};
