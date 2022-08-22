reg_t tmp = npc;
set_pc(JUMP_TARGET, true);
WRITE_RD(tmp);
