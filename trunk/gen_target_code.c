#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "basic_block.h"
#include "register.h"
#include "gen_intermediate_code.h"
extern triple *triple_list;
extern int *index_index;
extern int triple_list_index;
extern func_block *fblist;
FILE *out;
assemble *assemble_list;
int assemble_size;
int assemble_num;
int temp_reg_var[32];

int ifgoto_code(func_block *fb, int i);
int if_not_code(func_block *fb, int i);
int goto_code(func_block *fb, int i);
int negative_code(func_block *fb, int i);
int not_code(func_block *fb, int i);
int address_code(func_block *fb, int i);
int star_code(func_block *fb, int i);
int positive_code(func_block *fb, int i);
int assign_code(func_block *fb, int i);
int star_assign_code(func_block *fb, int i);
int add_code(func_block *fb, int i);
int minus_code(func_block *fb, int i);
int multiply_code(func_block *fb, int i);
int char_to_int_code(func_block *fb, int i);
int equal_code(func_block *fb, int i);
int less_code(func_block *fb, int i);
int larger_code(func_block *fb, int i);
int eqlarger_code(func_block *fb, int i);
int eqless_code(func_block *fb, int i);
int noteq_code(func_block *fb, int i);
int or_code(func_block *fb, int i);
int and_code(func_block *fb, int i);
int get_rb_code(func_block *fb, int i);
int set_rb_code(func_block *fb, int i);
int call_code(func_block *fb, int i);
int param_code(func_block *fb, int i);
int enterF_code(func_block *fb, int i);
int enterS_code(func_block *fb, int i);
int leaveF_code(func_block *fb, int i);
int leaveS_code(func_block *fb, int i);
int rtn_code(func_block *fb, int i);
int adds_code(func_block *fb, int i);

int int_to_char_code(func_block *fb, int i);
int (*g[33])(func_block*, int) = {
	ifgoto_code, if_not_code, goto_code, negative_code, not_code, address_code, star_code, positive_code, assign_code, star_assign_code, add_code, 	minus_code, multiply_code, char_to_int_code, equal_code, less_code, larger_code, eqlarger_code, eqless_code, noteq_code, or_code, and_code, 	get_rb_code, set_rb_code, call_code, param_code, enterF_code, enterS_code, leaveF_code, leaveS_code, rtn_code, adds_code, int_to_char_code
};
int initial();
int memory_allocation();
int setLabel();
int convert();
int add_assemble(char *label, enum instruction ins, int Rn, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm);
int add_std_assemble(func_block *fb, enum instruction ins, int u0, int u1, int u2);
int add_imm_assemble(func_block *fb, enum instruction ins, int u0, int u1, int imm);
int gen_target_code()
{
	out = fopen("assemble_code.s", "w");
	initial();
	memory_allocation();
	setLabel();
	convert();
	fclose(out);
	return 0;
}

int initial()
{
	int i;
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		for (i = 0; i < 32; i++)
			fb->reg_var[i] = -1;
	}
	return 0;
}

int memory_allocation()
{
	int i, off, para_num;
	func_block *fb;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		para_num = (triple_list[index_index[fb->start->begin]].symtbl)->para_num;
		for (i = para_num-5, off = 4; i >= 0; i--, off += 4)
			fb->uni_table[i]->offset = off;
		if (fb->reg_used < 10)
			off = -16;
		else
			off = -16 - (fb->reg_used - 9) * 4;
		for (i = (para_num<4)?0:para_num-4; i < fb->uni_item_num; i++)
		{
			if (!(fb->mapping[i].isTmp == 1 && fb->reg_alloc[i] != -1))
			{
				fb->uni_table[i]->offset = off;
				if (fb->uni_table[i]->size == -1)
					off -= 4;
				else
					off -= 4*fb->uni_table[i]->size;
			}
		}
		fb->min_stack_size = -off;
	}
	return 0;
}

int setLabel()
{
	int i, j, l;
	for (i = 0; i < triple_list_index; i++)
		triple_list[index_index[i]].label = 0;
	for (i = 0, l = 1; i < triple_list_index; i++)
	{
		if (triple_list[index_index[i]].op == goto_op)
		{
			j = triple_list[index_index[i]].arg1.temp_index;
			if (triple_list[index_index[j]].label == 0)
				triple_list[index_index[j]].label = l++;
		}
		if (triple_list[index_index[i]].op == if_op || triple_list[index_index[i]].op == if_not_op)
		{
			j = triple_list[index_index[i]].arg2.temp_index;
			if (triple_list[index_index[j]].label == 0)
				triple_list[index_index[j]].label = l++;
		}
	}
	return 0;
}

