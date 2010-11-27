#include "basic_block.h"
#include <stdlib.h>
#include <memory.h>
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
basic_block *bblist;
func_block *fblist;
int block_num;

int gen_basic_block()
{
	int i, k, m;
	basic_block *cur_block;
	func_block *cur_func;
	enum operator tmp_op;
	int *isLeader = (int*)malloc(triple_list_index*sizeof(int));
	memset(isLeader, 0, triple_list_index*sizeof(int));
	block_num = 0;
	for (i = 0; i < triple_list_index; i++)
	{
		tmp_op = triple_list[index_index[i]].op;
		if (tmp_op == enterF && isLeader[i] == 0)
		{
			block_num++;
			triple_list[index_index[i]].block = (basic_block*)malloc(sizeof(basic_block));
			isLeader[i] = 1;
		}
		if (tmp_op == if_op || tmp_op == if_not_op || tmp_op == goto_op)
		{
			if (tmp_op == goto_op)
				k = triple_list[index_index[i]].arg1.temp_index;
			else
				k = triple_list[index_index[i]].arg2.temp_index;
			if (isLeader[k] == 0)
			{
				block_num++;
				triple_list[index_index[k]].block = (basic_block*)malloc(sizeof(basic_block));
			}
			isLeader[k] = 1;
			if (i + 1 < triple_list_index)
			{
				if (isLeader[i+1] == 0)
				{
					block_num++;
					triple_list[index_index[i+1]].block = (basic_block*)malloc(sizeof(basic_block));
				}
				isLeader[i+1] = 1;
			}
		}
	}
	fblist = (func_block*)malloc(sizeof(func_block));
	bblist = (basic_block*)malloc(sizeof(basic_block));
	cur_func = fblist;
	cur_func->prev = NULL;
	cur_func->start = triple_list[index_index[0]].block;
	m = 0;
	cur_block = bblist;
	cur_block->begin = 0;
	cur_block->prev = NULL;
	cur_block->m = m;
	for (i = 1; i < triple_list_index; i++)
	{
		if (isLeader[i] == 1)
		{
			cur_block->end = i-1;
			if (triple_list[index_index[i-1]].op != goto_op && triple_list[index_index[i-1]].op != leaveF)
				cur_block->follow = triple_list[index_index[i]].block;
			else
				cur_block->follow = NULL;				
			tmp_op = triple_list[index_index[i-1]].op;
			if (tmp_op == if_op || tmp_op == if_not_op || tmp_op == goto_op)
			{
				if (tmp_op == goto_op)
					k = triple_list[index_index[i-1]].arg1.temp_index;
				else
					k = triple_list[index_index[i-1]].arg2.temp_index;
				cur_block->jump = triple_list[index_index[k]].block;
			}
			else
				cur_block->jump = NULL;
			cur_block->next = triple_list[index_index[i]].block;
			triple_list[index_index[i]].block->prev = cur_block;
			cur_block = triple_list[index_index[i]].block;
			cur_block->begin = i;
			cur_block->m = ++m;
		}
		if (triple_list[index_index[i]].op == enterF)
		{
			cur_func->bb_num = m;
			cur_func->over = cur_block->prev;
			cur_func->next = (func_block*)malloc(sizeof(func_block));
			cur_func->next->prev = cur_func;
			cur_func = cur_func->next;
			cur_func->start = cur_block;
			m = 0;
		}
	}
	cur_block->end = triple_list_index-1;
	cur_block->next = NULL;
	cur_block->follow = NULL;
	cur_func->next = NULL;
	cur_func->over = cur_block;
	cur_func->bb_num = m+1;
	return 0;
}
