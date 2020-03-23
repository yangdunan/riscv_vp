#ifndef RISCV_ISA_BASIC_TIMER_H
#define RISCV_ISA_BASIC_TIMER_H

#include <systemc>

#include <tlm_utils/simple_target_socket.h>

#include "core/common/irq_if.h"

struct BasicTimer : public sc_core::sc_module {
	interrupt_gateway *plic = 0;
	uint32_t irq_number = 0;

	SC_HAS_PROCESS(BasicTimer);

	BasicTimer(sc_core::sc_module_name, uint32_t irq_number) : irq_number(irq_number) {
		SC_THREAD(run);
		/*hint*/
		//std::cout<<"SC_HAS_PROCESS(BasicTimer); "<<std::endl;
	}

	void run() {
				/*hint*/
		//std::cout << "Basic_Timer_THREAD " << std::endl;
		while (true) {
			//std::cout << "Basic_Timer_THREAD wait(sc_core::sc_time(1, sc_core::SC_MS))" << std::endl;
			sc_core::wait(sc_core::sc_time(1, sc_core::SC_MS));
      		//std::cout << "Basic_Timer tigger time: " << sc_core::sc_time_stamp() << std::endl;
			//std::cout << "Basic_Timer_THREAD wait(sc_core::sc_time(1, sc_core::SC_MS)) done" << std::endl;
			plic->gateway_trigger_interrupt(irq_number);
		}
	}
};

#endif  // RISCV_ISA_BASIC_TIMER_H
