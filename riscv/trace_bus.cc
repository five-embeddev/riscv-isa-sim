#include "processor.h"

trace_location_t::trace_location_t(const std::string &name, reg_t addr, int size, bool trace_as_real) 
    : trace_var(vcd_tracer::make_value(size, trace_as_real))
    , name(name)
    , addr(addr)
    , size(size)
    , mask(size==sizeof(reg_t) ? -1 : ((1ul<<(size*8))-1))
    , value(0)
    , trace_as_real(trace_as_real)
{
}

void trace_location_t::trace(std::ostream &out, reg_t wr_addr, reg_t wr_val, int wr_size) {
    int offset = wr_addr - addr;
    reg_t trace_val = (wr_val << (offset*8)) & mask;
    reg_t wr_mask = wr_size==8 ? 0xFFFFFFFFFFFFFFFFull : ((1ull<<(wr_size*8))-1);
    reg_t trace_mask = (wr_mask << (offset*8)) & mask;
    value = trace_val & trace_mask | value & ~trace_mask;
    out << "TRACE: " << name 
        << " @ 0x" << std::hex << addr << " + 0x" << offset
        << "= 0x" << wr_val
        << "-> 0x" <<  value
        << " & 0x" << trace_mask
        << std::dec << "\n";
    if (trace_as_real) {
        double real_val;
        if (size==8) {
            real_val = reinterpret_cast<double*>(&value)[0];
        } else {
            real_val = reinterpret_cast<float*>(&value)[0];
        }
        trace_var->set_double(real_val);
    } else {
        trace_var->set_uint64(value);
    }
}

// Trace memory access like a memory bs
void trace_bus_t::clear_strobe(void) {
    if (wr_strobe_cnt > 0) {
        wr_strobe_cnt --;
        if (wr_strobe_cnt == 0) {
            wr_strobe.set(false);
        }
    }
    if (rd_strobe_cnt > 0) {
        rd_strobe_cnt --;
        if (rd_strobe_cnt == 0) {
            rd_strobe.set(false);
        }
    }
}
void trace_bus_t::mem_write( reg_t addr, uint64_t val, uint8_t size) {
    addr &= addr_mask;
    
    trace_addr.set(addr);
    trace_wr_data.set(val);
    trace_size.set(size);
    wr_strobe.set(true);
    wr_strobe_cnt ++;

    auto i = std::lower_bound(_trace_vars.begin(),
                              _trace_vars.end(),
                              addr,
                              [](const trace_location_t &var, reg_t addr) -> bool {
                                  // Does the first value go before the second?
                                  return var.addr < (addr - var.size);
                              });    
    if (i != _trace_vars.end())  {        
        do {
            if ((addr >= i->addr) && (addr<(i->addr + i->size))) {
                i->trace(sout, addr, val, size);
            }
            i++;
        } while (i!=_trace_vars.end() && (i->addr < (addr + size)));
    }
}
void trace_bus_t::mem_read( reg_t addr, uint64_t val, uint8_t size) {
    trace_addr.set(addr);
    trace_rd_data.set(val);
    trace_size.set(size);
    rd_strobe.set(true);
    rd_strobe_cnt ++;
}
void trace_bus_t::add_trace(const std::string &name, reg_t addr, int size, bool trace_as_real) {
    auto i = std::lower_bound(_trace_vars.begin(),
                              _trace_vars.end(),
                              addr,
                              [](const trace_location_t &x, reg_t addr) {
                                  // Does the first value go before the second?
                                  return x.addr < addr;
                              });
    _trace_vars.emplace(i, name, addr, size, trace_as_real);
}

