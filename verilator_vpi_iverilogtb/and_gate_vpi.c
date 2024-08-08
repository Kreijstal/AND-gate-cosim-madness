#include <vpi_user.h>
#include <stdint.h>
#include "and_gate_vpi_wrapper.h"

static void* and_gate_model = NULL;

static PLI_INT32 and_gate_vpi_calltf(PLI_BYTE8* user_data) {
    (void)user_data; // Suppress unused parameter warning
    vpiHandle systf_handle, args_iter, arg_handle;
    s_vpi_value value;
    int32_t a, b, y;

    if (!and_gate_model) {
        and_gate_model = create_and_gate_model();
    }

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

    // Set inputs and evaluate the model
    set_and_gate_inputs(and_gate_model, a, b);

    // Get the output
    y = get_and_gate_output(and_gate_model);

    // Set the output value
    value.format = vpiIntVal;
    value.value.integer = y;
    vpi_put_value(systf_handle, &value, NULL, vpiNoDelay);

    vpi_free_object(args_iter);
    return 0;
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