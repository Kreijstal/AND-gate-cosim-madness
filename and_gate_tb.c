#include <stdio.h>
#include <stdlib.h>
#include <vpi_user.h>

static int test_case = 0;
static vpiHandle a_handle, b_handle, y_handle;

PLI_INT32 check_output(PLI_BYTE8 *user_data);
PLI_INT32 run_test(PLI_BYTE8 *user_data);

static PLI_INT32 delay_callback(p_cb_data cb_data) {
    (void)cb_data; // Suppress unused parameter warning
    return run_test(NULL);
}

static PLI_INT32 propagation_delay_callback(p_cb_data cb_data) {
    (void)cb_data; // Suppress unused parameter warning
    return check_output(NULL);
}

PLI_INT32 check_output(PLI_BYTE8 *user_data) {
    (void)user_data;  // Suppress unused parameter warning
    s_vpi_value value = {vpiIntVal, {0}};
    s_vpi_time time;

    // Print the current simulation time
    time.type = vpiSimTime;
    vpi_get_time(NULL, &time);
    printf("Current Simulation Time: %0u\n", time.low);

    // Check output
    vpi_get_value(y_handle, &value);
    int a = (test_case >> 1) & 1;
    int b = test_case & 1;
    int y = value.value.integer;
    int expected = a & b;

    printf("Test case %d: a=%d, b=%d, y=%d ", test_case, a, b, y);
    if (y == expected) {
        printf("(PASS)\n");
    } else {
        printf("(FAIL: expected %d)\n", expected);
    }

    test_case++;

    if (test_case < 4) {
        // Schedule next test case after 10 time units
        s_cb_data cb_data_s;
        s_vpi_time vpi_time_s;

        vpi_time_s.type = vpiSimTime;
        vpi_time_s.low = 10;
        vpi_time_s.high = 0;

        cb_data_s.reason = cbAfterDelay;
        cb_data_s.cb_rtn = delay_callback;
        cb_data_s.obj = NULL;
        cb_data_s.time = &vpi_time_s;
        cb_data_s.value = NULL;
        cb_data_s.user_data = NULL;

        vpi_register_cb(&cb_data_s);
    } else {
        printf("All test cases completed.\n");
        vpi_control(vpiFinish, 0);
    }

    return 0;
}

PLI_INT32 run_test(PLI_BYTE8 *user_data) {
    (void)user_data;  // Suppress unused parameter warning
    s_vpi_value value = {vpiIntVal, {0}};

    // Initialize handles if not already done
    if (a_handle == NULL) {
        a_handle = vpi_handle_by_name("and_gate_tb.a", NULL);
        b_handle = vpi_handle_by_name("and_gate_tb.b", NULL);
        y_handle = vpi_handle_by_name("and_gate_tb.y", NULL);
    }

    // Drive inputs
    value.value.integer = (test_case >> 1) & 1;
    vpi_put_value(a_handle, &value, NULL, vpiNoDelay);
    value.value.integer = test_case & 1;
    vpi_put_value(b_handle, &value, NULL, vpiNoDelay);

    // Schedule output check after a small propagation delay (e.g., 1 time unit)
    s_cb_data cb_data_s;
    s_vpi_time vpi_time_s;

    vpi_time_s.type = vpiSimTime;
    vpi_time_s.low = 1;  // 1 time unit delay for propagation
    vpi_time_s.high = 0;

    cb_data_s.reason = cbAfterDelay;
    cb_data_s.cb_rtn = propagation_delay_callback;
    cb_data_s.obj = NULL;
    cb_data_s.time = &vpi_time_s;
    cb_data_s.value = NULL;
    cb_data_s.user_data = NULL;

    vpi_register_cb(&cb_data_s);

    return 0;
}

static PLI_INT32 run_test_compiletf(PLI_BYTE8 *user_data) {
    (void)user_data;  // Suppress unused parameter warning
    return 0;
}

void register_my_systfs(void) {
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$run_test";
    tf_data.calltf      = run_test;
    tf_data.compiletf   = run_test_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;

    vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])(void) = {
    register_my_systfs,
    0
};
