/**
 * Caution:
 * Due to the dependency on unistd.h, 
 * only under POSIX OS can this file be compiled.
 * To use minic under Windows, compile main.c.old
 */
#include "validation_utils.h"
#include "main.h"
#include "register_stats.h"
#include <unistd.h> //getopt() support
#include <assert.h>
#include <string.h>

#define PREPROCESSOR_TMP_SUFFIX ".p"
int debug_mode= 0;
int peephole_disabled = 0;
int error_number;
int tail_recursion_flag = 0;
void usage()
{
	printf("MiniC compiler:\n\
Usage: minic [options] source_file\n\
Options:\n\
	-d Bison parser debug output\n\
	-o Print AST in a DOT file\n\
	-t Print ASCII AST tree\n\
	-s Print symbol tables\n\
	-i Print intermediate code\n\
	-b Print basic block in a DOT file\n\
	-g Print interference graph\n\
	-O1 Enable instruction dispatch\n\
	-O2 Enable available expression remove\n\
	-O3 = both -O1 -O2\n\
	-O0 = Disable peephole optimization on target code\n\
	-Ox = Enable tail recursion optimizationi. WARNING: MAY GENERATE WRONG TARGET CODE, PLEASE REFER TO THE MANUAL!\n\
	----------------------------------\n\
	Target machine register system info (corresponding with ABI):\n\
		Caller save: r%d - r%d\n\
		Callee save: r%d - r%d\n\
		Totally %d usable registers\n", CALLER_REG_START,
	CALLER_REG_END,
	CALLEE_REG_START,
	CALLEE_REG_END,
	AVALIABLE_REG_NUM);
	if(debug_mode)
		printf("***Caution: DEBUG MODE ***\n");
	if(peephole_disabled)
		printf("***Peephole optimization on target DISABLED!\n");
}
/**
 * parse the input file name, cut out the extend name.
 */
