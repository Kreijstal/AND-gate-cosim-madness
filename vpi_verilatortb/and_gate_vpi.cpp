#include "vpi_user.h"
#include "verilated.h"
#include "verilated_vpi.h"

PLI_INT32 and_gate_calltf(PLI_BYTE8* user_data) {
    vpi_printf("and_gate_calltf called\n");

    vpiHandle systf_handle = vpi_handle(vpiSysTfCall, NULL);
    vpiHandle arg_iterator = vpi_iterate(vpiArgument, systf_handle);

    vpiHandle a_handle = vpi_scan(arg_iterator);
    vpiHandle b_handle = vpi_scan(arg_iterator);
    vpiHandle y_handle = vpi_scan(arg_iterator);

    if (!a_handle || !b_handle || !y_handle) {
        vpi_printf("Error: Could not get handles to a, b, or y\n");
        return 0;
    }

    s_vpi_value a_val, b_val, y_val;
    a_val.format = b_val.format = y_val.format = vpiIntVal;

    vpi_get_value(a_handle, &a_val);
    vpi_get_value(b_handle, &b_val);

    int a = a_val.value.integer;
    int b = b_val.value.integer;
    int y = a & b;

    y_val.value.integer = y;
    vpi_put_value(y_handle, &y_val, NULL, vpiNoDelay);

    vpi_printf("VPI: a=%d, b=%d -> y=%d\n", a, b, y);

    return 0;
}

void and_gate_register() {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysTask;
    tf_data.tfname = (PLI_BYTE8*)"$and_gate";
    tf_data.calltf = and_gate_calltf;
    tf_data.compiletf = NULL;
    tf_data.sizetf = NULL;
    tf_data.user_data = NULL;

    vpi_register_systf(&tf_data);
    vpi_printf("$and_gate registered\n");
}

void (*vlog_startup_routines[])() = {
    and_gate_register,
    0
};
