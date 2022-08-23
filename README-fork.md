Spike RISC-V ISA Simulator Fork
===============================

About
-----

This fork adds a few features.

- Tracing of registers and memory bus.
- Time limit for simulation.
- New commands to trace variables.
- Two way symbol- address lookup.


New Options
-----------

~~~
  --vcd-log=<file>      Log VCD to this file.
  --max-cycles=<cycle count>      Limit simulation to this number of cycles.
~~~

New Interpreter Commands
------------------------

~~~
echo <on|off>
interrupt <core>  <raise|clear> [mei|mti|msi] 
trace <name> <addr> <size> 
trace <var>                
~~~

### echo <on|off>

Interpreter commands will be echo'ed to the console.

This allows log output to be examined with the commands included as
part of the log.

### interrupt

- core: Select the HART to apply the interrupt to.
- <raise|clear> : Raise or clear the interrupt at the source.
- [mei|mti|msi] : Select the type of interrupt.

### trace <name> <addr> <size>

Trace a variable. Provide the name, address and size to trace. The size will determine the number of bytes to trace and the type of integer conversion.

Any bus access to the given address will be reported to the console.

The variable will be traced to the VCD log file if the trace command
is issued before the simulation is started.

### trace <var>                

Trace a variable. The symbol will be looked up and traced according to the above command.

VCD Logging
-----------

Trace the processor internals with
[VCD](https://en.wikipedia.org/wiki/Value_change_dump). This can be
viewed with [GTKWave](https://github.com/gtkwave/gtkwave) and many
commercial tools.

The following are traced:

- PC
- Opcode
- Core registers
- CSR registers
- Memory bus read/write access
- Machine mode interrupts (msi, mti, mei)

