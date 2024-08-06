#include <verilated.h>
#include "Vand_gate.h"
#include <vpi_user.h>

static Vand_gate* and_gate_model = nullptr;

static int and_gate_vpi_calltf(PLI_BYTE8* user_data) {
    vpiHandle systf_handle, args_iter, arg_handle;
    s_vpi_value value;
    
    if (!and_gate_model) {
        and_gate_model = new Vand_gate;
    }

    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    args_iter = vpi_iterate(vpiArgument, systf_handle);

    // Get first argument (a)
    arg_handle = vpi_scan(args_iter);
    vpi_get_value(arg_handle, &value);
    and_gate_model->a = value.value.integer;

    // Get second argument (b)
    arg_handle = vpi_scan(args_iter);
    vpi_get_value(arg_handle, &value);
    and_gate_model->b = value.value.integer;

    // Evaluate the model
    and_gate_model->eval();

    // Set the output value
    value.format = vpiIntVal;
    value.value.integer = and_gate_model->y;
    vpi_put_value(systf_handle, &value, NULL, vpiNoDelay);

    vpi_free_object(args_iter);
    return 0;
}

void and_gate_vpi_register() {
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

void (*vlog_startup_routines[])(void) = {
    and_gate_vpi_register,
    0
};
