#ifndef RISCV_ISA_MEMORY_H
#define RISCV_ISA_MEMORY_H

#include <systemc>
// #include <tlm>
#include <scml2.h>


template <typename T>
class MemoryModel : public sc_module {
public:
	/* Port list */
	// tlm::tlm_target_socket<32> t_socket;

	/* Memory bank */
	scml2::memory<T> memory_bank;
	
	
	uint8_t *data;
	uint32_t size;
	//bool read_only;
	
	
	/* Constructor */
	SC_HAS_PROCESS(MemoryModel);
	MemoryModel(const sc_module_name &name,
	            unsigned long long size,
	            const sc_time &read_latency = sc_time(0, SC_NS),
	            const sc_time &write_latency = sc_time(0, SC_NS));

	/* Destructor */
	~MemoryModel();

private:
	/* Memory bank */
	// scml2::memory<T> memory_bank;

	/* Port adapter */
	// scml2::tlm2_gp_target_adapter<32> t_adapter;
};

#endif /*MEMORY_MODEL_H*/
















/*
#include <stdint.h>
#include <boost/iostreams/device/mapped_file.hpp>
#include <iostream>

#include "bus.h"

#include <tlm_utils/simple_target_socket.h>
#include <systemc>

struct SimpleMemory : public sc_core::sc_module {
	tlm_utils::simple_target_socket<SimpleMemory> tsock;
	
	uint8_t *data;
	uint32_t size;
	bool read_only;

	SimpleMemory(sc_core::sc_module_name, uint32_t size, bool read_only = false)
	    : data(new uint8_t[size]()), size(size), read_only(read_only) {
		tsock.register_b_transport(this, &SimpleMemory::transport);
		tsock.register_get_direct_mem_ptr(this, &SimpleMemory::get_direct_mem_ptr);
		tsock.register_transport_dbg(this, &SimpleMemory::transport_dbg);
	}

	void load_binary_file(const std::string &filename, unsigned addr) {
		boost::iostreams::mapped_file_source f(filename);
		assert(f.is_open());
		write_data(addr, (const uint8_t *)f.data(), f.size());
	}

	void write_data(unsigned addr, const uint8_t *src, unsigned num_bytes) {
		assert(addr + num_bytes <= size);

		memcpy(data + addr, src, num_bytes);
	}

	void read_data(unsigned addr, uint8_t *dst, unsigned num_bytes) {
		assert(addr + num_bytes <= size);

		memcpy(dst, data + addr, num_bytes);
	}

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay) {
		transport_dbg(trans);
		delay += sc_core::sc_time(10, sc_core::SC_NS);
	}

	unsigned transport_dbg(tlm::tlm_generic_payload &trans) {
		tlm::tlm_command cmd = trans.get_command();
		unsigned addr = trans.get_address();
		auto *ptr = trans.get_data_ptr();
		auto len = trans.get_data_length();

		assert(addr < size);

		if (cmd == tlm::TLM_WRITE_COMMAND) {
			write_data(addr, ptr, len);
		} else if (cmd == tlm::TLM_READ_COMMAND) {
			read_data(addr, ptr, len);
		} else {
			sc_assert(false && "unsupported tlm command");
		}

		return len;
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi) {
		(void)trans;
		dmi.set_start_address(0);
		dmi.set_end_address(size);
		dmi.set_dmi_ptr(data);
		if (read_only)
			dmi.allow_read();
		else
			dmi.allow_read_write();
		return true;
	}
};

#endif  // RISCV_ISA_MEMORY_H
*/