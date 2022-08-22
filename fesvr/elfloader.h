// See LICENSE for license details.

#ifndef _ELFLOADER_H
#define _ELFLOADER_H

#include "memif.h"
#include <map>
#include <string>

struct elf_symbol_t {
    uint64_t addr;
    uint64_t size;    
} ;

class memif_t;
// Name, address, size
std::map<std::string, elf_symbol_t> load_elf(const char* fn, memif_t* memif, reg_t* entry);


#endif
