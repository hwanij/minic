#include "AST.h"
#include <stdio.h>
#include <memory.h>
char name[][30] = {"PROGRAM","EXTERNAL_DECLS","DECLARATION","FUNCTION_LIST","TYPE_NAME","VAR_LIST","VAR_ITEM","ARRAY_VAR","SCALAR_VAR","FUNCTION_DEF","FUNCTION_HDR","PARM_TYPE_LIST","PARM_LIST","PARM_DECL","FUNCTION_BODY","INTERNAL_DECLS","STATEMENT_LIST","STATEMENT","COMPOUNDSTMT","NULLSTMT","EXPRESSION_STMT","IFSTMT","FOR_STMT","WHILE_STMT","RETURN_STMT","EXPRESSION","ASSIGNMENT_EXPRESSION","LVALUE","RVALUE","OP","CONSTANT","ARGUMENT_LIST","EXTERN_T","REGISTER_T","VOID_T","INT_T","CHAR_T","IF_T","ELSE_T","FOR_T","WHILE_T","RETURN_T","BOOLEAN_OP_T","REL_OP_T","DOUBLE_OP_T","ICONSTANT_T","CHAR_CONSTANT_T","STRING_CONSTANT_T","SEMICOLON","COMMA","STAR","LEFT_SQUARE_BRACKET","RIGHT_SQUARE_BRACKET","LEFT_PARENTHESE","RIGHT_PARENTHESE","LEFT_BRACE","RIGHT_BRACE","EQUALITY_SIGN","MINUS_SIGN","PLUS_SIGN","MULTIPLY_SIGN","POSITIVE_SIGN","NEGATIVE_SIGN","NOT_SIGN","ADDRESS_SIGN","IDENT_T","EPSILON"};
// use this function to print the AST tree (with terminal/non-terminal names)
int counter[100];
void print_AST_dot_core(AST_NODE* ptr);
void print_AST(AST_NODE* ptr, int level)
{
	int i;
	AST_NODE* p;
	if (ptr == NULL)
		return;
	for (i = 0; i < level; i++)
		printf("|    ");
	printf("%s", name[ptr->nodeType-1001]);
	
	// additional node information
	if( ptr->nodeType == COMPOUNDSTMT )
		printf(": scope_number=%d",ptr->content.i_content);
	// additional node information end
	printf("\n");
	for (p = ptr->leftChild; p != NULL; p = p->rightSibling)
		print_AST(p, level+1);


}

void print_AST_dot(AST_NODE* ptr)
{
	memset(counter,0,100*sizeof(int));
	printf("digraph G \{\n");
	print_AST_dot_core(ptr);
	printf("}\n");
}

void print_AST_dot_core(AST_NODE* ptr)
{
	AST_NODE *p;
	int temp = counter[ptr->nodeType-1001]++;
	if (ptr->leftChild == NULL)
	{
		printf("%s%d;\n", name[ptr->nodeType-1001],counter[ptr->nodeType-1001]);
		return;
	}
	for (p = ptr->leftChild; p != NULL; p = p->rightSibling)
	{
		printf("%s%d -> ", name[ptr->nodeType-1001], temp);
		print_AST_dot_core(p);
	}
	return;
}


void print_symtbl(AST_NODE* p)
{
	int i;
	AST_NODE* ptr;
	if (p == NULL)
		return;
	if (p->nodeType == PROGRAM || p->nodeType == FUNCTION_DEF || p->nodeType == COMPOUNDSTMT)
	{
		printf("=======================================\n");
		printf("%s\n", name[p->nodeType-FUNC_OFFSET]);
		printf("ret_type: %s\n", name[p->symtbl->ret_type-FUNC_OFFSET]);
		printf("ret_star: %d\n", p->symtbl->ret_star);
		printf("para_num: %d\n", p->symtbl->para_num);
		printf("item_num: %d\n", p->symtbl->item_num);
		printf("maxSize: %d\n", p->symtbl->maxSize);
		for (i = 0; i < p->symtbl->item_num; i++)
			printf("%s  %d  %d  %s  %d\n", name[p->symtbl->item[i].type-FUNC_OFFSET], p->symtbl->item[i].star_num, p->symtbl->item[i].writable, p->symtbl->item[i].name, p->symtbl->item[i].size);
		printf("\n");
	}
	for (ptr = p->leftChild; ptr != NULL; ptr = ptr->rightSibling)
		print_symtbl(ptr);
}


