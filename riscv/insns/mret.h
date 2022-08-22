require_privilege(PRV_M);
reg_t next_pc = p->get_state()->mepc->read();
p->get_state()->trace_jump_branch_pc(next_pc, true);
set_pc_and_serialize(next_pc);
reg_t s = STATE.mstatus->read();
reg_t prev_prv = get_field(s, MSTATUS_MPP);
reg_t prev_virt = get_field(s, MSTATUS_MPV);
if (prev_prv != PRV_M)
  s = set_field(s, MSTATUS_MPRV, 0);
s = set_field(s, MSTATUS_MIE, get_field(s, MSTATUS_MPIE));
s = set_field(s, MSTATUS_MPIE, 1);
s = set_field(s, MSTATUS_MPP, PRV_U);
s = set_field(s, MSTATUS_MPV, 0);
p->set_csr(CSR_MSTATUS, s);
p->set_privilege(prev_prv);
p->set_virt(prev_virt);
