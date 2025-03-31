#include <vpi_user.h>
#include <stdint.h>
#include "and_gate_vpi_wrapper.h"

static void* and_gate_model = NULL;

static PLI_INT32 and_gate_vpi_calltf(PLI_BYTE8* user_data) {
    (void)user_data; // Suppress unused parameter warning
    vpiHandle systf_handle = NULL, args_iter = NULL, arg_handle = NULL;
    s_vpi_value value;
    int32_t a = 0, b = 0, y_result = 0; // Initialize variables
    vpiHandle y_arg_handle = NULL;
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

    // Get third argument (y) handle
    y_arg_handle = vpi_scan(args_iter);
    if (!y_arg_handle) {
        vpi_printf("ERROR: $and_gate VPI: Could not get handle for argument 'y'.\n");
        error_occurred = 1;
        goto cleanup;
    }
    // Check if y is writable (e.g., reg, integer)
    if (vpi_get(vpiType, y_arg_handle) != vpiReg && vpi_get(vpiType, y_arg_handle) != vpiIntegerVar) { // Use vpiIntegerVar
         vpi_printf("ERROR: $and_gate VPI: Output argument 'y' must be a writable type (e.g., reg, integer).\n");
         error_occurred = 1;
         goto cleanup;
    }


    // Set inputs and evaluate the model
    set_and_gate_inputs(and_gate_model, a, b);

    // Get the output
    y_result = get_and_gate_output(and_gate_model);

    // Set the output value back to the simulator via the 'y' argument handle
    value.format = vpiIntVal;
    value.value.integer = y_result;
    vpi_put_value(y_arg_handle, &value, NULL, vpiNoDelay);

cleanup:
    // Cleanup: Free the iterator handle (args_iter might be NULL if error occurred early)
    if (args_iter) {
        vpi_free_object(args_iter);
    }

    return error_occurred ? 1 : 0; // Return 0 on success, 1 on error
}

static PLI_INT32 and_gate_vpi_compiletf(PLI_BYTE8* user_data) {
    // Optional: Add compile-time checks here if needed
    (void)user_data; // Suppress unused parameter warning
    // Example check: Ensure 3 arguments are provided
    vpiHandle systf_handle = vpi_handle(vpiSysTfCall, NULL);
    vpiHandle args_iter = vpi_iterate(vpiArgument, systf_handle);
    int arg_count = 0;
    while (vpi_scan(args_iter)) { // vpi_scan frees the iterator automatically
        arg_count++;
    }
     // Note: vpi_iterate followed by vpi_scan(iter) until NULL is the way to count.
     // Need to re-iterate in calltf. A bit inefficient but standard.
     // Alternatively, don't free iterator here and pass it via user_data, but that's complex.
     // Let's keep it simple and rely on runtime checks in calltf.
     // A simpler check might be possible depending on VPI implementation details.
    if (arg_count != 3) {
         vpi_printf("ERROR: $and_gate VPI: Requires exactly 3 arguments (a, b, y).\n");
         vpi_control(vpiFinish, 1); // Abort simulation
    }

    return 0; // Success
}


static PLI_INT32 and_gate_vpi_register(void) {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysTask; // Changed to Task
    tf_data.sysfunctype = 0;    // Not used for Tasks
    tf_data.tfname = "$and_gate";
    tf_data.calltf = and_gate_vpi_calltf;
    tf_data.compiletf = and_gate_vpi_compiletf; // Add compile check
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
