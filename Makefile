ARGS = $(CFLAG)
ARGS += -g -Wall
OBJECT = minic.tab.o lex.yy.o AST_operation.o main.o validation_utils.o gen_symtbl.o symtbl_operation.o type_verification.o gen_intermediate_code.o gen_basic_block.o live_var_anal.o register_allocation.o gen_target_code.o instruction_dispatch.o available_expr.o prepare_dataflow.o peephole.o pre_compile.o pointer_anal.o
CSCOPE = *.c *.h *.l *.y
#ARGS = -g -Wall -DDISABLE_PEEPHOLE
minic: $(OBJECT)
	gcc -lm $(ARGS) -o minic $(OBJECT)
minic.tab.o: minic.tab.c minic.tab.h AST.h attr.h
	gcc $(ARGS) -c minic.tab.c
lex.yy.o: lex.yy.c minic.tab.h
	gcc $(ARGS) -c lex.yy.c
minic.tab.c: minic.y
	bison -d -t minic.y
minic.tab.h: minic.y
	bison -d -t minic.y
lex.yy.c: minic.l 
	flex minic.l
AST_operation.o: AST.h AST_operation.c
	gcc $(ARGS) -c AST_operation.c
validation_utils.o : validation_utils.c AST.h validation_utils.h
	gcc $(ARGS) -c validation_utils.c
gen_symtbl.o: gen_symtbl.c symtbl.h
	gcc $(ARGS) -c gen_symtbl.c
symtbl_operation.o: symtbl_operation.c symtbl.h
	gcc $(ARGS) -c symtbl_operation.c
type_verification.o: type_verification.c type_verification.h
	gcc $(ARGS) -c type_verification.c
gen_intermediate_code.o: gen_intermediate_code.c gen_intermediate_code.h
	gcc $(ARGS) -c gen_intermediate_code.c
main.o : main.c validation_utils.h main.h register_stats.h
	gcc $(ARGS) -c main.c
pre_compile.o: pre_compile.c
	gcc $(ARGS) -c pre_compile.c
gen_basic_block.o: gen_basic_block.c basic_block.h
	gcc $(ARGS) -c gen_basic_block.c
prepare_dataflow.o: prepare_dataflow.c basic_block.h
	gcc $(ARGS) -c prepare_dataflow.c
live_var_anal.o: live_var_anal.c basic_block.h
	gcc $(ARGS) -c live_var_anal.c
register_allocation.o: register_allocation.c register.h basic_block.h register_stats.h
	gcc $(ARGS) -c register_allocation.c
gen_target_code.o: gen_target_code.c register.h register_stats.h
	gcc $(ARGS) -c gen_target_code.c
instruction_dispatch.o:instruction_dispatch.c register.h 
	gcc $(ARGS)  -c instruction_dispatch.c
available_expr.o: available_expr.c basic_block.h
	gcc $(ARGS) -c available_expr.c
peephole.o: peephole.c gen_intermediate_code.h register.h
	gcc $(ARGS) -c peephole.c
pointer_anal.o: pointer_anal.c register.h gen_intermediate_code.h
	gcc $(ARGS) -c pointer_anal.c
cscope: $(CSCOPE)
	cscope -Rbq
config-less:
	rm -f register_stats.h
	ln -s register_stats.h.less register_stats.h
config-more:
	rm -f register_stats.h
	ln -s register_stats.h.more register_stats.h
clean:
	rm *.o; rm minic; rm lex.yy.c; rm minic.tab.*;
cleanw:
	del *.o; del minic.exe; del lex.yy.c; del minic.tab.*