int convert()
{
	func_block *fb;
	int i;
	char temp[16];
	assemble_list = (assemble*)malloc(64*sizeof(assemble));
	assemble_size = 64*sizeof(assemble);
	assemble_num = 0;
	for (fb = fblist; fb != NULL; fb = fb->next)
	{
		add_assemble((triple_list[index_index[fb->start->begin]].symtbl)->func_name, label, -1, -1, 0, -1, 0, -1);
		//fprintf(out, "%s:\n", (triple_list[index_index[fb->start->begin]].symtbl)->func_name);
		for (i = fb->start->begin; i <= fb->over->end; i++)
		{
			if (triple_list[index_index[i]].label != 0)
			{
				sprintf(temp, ".L%d", triple_list[index_index[i]].label);
				add_assemble(name_address(temp), label, -1, -1, 0, -1, 0, -1);	
				//fprintf(out, ".L%d:\n", triple_list[index_index[i]].label);
			}
			g[triple_list[index_index[i]].op-3000](fb, i);
		}
	}
	return 0;
}

int add_assemble(char *label, enum instruction ins, int Rn, int Rd, int Rs_or_Imm, int Rs_Imm, int Rm_or_Imm, int Rm_Imm)
{
	if ((assemble_num+1)*sizeof(assemble) > assemble_size)
		adjustSize((void**)&assemble_list, &assemble_size);
	assemble_list[assemble_num].label = label;
	assemble_list[assemble_num].ins = ins;
	assemble_list[assemble_num].Rn = Rn;
	assemble_list[assemble_num].Rd = Rd;
	assemble_list[assemble_num].Rs_or_Imm = Rs_or_Imm;
	assemble_list[assemble_num].Rs_Imm = Rs_Imm;
	assemble_list[assemble_num].Rm_or_Imm = Rm_or_Imm;
	assemble_list[assemble_num].Rm_Imm = Rm_Imm;
	assemble_num++;
	return 0;
}

int add_std_assemble(func_block *fb, enum instruction ins, int u0, int u1, int u2)
{
	int r0, r1, r2;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	r2 = fb->reg_alloc[u2];
	if (r1 == -1)
	{
		r1 = 4;
		add_assemble(NULL, ldw, 27, r1, 0, -1, 1, fb->uni_table[u1]->offset);
	}
	else
	{
		if (fb->reg_var[r1] != u1)
		{
			/*if (fb->reg_var[r1] != -1)
				add_assemble(NULL, stw, 27, r1, 0, -1, 1, fb->uni_table[fb->reg_var[r1]]->offset);*/
			fb->reg_var[r1] = u1;
			add_assemble(NULL, ldw, 27, r1, 0, -1, 1, fb->uni_table[u1]->offset);
		}
	}
	if (r2 == -1)
	{
		r1 = 5;
		add_assemble(NULL, ldw, 27, r2, 0, -1, 1, fb->uni_table[u2]->offset);
	}
	else
	{
		if (fb->reg_var[r2] != u2)
		{
			/*if (fb->reg_var[r2] != -1)
				add_assemble(NULL, stw, 27, r2, 0, -1, 1, fb->uni_table[fb->reg_var[r2]]->offset);*/
			fb->reg_var[r2] = u2;
			add_assemble(NULL, ldw, 27, r2, 0, -1, 1, fb->uni_table[u2]->offset);
		}
	}
	if (r0 == -1)
	{
		r0 = 4;
		add_assemble(NULL, ins, r1, r0, 0, -1, 0, r2);
		add_assemble(NULL, stw, 27, r0, 0, -1, 1, fb->uni_table[u0]->offset);
	}
	else
	{
		/*if (fb->reg_var[r0] != u0 && fb->reg_var[r0] != -1)
		{
			printf("sb!!!!!!!!!!!!!!!!!!!\n");
			add_assemble(NULL, stw, 27, r0, 0, -1, 1, fb->uni_table[fb->reg_var[r0]]->offset);
		}*/
		fb->reg_var[r0] = u0;
		add_assemble(NULL, ins, r1, r0, 0, -1, 0, r2);
	}
	return 0;
}

