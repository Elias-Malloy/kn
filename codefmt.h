#pragma once

#include <stdint.h>

typedef enum CodeFormat {
	CODE_FORMAT_GNU_ATNT_x86_64_ASSEMBLY,
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
	INSTR_CALL,
	INSTR_RET,
	INSTR_SHR,
	INSTR_SHL,
	INSTR_SAR,
} Instruction;

typedef uint64_t Argument;

void add_instruction(Program *program, Instruction instr, Argument *args);