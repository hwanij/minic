#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "CPU.h"
#include <stdint.h>

#define BYTE	0
#define HWORD 1
#define WORD	2

extern void debugger_print_cpu_info(CPU_info* cpu_info);
extern void debugger_print_register(REGISTERS* regs, int reg_num);
extern void debugger_print_CMSR(REGISTERS* regs);
extern void debugger_modify_register(REGISTERS* regs, int reg_num, int32_t content);
extern void debugger_print_mem(PROC_MEM* mem, uint32_t addr);
extern void debugger_modify_mem(PROC_MEM* mem, uint32_t addr, int32_t content);
extern void debugger_print_stack(PROC_MEM* mem, uint32_t sp);
extern void debugger_modify_stack(PROC_MEM* mem, uint32_t sp,
                                  uint32_t addr, int32_t content);
extern int debugger_search_symtbl(PROCESS* proc, uint32_t addr, char* result,
                                  int type);
extern uint32_t debugger_search_symtbl_func(PROCESS* proc, char* func_name);
void debugger_print_inst_statistic(PROCESS* proc);

#endif