int add_imm_assemble(func_block *fb, enum instruction ins, int u0, int u1, int imm)
{
	int r0, r1;
	r0 = fb->reg_alloc[u0];
	r1 = fb->reg_alloc[u1];
	if (r1 == -1)
	{
		r1 = 4;
		add_assemble(NULL, ldw, 27, r1, 0, -1, 1, fb->uni_table[u1]->offset);
	}
	else
	{
		if (fb->reg_var[r1] != u1)
		{
			/*if (fb->reg_var[r1] != -1)
				add_assemble(NULL, stw, 27, r1, 0, -1, 1, fb->uni_table[fb->reg_var[r1]]->offset);*/
			fb->reg_var[r1] = u1;
			add_assemble(NULL, ldw, 27, r1, 0, -1, 1, fb->uni_table[u1]->offset);
		}
	}
	if (r0 == -1)
	{
		r0 = 4;
		add_assemble(NULL, ins, r1, r0, 0, -1, 1, imm);
		add_assemble(NULL, stw, 27, r0, 0, -1, 1, fb->uni_table[u0]->offset);
	}
	else
	{
		/*if (fb->reg_var[r0] != u0 && fb->reg_var[r0] != -1)
			add_assemble(NULL, stw, 27, r0, 0, -1, 1, fb->uni_table[fb->reg_var[r0]]->offset);*/
		fb->reg_var[r0] = u0;
		add_assemble(NULL, ins, r1, r0, 0, -1, 1, imm);	
	}
	return 0;
}

int ifgoto_code(func_block *fb, int i)
{
	return 0;
}
int if_not_code(func_block *fb, int i)
{
	return 0;
}
int goto_code(func_block *fb, int i)
{
	return 0;
}
int negative_code(func_block *fb, int i)
{
	return 0;
}
int not_code(func_block *fb, int i)
{
	return 0;
}
int address_code(func_block *fb, int i)
{
	return 0;
}
int star_code(func_block *fb, int i)
{
	return 0;
}
int positive_code(func_block *fb, int i)
{
	return 0;
}
int assign_code(func_block *fb, int i)
{
	
	return 0;
}
int star_assign_code(func_block *fb, int i)
{
	return 0;
}
int add_code(func_block *fb, int i)
{
	int type1, type2, tmp_uni, arg1_uni, arg2_uni;
	type1 = triple_list[index_index[i]].arg1_type;
	type2 = triple_list[index_index[i]].arg2_type;
	tmp_uni = triple_list[index_index[i]].tmp_uni;
	arg1_uni = triple_list[index_index[i]].arg1_uni;
	arg2_uni = triple_list[index_index[i]].arg2_uni;
	if (type1 == 0 && type2 == 0)
		add_std_assemble(fb, add, tmp_uni, arg1_uni, arg2_uni);
	else
	{
		if (type1 == 0)
			add_imm_assemble(fb, add, tmp_uni, arg1_uni, triple_list[index_index[i]].arg2.imm_value);
		if (type2 == 0)
			add_imm_assemble(fb, add, tmp_uni, arg2_uni, triple_list[index_index[i]].arg1.imm_value);
	}
	return 0;
}
int minus_code(func_block *fb, int i)
{
	return 0;
}
int multiply_code(func_block *fb, int i)
{
	return 0;
}
int char_to_int_code(func_block *fb, int i)
{
	return 0;
}
int equal_code(func_block *fb, int i)
{
	return 0;
}
int less_code(func_block *fb, int i)
{
	return 0;
}
int larger_code(func_block *fb, int i)
{
	return 0;
}
int eqlarger_code(func_block *fb, int i)
{
	return 0;
}
int eqless_code(func_block *fb, int i)
{
	return 0;
}
int noteq_code(func_block *fb, int i)
{
	return 0;
}
int or_code(func_block *fb, int i)
{
	return 0;
}
int and_code(func_block *fb, int i)
{
	return 0;
}
int get_rb_code(func_block *fb, int i)
{
	return 0;
}
int set_rb_code(func_block *fb, int i)
{
	return 0;
}

