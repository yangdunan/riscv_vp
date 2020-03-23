#ifndef RISCV_ISA_DMA_H
#define RISCV_ISA_DMA_H

#include <systemc>

#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <unordered_map>

struct SimpleDMA : public sc_core::sc_module {
	tlm_utils::simple_initiator_socket<SimpleDMA> isock;
	tlm_utils::simple_target_socket<SimpleDMA> tsock;

	interrupt_gateway *plic = 0;
	uint32_t irq_number = 0;

	std::array<uint8_t, 64> buffer;

	uint32_t src = 0;
	uint32_t dst = 0;
	uint32_t len = 0;
	uint32_t op = 0;
	uint32_t stat = 0;

	std::unordered_map<uint64_t, uint32_t *> addr_to_reg;

	enum {
		OP_NOP = 0,
		OP_MEMCPY = 1,
		OP_MEMSET = 2,
		OP_MEMCMP = 3,
		OP_MEMCHR = 4,
		OP_MEMMOVE = 5,
	};

	enum {
		SRC_ADDR = 0,
		DST_ADDR = 4,
		LEN_ADDR = 8,
		OP_ADDR = 12,
		STAT_ADDR = 16,
	};

	sc_core::sc_event run_event;

	SC_HAS_PROCESS(SimpleDMA);

	SimpleDMA(sc_core::sc_module_name, uint32_t irq_number) : irq_number(irq_number) {
		tsock.register_b_transport(this, &SimpleDMA::transport);

		SC_THREAD(run);
			/*hint*/
		//std::cout<<"SC_HAS_PROCESS(SimpleDMA); "<<std::endl;
		addr_to_reg = {
		    {SRC_ADDR, &src},
		    {DST_ADDR, &dst},
		    {LEN_ADDR, &len},
		    {OP_ADDR, &op},
		};
	}

	void _copy_block(uint32_t off, uint32_t n) {

		//std::cout << "DMA::__copy_block" << std::endl;
		do_transaction(tlm::TLM_READ_COMMAND, src + off, &buffer[0], n);
		do_transaction(tlm::TLM_WRITE_COMMAND, dst + off, &buffer[0], n);
	}

	void _perform_memcpy() {
		//std::cout << "DMA::_perform_memcpy()" << std::endl;
		auto n = len;
		uint32_t off = 0;

		while (n > buffer.size()) {
			_copy_block(off, buffer.size());
			n -= buffer.size();
			off += buffer.size();
		}

		_copy_block(off, n);
	}

	void run() {
				/*hint*/
		//std::cout << "Simple_DMA_THREAD " << std::endl;
		while (true) {
			//std::cout << "Simple_DMA_THREAD sc_core::wait(run_event);" << std::endl;
			sc_core::wait(run_event);
			//std::cout << "simple DMA tigger time: " << sc_core::sc_time_stamp() << std::endl;

			//std::cout << "Simple_DMA_THREAD sc_core::wait(run_event) done" << std::endl;

			switch (op) {
				case OP_NOP:
					//std::cout << "Simple_DMA_THREAD case OP_NOP" << std::endl;
					break;

				case OP_MEMCPY:
					//std::cout << "Simple_DMA_THREAD case OP_MEMCPY" << std::endl;
					_perform_memcpy();
					break;

				case OP_MEMSET:
					//std::cout << "Simple_DMA_THREAD case OP_MEMSET" << std::endl;					
					assert(false && "not implemented");
					break;

				case OP_MEMCMP:
					//std::cout << "Simple_DMA_THREAD case OP_MEMCMP" << std::endl;					
					assert(false && "not implemented");
					break;

				case OP_MEMCHR:
					//std::cout << "Simple_DMA_THREAD case OP_MEMCHR" << std::endl;					
					assert(false && "not implemented");
					break;

				case OP_MEMMOVE:
					//std::cout << "Simple_DMA_THREAD case OP_MEMMOVE" << std::endl;
					assert(false && "not implemented");
					break;

				default:
					//std::cout << "Simple_DMA_THREAD case default" << std::endl;
					assert(false && "unknown operation requested by software");
			}

			plic->gateway_trigger_interrupt(irq_number);
		}
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		//std::cout << "DMA transport" << std::endl;

		auto addr = trans.get_address();
		auto cmd = trans.get_command();
		auto len = trans.get_data_length();
		auto ptr = trans.get_data_ptr();

		assert(len == 4);  // NOTE: only allow to read/write whole register

		auto it = addr_to_reg.find(addr);
		assert(it != addr_to_reg.end());  // access to non-mapped address

		// actual read/write
		if (cmd == tlm::TLM_READ_COMMAND) {
			*((uint32_t *)ptr) = *it->second;
		} else if (cmd == tlm::TLM_WRITE_COMMAND) {
			*it->second = *((uint32_t *)ptr);
		} else {
			assert(false && "unsupported tlm command for dma access");
		}

		// post read/write actions
		if ((cmd == tlm::TLM_WRITE_COMMAND) && (addr == OP_ADDR)) {
			run_event.notify(sc_core::sc_time(10, sc_core::SC_NS));
		}

		(void)delay;  // zero delay
	}

	void do_transaction(tlm::tlm_command cmd, uint64_t addr, uint8_t *data, unsigned num_bytes) {
		sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
		//std::cout << "DMA do_transaction" << std::endl;
		tlm::tlm_generic_payload trans;
		trans.set_command(cmd);
		trans.set_address(addr);
		trans.set_data_ptr(data);
		trans.set_data_length(num_bytes);

		isock->b_transport(trans, delay);

		if (delay != sc_core::SC_ZERO_TIME)
			sc_core::wait(delay);
	}
};

#endif  // RISCV_ISA_DMA_H
