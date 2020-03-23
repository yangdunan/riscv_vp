#include <cmath>
#include <iomanip>

#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_module_name n,uint32_t irq_number) : irq_number(irq_number),
     sc_module(n), tsock("tsock"), base_offset(0) {
  SC_THREAD(do_filter);

  tsock.register_b_transport(this, &SobelFilter::blocking_transport);
}

SobelFilter::~SobelFilter() = default;

void SobelFilter::do_filter() {
  		std::cout << "sobel_filter_THREAD " << std::endl;

    while (true) {


    run_event.notify(sc_core::sc_time(scaler, sc_core::SC_MS));
			std::cout << "sobel_filter run_event.notify done" << std::endl;
			std::cout << "sobel_filter wait(run_event)" << std::endl;
			sc_core::wait(run_event);  // 40 times per second by default
      std::cout << "sobel filter tigger time: " << sc_core::sc_time_stamp() << std::endl;

			std::cout << "sobel_filter wait(run_event) done" << std::endl;
    		 	std::cout << "Saaaaaaaaaaaaaa" << std::endl;

     plic->gateway_trigger_interrupt(irq_number);
  }
}

void SobelFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  wait(delay);
  sc_dt::uint64 addr = payload.get_address();
  addr = addr - base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case SOBEL_FILTER_RESULT_ADDR:
      //buffer.uint = o_result.read();
        buffer.uint = 1;
        std::cout<<"[sobel filter ]TLM_READ_COMMAND SOBEL_FILTER_RESULT_ADDR "<<std::endl;
      break;
      case SOBEL_FILTER_R_ADDR:
      //buffer.uint = o_result.read();
        buffer.uint = 2;
        std::cout<<"[sobel filter ]TLM_READ_COMMAND SOBEL_FILTER_RESULT_ADDR "<<std::endl;

      break;



    default:
      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case SOBEL_FILTER_RESULT_ADDR:
        std::cout<<"[sobel filter ]TLM_WRITE_COMMAND SOBEL_FILTER_RESULT_ADDR data =="<<(int)*data_ptr<<std::endl;

      break;

    case SOBEL_FILTER_R_ADDR:
        std::cout<<"[sobel filter ]TLM_WRITE_COMMAND SOBEL_FILTER_R_ADDR data =="<<(int)*data_ptr<<std::endl;
          break;





      
    default:
         std::cout << "[vp::sensor] transport cmd == tlm::TLM_WRITE_COMMAND) && (addr == SCALER_REG_ADDR"<< std::endl;
				run_event.cancel();
				run_event.notify(sc_core::sc_time(scaler, sc_core::SC_MS));



      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
