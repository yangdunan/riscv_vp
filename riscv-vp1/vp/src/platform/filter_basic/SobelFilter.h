#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#include "filter_def.h"

#include "core/common/irq_if.h"

class SobelFilter : public sc_module {
public:
/*for plic */
	interrupt_gateway *plic = 0;
	uint32_t irq_number = 0;
	int scaler=10;
  sc_event run_event;
/*for plic*/

  tlm_utils::simple_target_socket<SobelFilter> tsock;

  sc_fifo<unsigned char> i_r;
  sc_fifo<unsigned char> i_g;
  sc_fifo<unsigned char> i_b;
  sc_fifo<int> o_result;

  SC_HAS_PROCESS(SobelFilter);
  SobelFilter(sc_module_name n,uint32_t irq_number);
  ~SobelFilter();

private:
  void do_filter();
  int val[1];

  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
