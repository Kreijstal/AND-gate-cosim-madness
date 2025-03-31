#include <vpi_user.h>
#include <stdint.h>
#include "and_gate_vpi_wrapper.h"

static void* and_gate_model = NULL;

static PLI_INT32 and_gate_vpi_calltf(PLI_BYTE8* user_data) {
    (void)user_data; // Suppress unused parameter warning
    vpiHandle systf_handle = NULL, args_iter = NULL, arg_handle = NULL;
    s_vpi_value value;
    int32_t a = 0, b = 0, y = 0; // Initialize variables
    int error_occurred = 0;

    if (!and_gate_model) {
        and_gate_model = create_and_gate_model();
    }

    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    if (!systf_handle) {
        vpi_printf("ERROR: $and_gate VPI: Could not get system task handle.\n");
        return 1; // Return error
    }

    args_iter = vpi_iterate(vpiArgument, systf_handle);
    if (!args_iter) {
        vpi_printf("ERROR: $and_gate VPI: Could not get argument iterator.\n");
        // No need to free args_iter as it's NULL
        return 1; // Return error
    }

    // Get first argument (a)
    arg_handle = vpi_scan(args_iter);
    if (!arg_handle) {
        vpi_printf("ERROR: $and_gate VPI: Could not get handle for argument 'a'.\n");
        error_occurred = 1;
        goto cleanup;
    }
    value.format = vpiIntVal;
    vpi_get_value(arg_handle, &value); // Assume format is correct for simplicity here
    a = value.value.integer;

    // Get second argument (b)
    arg_handle = vpi_scan(args_iter);
     if (!arg_handle) {
        vpi_printf("ERROR: $and_gate VPI: Could not get handle for argument 'b'.\n");
        error_occurred = 1;
        goto cleanup;
    }
    value.format = vpiIntVal;
    vpi_get_value(arg_handle, &value); // Assume format is correct
    b = value.value.integer;

    // Set inputs and evaluate the model
    set_and_gate_inputs(and_gate_model, a, b);
    
    // Ensure model is properly evaluated
    ((Vand_gate*)and_gate_model)->eval();
    
    // Get the output
    y = get_and_gate_output(and_gate_model);

    // Set the output value back to the simulator
    value.format = vpiIntVal;
    value.value.integer = y;
    // Use systf_handle here as the return value handle for SysFunc
    vpi_put_value(systf_handle, &value, NULL, vpiNoDelay); 

cleanup:
    // Cleanup: Free the iterator handle
    if (args_iter) {
        vpi_free_object(args_iter);
    }

    return error_occurred ? 1 : 0; // Return 0 on success, 1 on error
}

static PLI_INT32 and_gate_vpi_register(void) {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname = "$and_gate";
    tf_data.calltf = and_gate_vpi_calltf;
    tf_data.compiletf = NULL;
    tf_data.sizetf = NULL;
    tf_data.user_data = NULL;
    vpi_register_systf(&tf_data);

    return 0;
}

void (*vlog_startup_routines[])(void) = {
    (void (*)(void))and_gate_vpi_register,
    0
};