void parse_file_name(char* dst, char* src)
{
	int i, len = strlen(src);
	memset(dst,0,sizeof(char) * (len+1));
	for (i = len - 1; i >= 0 && src[i] != '.'; i--)
		;
	strncpy(dst,src,i<0?len:i);
}
int main(int argc, char** argv)
{
	//CLI arguments
	char srcfile_name[255];
	int oc,
		dbg_print_tree = 0,
		dbg_print_symtbl = 0,
		dbg_print_tree_dot = 0,
		dbg_no_intermediate = 0,
		dbg_print_basic_block = 0,
		dbg_print_interference_graph=0,
		dbg_print_intermediate_code = 0,
		dbg_print_live_var = 0,
		dbg_print_register_allocation = 0;
		debug_mode = 0;
#ifdef DEBUG
	dbg_print_live_var = 1;
	dbg_print_register_allocation = 1;
	debug_mode = 1;
#endif
#ifdef DISABLE_PEEPHOLE
	peephole_disabled = 1;
#endif
	/**
	 *Optimizing arguments:
	 * *assemble dispatch
	 * *available expression dataflow analysis
	 */
	int dispatch_flag = 0;
	int data_flow_flag = 0;
	/*--*/
	/*
	 * Iteration count of available analysis
	 * FOR DEBUG
	 */
	int iteration_count;


	int gen_symtbl_result ;

	FILE *source_file, *target_file;
	//error number, handled by bison
	error_number = 0;
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	while((oc = getopt(argc, argv, "dotsgaxiblrO:")) != -1)
	{
		switch(oc)
		{
			case 'd': // debug on
				yydebug = 1;
				break;
			case 'o': //output AST
				dbg_print_tree_dot = 1;
				break;
			case 's':
				dbg_print_symtbl = 1;
				break;
			case 'a':
				yydebug = dbg_print_tree
					= dbg_print_symtbl 
					= 1;
				break;
			case 'x':
				dbg_no_intermediate = 1;
				break;
			case 't':
				dbg_print_tree = 1;
				dbg_print_intermediate_code = 0;
				dbg_print_basic_block = 0;
				dbg_print_interference_graph = 0;
				break;
			case 'i':
				dbg_print_tree = 0;
				dbg_print_basic_block = 0;
				dbg_print_interference_graph = 0;
				dbg_print_intermediate_code = 1;
				break;
			case 'b':
				dbg_print_intermediate_code = 0;
				dbg_print_tree = 0;
				dbg_print_interference_graph = 0;
				dbg_print_basic_block = 1;
				break;
			case 'g':
				dbg_print_intermediate_code = 0;
				dbg_print_tree = 0;
				dbg_print_interference_graph = 1;
				dbg_print_basic_block = 0;
				break;
			case 'l':
				dbg_print_live_var = 1;
				break;
			case 'r':
				dbg_print_register_allocation = 1;
				break;
			case 'O':
				switch(optarg[0])
				{
					case '3':
						data_flow_flag = 1;
						dispatch_flag = 1;
						peephole_disabled = 0;
						break;
					case '2':
						data_flow_flag = 1;
						peephole_disabled = 1;
						break;
					case '1':
						dispatch_flag = 1;
						peephole_disabled = 1;
						break;
					case '0':
						// disable peephole at all
						peephole_disabled = 1;
						break;
					case 'x':
						tail_recursion_flag = 1;
						break;
					default:
						;
				}
				break;
			default:
				usage();
				return -1;
		}
	}
	if(optind >= argc) // no file name given
	{
		usage();
		return -1;
	}
	parse_file_name(srcfile_name, argv[optind]);
	
	/**
	 *call preprocessor
	 */
	pre_compile(srcfile_name);

	source_file = fopen (strcat(srcfile_name,PREPROCESSOR_TMP_SUFFIX), "r");
//	source_file = fopen(argv[optind],"r");
	if (!source_file) //file open error
	{
		fprintf(stderr,"%s: open error!\n",argv[optind]);
		return -1;
	}
	
	yyin = source_file;
	//call bison parser
	if (yyparse() || error_number)
	{
		fprintf(stderr, "Parser: terminated, %d error(s).\n",error_number);
		return -1;
	}

	//delete preprocesser temp file
	fclose(source_file);
	//remove(strcat(srcfile_name,PREPROCESSOR_TMP_SUFFIX));
	
	if(dbg_print_tree)
		print_AST(tree_root,0);

	if(dbg_print_tree_dot)
		print_AST_dot(tree_root);
	
	//generate symbol table
	fprintf(stderr,"Generating symbol table...");
	gen_symtbl_result = gen_symtbl(tree_root);
	if(gen_symtbl_result)
	{
		fprintf(stderr,"Error generating symbol tables!\n");
		return -1;
	}
	fprintf(stderr,"done.\n");

	if(dbg_print_symtbl)
		print_symtbl(tree_root->symtbl);

	//type verification
	fprintf(stderr,"Doing type verification...");
	dfs_type_verification(tree_root);
	fprintf(stderr,"done.\n");
	if(error_number)
	{
		fprintf(stderr,"Error type verification!\n");
		return -1;
	};
	if(dbg_no_intermediate)
	{
		fprintf(stderr,"No intermediate mode: i am quiting...\n");
		return 0;
	}

	//generate intermediate code
	fprintf(stderr,"Generating intermediate code...");
	intermediate_code(tree_root);
	fprintf(stderr,"done.\n");
	if(dbg_print_intermediate_code)
	{
		print_intermediate_code();
		return 0;
	}
//	if(!peephole_disabled)
	{
		fprintf(stderr,"Applying intermediate code peephole optimization...");
		peephole_on_intermediate_code();
		fprintf(stderr,"done.\n");
	}
	//generate basic block
	fprintf(stderr,"Generating basic block...");
	gen_basic_block();
	fprintf(stderr,"done.\n");
	if(dbg_print_basic_block)
	{	
		print_basic_block();
		return 0;
	}
	//作数据流分析的准备：建立联合符号表
	gen_uni_table();
	//指针分析
	pointer_anal();

	/**
	 * DEBUG
	 */
//	print_ptr_anal();
	//可用表达式分析
	if(data_flow_flag)
	{
		fprintf(stderr,"OPTIMIZATION: will do extra dataflow analysis!\n");
		fprintf(stderr,"Running available expression analysis...");
		iteration_count = available_expr();
		fprintf(stderr,"done, iteration count: %d\n",iteration_count);
	//	print_available_expr();
	}
	//live variable analyzing
	fprintf(stderr,"Analyzing lively variables...");
	live_var_anal();
	fprintf(stderr,"done.\n");
	/**
	 *DEBUG FILE:
	 *result file of live variable analizing is live_var_anal.debug
	 */
	if(dbg_print_live_var)
		print_live_var();
	if(dbg_print_interference_graph)
	{	
		print_interference_graph();
		return 0;
	}
	//register allocation
	fprintf(stderr,"Allocating registers for variables...");
	register_allocation(AVALIABLE_REG_NUM);
	fprintf(stderr,"done.\n");
	
	/**
	 *DEBUG FILE:
	 *register allocation debug file is register_allocation.debug
	 */
	if(dbg_print_register_allocation)
		print_register_allocation();
		
	/*
	 * TODO: Optimizing
	 */
	fprintf(stderr,"Generating target code...");
	if(tail_recursion_flag)
		fprintf(stderr,"with tail recursion enabled...");
	gen_target_code();
	fprintf(stderr,"done.\n");
	
	fprintf(stderr,"Dispatch assemble instructions...");
	instruction_dispatch();
	fprintf(stderr,"done.\n");
	if(dispatch_flag)
		fprintf(stderr,"OPTIMIZATION: Will use dispatched assemble!\n");
	if(!peephole_disabled)
	{
		fprintf(stderr,"Applying target code peephole optimization...");
		peephole_on_target_code();
		fprintf(stderr,"done.\n");
	}
	/* get code output!*/
	target_file = stdout;
#ifndef DEBUG
	parse_file_name(srcfile_name, argv[optind]);
	target_file = fopen(strcat(srcfile_name,".s"),"w+");
#endif

	print_target_code(target_file,dispatch_flag);
	fclose(target_file);
	return 0;
}
