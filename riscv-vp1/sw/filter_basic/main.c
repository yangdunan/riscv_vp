#include <stdint.h>
#include "irq.h"

static volatile char * const TERMINAL_ADDR = (char * const)0x20000000;
static volatile char * const SENSOR_INPUT_ADDR = (char * const)0x50000000;
static volatile uint32_t * const SENSOR_SCALER_REG_ADDR = (uint32_t * const)0x50000080;
static volatile uint32_t * const SENSOR_FILTER_REG_ADDR = (uint32_t * const)0x50000084;
static volatile uint32_t * const SOBEL_FILTER_R_ADDR = (uint32_t * const) 0x80000000;
static volatile uint32_t * const SOBEL_FILTER_RESULT_ADDR =(uint32_t * const) 0x80000004;
_Bool has_sensor_data = 0;

void sensor_irq_handler() {
	has_sensor_data = 1;
}

void dump_sensor_data() {
	while (!has_sensor_data) {
		asm volatile ("wfi");
	}
	has_sensor_data = 0;
	
	for (int i=0; i<1; ++i) {
		//*TERMINAL_ADDR = *(SENSOR_INPUT_ADDR + i) % 92 + 32;
		*TERMINAL_ADDR = *(SOBEL_FILTER_R_ADDR + i) % 92 + 32;

	}
	*TERMINAL_ADDR = '\n';
}

int main() {	
	register_interrupt_handler(2, sensor_irq_handler);
	
	//*SENSOR_SCALER_REG_ADDR = 5;
	//*SENSOR_FILTER_REG_ADDR = 2;
	*SOBEL_FILTER_R_ADDR = 5;
	*SOBEL_FILTER_RESULT_ADDR = 2;
	for (int i=0; i<1; ++i)
		dump_sensor_data();

	return 0;
}
