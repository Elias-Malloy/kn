#pragma once

#include <stdint.h>

typedef enum CodeFormat {
	CODE_FORMAT_GNU_ATNT_X86_64_ASSEMBLY,
} CodeFormat;

typedef struct GNU_ATNT_x86_64_Assembly {
	char *start;
	char *cursor;
	char *end;
} GNU_ATNT_x86_64_Assembly;

typedef union ProgramData {
	GNU_ATNT_x86_64_Assembly gnu_asm;
} ProgramData;

typedef struct Program {
	CodeFormat fmt;
	ProgramData data;
} Program;

typedef enum Instruction {
	INSTR_MOV,
	INSTR_ADD,
	INSTR_SUB,
	INSTR_MUL,
	INSTR_DIV,
	INSTR_AND,
	INSTR_OR,
	INSTR_XOR,
	INSTR_NOT,
	INSTR_SHL,
	INSTR_SHR,
	INSTR_SAR,
	INSTR_CALL,
	INSTR_RET,
} Instruction;

char *instruction_to_string[] = {
	"mov",
	"add",
	"sub",
	"mul",
	"div",
	"and",
	"or",
	"xor",
	"not",
	"shl",
	"shr",
	"sar",
	"call",
	"ret",
}

enum PrimitiveType {
	PRIM_INT64,
	PRIM_UINT64,
	PRIM_INT32,
	PRIM_UINT32,
	PRIM_INT16,
	PRIM_UINT16,
	PRIM_INT8,
	PRIM_UINT8,
	PRIM_TYPE_COUNT,
}

uint16_t atnt_size_suffix[PRIM_TYPE_COUNT] = {
	'q', 'q',
	'l', 'l',
	'w', 'w',
	'b', 'b',
}

typedef uint64_t Argument;

#define INSTR_BUF_SIZE 100

void add_instruction(Program *program, Instruction instr, Argument *args) {
	uint64_t i, len;
	char instr_buf[INSTR_BUF_SIZE];	
	char *mem;
	
	if (program->fmt == CODE_FORMAT_GNU_ATNT_X86_64_ASSEMBLY) {
		
		strcpy(instr_buf, instruction_to_string[instr]);
		strcat(instr_buf, &atnt_size_suffix[args[0].type];
		
		len = strlen(instr_buf);
		
		if (program->data.gnu_asm.cursor + len >= program->data.gnu_asm.end) {
			i = program->data.gnu_asm.cursor - program->data.gnu_asm.start;
			mem = realloc(program->data.gnu_asm.start, i * 2 + len);
			if (mem == NULL) return;
			program->data.gnu_asm.start = mem;
			program->data.gnu_asm.cursor = mem + i;
			program->data.gnu_asm.end = mem + i * 2 + len;
		}
		
		strcpy(program->data.gnu_asm.cursor, instr_buf); 
		program->data.gnu_asm.cursor += len;
	}
	
	return 0;
}