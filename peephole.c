#include "register.h"
#include "gen_intermediate_code.h"
void peephole_on_intermediate_code();
void peephole_on_target_code();
void calc_const();

extern triple* triple_list;
extern int triple_list_index;

void peephole_on_intermediate_code()
{
	calc_const();
}

void calc_const()
{
	int i, j;
	triple temp;
	for(i = 0 ; i < triple_list_index ; i ++){
		temp = triple_list[i];
		switch(temp.op){
			case negative_op:
				{
					if(temp.arg1_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(-temp.arg1.imm_value);
							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(-temp.arg1.imm_value);
							}
						}
					} 
					break;
				}
			case positive_op:
				{
					temp.is_deleted = 1;
					for(j = i+1; j < triple_list_index ;j ++){
						if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
							triple_list[j].arg1_type = temp.arg1_type;
							triple_list[j].arg1 = temp.arg1;
						}
						else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
							triple_list[j].arg2_type = temp.arg1_type;
							triple_list[j].arg2 = temp.arg2;
						}
					}
					break;
				}
			case add_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(temp.arg1.imm_value + temp.arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(temp.arg1.imm_value + temp.arg2.imm_value);
							}
						}
					}
					break;
				}
			case minus_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(temp.arg1.imm_value - temp.arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(temp.arg1.imm_value - temp.arg2.imm_value);
							}
						}
					}
					break;
				}
			case multiply_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)(temp.arg1.imm_value * temp.arg2.imm_value); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)(temp.arg1.imm_value * temp.arg2.imm_value);
							}
						}
					}
					break;
				}
			case equal_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value == temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value == temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case less_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value < temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value < temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case larger_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value > temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value > temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case eqlarger_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value >= temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value >= temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case eqless_op: 
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value <= temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value <= temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			case noteq_op:
				{
					if(temp.arg1_type == 2 && temp.arg2_type == 2){
						temp.is_deleted = 1;
						for(j = i+1 ; j < triple_list_index ; j ++){
							if(triple_list[j].arg1_type == 1 && triple_list[j].arg1.temp_index == i){
								triple_list[j].arg1_type = 2;
								triple_list[j].arg1 = (union arg)((temp.arg1.imm_value != temp.arg2.imm_value)?1:0); 							}
							else if(triple_list[j].arg2_type == 1 && triple_list[j].arg2.temp_index == i){
								triple_list[j].arg2_type = 2;
								triple_list[j].arg2 = (union arg)((temp.arg1.imm_value != temp.arg2.imm_value)?1:0);
							}
						}
					}
					break;
				}
			default:break;
		}
	}
}
