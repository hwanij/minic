#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>
#include "mem.h"

//All macro is in Byte
#define CACHE_SIZE		1024
#define CACHE_BLOCK_SIZE	32
#define CACHE_NUM		1
#define WORD_WIDTH		32
#define CACHE_MISSED_CYCLE	8
#define CACHE_HIT_CYCLE		1
#define DATA_W      0
#define DATA_HW     1
#define DATA_B      2

typedef struct{
    int block_num;
    int sign_bits_num;
    PROC_MEM* mem;
    int valid[CACHE_SIZE/CACHE_BLOCK_SIZE];
    uint8_t data[CACHE_SIZE/CACHE_BLOCK_SIZE][CACHE_BLOCK_SIZE];
    uint32_t mark[CACHE_SIZE/CACHE_BLOCK_SIZE];
}CACHE;

typedef struct{
    int cpu_cycles;
    int32_t data;
}CACHE_RETURN;

CACHE* cache_initial(PROC_MEM* _mem);
int cache_mem_link(CACHE* cache, PROC_MEM* _mem);
CACHE_RETURN cache_search(CACHE* cache, uint32_t addr);
int cache_write(CACHE* cache, uint32_t addr, uint32_t data, int data_type);
int cache_destroy(CACHE* cache);
int cache_update(CACHE* cache, uint32_t addr);

#endif
