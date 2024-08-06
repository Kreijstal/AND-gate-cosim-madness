// and_gate_vpi_wrapper.h
#ifndef AND_GATE_VPI_WRAPPER_H
#define AND_GATE_VPI_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

void* create_and_gate_model();
void destroy_and_gate_model(void* model);
void set_and_gate_inputs(void* model, int a, int b);
int get_and_gate_output(void* model);

#ifdef __cplusplus
}
#endif

#endif // AND_GATE_VPI_WRAPPER_H
