#ifndef VCD_TRACE_H
#define VCD_TRACE_H

#include "cpp_vcd_tracer/src/vcd_tracer.hpp"

#include "softfloat_types.h"

namespace vcd_tracer { 

    static constexpr int PC_TRACE_DEPTH=5000; // Should match sim_t::INTERLEAVE.
    static constexpr int REG_TRACE_DEPTH=2000; 
    static constexpr int CSR_TRACE_DEPTH=500; 
    static constexpr int BUS_TRACE_WIDTH=32; 

    extern  vcd_tracer::scope_fn::sequence_t trace_seq;

    template<>
    inline bool sample_changed(const float128_t new_sample, const float128_t prev_sample) {
        // TODO
        return true;
    }


    template<typename T> using sim_pc_value = value<T, vcd_tracer::bit_size<T>::value, PC_TRACE_DEPTH, &trace_seq>;
    template<typename T> using sim_reg_value = value<T, vcd_tracer::bit_size<T>::value, REG_TRACE_DEPTH, &trace_seq>;
    template<typename T> using sim_csr_value = value<T, vcd_tracer::bit_size<T>::value, CSR_TRACE_DEPTH, &trace_seq>;

    static std::unique_ptr<vcd_tracer::value_base> make_value(int size_bytes) {
        switch (size_bytes) {
        case 1:
            return std::make_unique<sim_reg_value<uint8_t>>();
        case 2:
            return std::make_unique<sim_reg_value<uint16_t>>();
        case 3:
        case 4:
            return std::make_unique<sim_reg_value<uint32_t>>();
        case 5:
        case 6:
        case 7:
        case 8:
            return std::make_unique<sim_reg_value<uint64_t>>();
        }
        return std::make_unique<sim_reg_value<uint64_t>>();
    }
}

static std::ostream &operator<<(std::ostream &out, const float128_t) {
    // NOP
    return out;
}

#endif

