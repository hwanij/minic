#ifndef _PIPLINE_H_
#define _PIPLINE_H_

#include <stdint.h>
#include "cache.h"
#include "register.h"
#include "process.h"


#define PIPLINE_LEVEL	5

typedef struct{
    uint32_t inst_addr;
    uint32_t inst_code;
    int inst_type;
    int opcodes;
    uint32_t Rn, Rd, Rs, Rm;
    int imm;
    int shift;
    int rotate;
    int cond;
    int high_offset, low_offset;
    int S, A, P, U, B, W, L, H;
    uint32_t cur_inst_PC;
    uint32_t addr;
}PIPLINE_DATA;

typedef struct{
    int block;//1 means pipline block, 0 mean the opposite
    int block_reg;
    PIPLINE_DATA* pipline_data[PIPLINE_LEVEL];
    char pipline_info[PIPLINE_LEVEL][200];
    PROC_STACK* stack;
    REGISTERS* regs;
    CACHE *i_cache, *d_cache;
    PROCESS* proc;
    int drain_pipline;
    int pc_src;
    int ex_begin;
}PIPLINE;

typedef struct{
    int cycles_total;//total cycles of cpu
    int inst_total;//total instruction number
    //int cycles_idle;//idle cycles of cpu, include bubles and cache miss
    int cycles_work;//work cycles of cpu
    int bubbles;//bubbles of pipline
    int rd_mem_times;//times of read memory
    int wr_mem_times;//times of write memory
    int i_cache_visit;//times of i cache visit
    int i_cache_miss;//times of i cache miss
    int d_cache_visit;//times of d cache visit
    int d_cache_miss;//times of d cache miss
}CPU_info;
  
PIPLINE* pipline_initial(REGISTERS* regs, CACHE* i_cache,
                         CACHE* d_cache);
int pipline_destroy(PIPLINE* pipline);
int pipline_next_step(PIPLINE* pipline, CPU_info* cpu_info);
void drain_pipline(PIPLINE* pipline, int level);
int sign_extend(int imm, int size);

#endif
