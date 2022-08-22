#include "cpp_vcd_tracer/src/vcd_tracer.cpp"

#include"softfloat_types.h"

namespace vcd_tracer { 
    volatile vcd_tracer::scope_fn::sequence_t trace_seq = 0;

    template<> void value_base::dump<float128_t>(std::ostream &out, 
                                                 const size_t bit_size, 
                                                 const value_state state, 
                                                 const float128_t value) const {
    }
}