int call_code(func_block *fb, int i)
{
	int j, k, tmp, m, rtn_reg;
	int tmp_reg_var[32];
	symtbl_hdr *ptr = triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].symtbl;
	func_block *q = (triple_list[index_index[triple_list[index_index[i]].arg1.temp_index]].block)->fb;
	unsigned int *live = fb->live_status[i-fb->start->begin];
	for (j = 7, k = 0; j <= 15 && j <= q->reg_used+6; j++)
	{
		if (fb->reg_var[j] != -1 && (live[fb->reg_var[j]/32] >> (31-fb->reg_var[j]%32)) % 2 == 1)
		{
			k++;
			add_assemble(NULL, stw, 29, j, 0, -1, 1, -k*4);
		}
		tmp_reg_var[j] = fb->reg_var[j];
	}
	if (ptr->para_num > 4)
		m = (k + ptr->para_num - 4) * 4;
	else
		m = k * 4;
	add_assemble(NULL, sub, 29, 29, 0, -1, 1, m);
	for (j = ptr->para_num-1, k = 0; j >= 0; j--)
	{
		tmp = fb->reg_alloc[triple_list[index_index[i-ptr->para_num+j]].arg1_uni];
		if (tmp == -1)
		{
			add_assemble(NULL, ldw, 27, 4, 0, -1, 1, ptr->item[j].offset);
			tmp = 4;
		}
		if (j >= ptr->para_num-4)
			add_assemble(NULL, mov, -1, ptr->para_num-1-j, 0, -1, 0, tmp);
		else
		{
			add_assemble(NULL, stw, 29, tmp, 0, -1, 1, k);
			k += 4;
		}
	}
	add_assemble(ptr->func_name, b_l, -1, -1, 0, -1, 0, -1);
	add_assemble(NULL, add, 29, 29, 0, -1, 1, m);
	for (j = 7, k = 0; j <= 15 && j <= q->reg_used+6; j++)
	{
		fb->reg_var[j] = tmp_reg_var[j];
		if (fb->reg_var[j] != -1 && (live[fb->reg_var[j]/32] >> (31-fb->reg_var[j]%32)) % 2 == 1)
		{
			k++;
			add_assemble(NULL, ldw, 29, j, 0, -1, 1, -k*4);
		}
	}
	rtn_reg = fb->reg_alloc[triple_list[index_index[i]].tmp_uni];
	if (rtn_reg != -1)
	{
		fb->reg_var[rtn_reg] = triple_list[index_index[i]].tmp_uni;
		add_assemble(NULL, mov, -1, rtn_reg, 0, -1, 0, 0);
	}
	else
		add_assemble(NULL, stw, 27, 0, 0, -1, 1, fb->uni_table[triple_list[index_index[i]].tmp_uni]->offset);
	return 0;
}

int param_code(func_block *fb, int i)
{
	return 0;
}

int enterF_code(func_block *fb, int i)
{
	int j;
	symtbl_hdr *ptr = triple_list[index_index[i]].symtbl;
	add_assemble(NULL, stw, 29, 31, 0, -1, 1, -4);
	add_assemble(NULL, stw, 29, 30, 0, -1, 1, -8);
	add_assemble(NULL, stw, 29, 29, 0, -1, 1, -12);
	add_assemble(NULL, stw, 29, 27, 0, -1, 1, -16);
	add_assemble(NULL, sub, 29, 27, 0, -1, 1, 4);
	add_assemble(NULL, sub, 29, 29, 0, -1, 1, fb->min_stack_size);
	for (j = 0; j < fb->reg_used-9; j++)
	{
		add_assemble(NULL, stw, 27, 17+j, 0, -1, 1, -(16+j*4));
		temp_reg_var[17+j] = fb->reg_var[17+j];
	}
	for (j = 0; j < 4 && j < ptr->para_num; j++)
		add_assemble(NULL, stw, 27, j, 0, -1, 1, ptr->item[ptr->para_num-1-j].offset);
	return 0;
}

int enterS_code(func_block *fb, int i)
{
	return 0;
}

int leaveF_code(func_block *fb, int i)
{
	int j;
	//symtbl_hdr *ptr = triple_list[index_index[i]].symtbl;
	for (j = 0; j < fb->reg_used-9; j++)
	{
		fb->reg_var[17+j] = temp_reg_var[17+j];
		add_assemble(NULL, ldw, 27, 17+j, 0, -1, 1, -(16+j*4));
	}
	add_assemble(NULL, ldw, 27, 30, 0, -1, 1, -4);
	add_assemble(NULL, ldw, 27, 29, 0, -1, 1, -8);
	add_assemble(NULL, ldw, 27, 27, 0, -1, 1, -12);
	add_assemble(NULL, jump, -1, 30, 0, -1, 0, -1);
	return 0;
}

int leaveS_code(func_block *fb, int i)
{
	return 0;
}

int rtn_code(func_block *fb, int i)
{
	int j;
	j = triple_list[index_index[i]].arg1_uni;
	if (fb->reg_alloc[j] != -1)
		add_assemble(NULL, mov, -1, 0, 0, -1, 0, fb->reg_alloc[j]);
	else
		add_assemble(NULL, ldw, 27, 0, 0, -1, 1, fb->uni_table[j]->offset);
	return 0;
}

int adds_code(func_block *fb, int i)
{
	return 0;
}

int int_to_char_code(func_block *fb, int i)
{
	return 0;
}

