#ifndef AND_GATE_VPI_WRAPPER_H
#define AND_GATE_VPI_WRAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void* create_and_gate_model();
void destroy_and_gate_model(void* model);
void set_and_gate_inputs(void* model, int32_t a, int32_t b);
int32_t get_and_gate_output(void* model);

#ifdef __cplusplus
}
#endif

#endif // AND_GATE_VPI_WRAPPER_H