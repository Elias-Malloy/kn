#pragma once

#include <stdint.h>

typedef enum CodeFormat {
	CODE_FORMAT_GNU_ATNT_x86_64_ASSEMBLY,
} CodeFormat;

typedef struct GNU_ATNT_x86_64_Assembly {
	char *str;
	uint64_t cursor;
	uint64_t capacity;
} GNU_ATNT_x86_64_Assembly;

typedef union ProgramData {
	GNU_ATNT_x86_64_Assembly gnu_asm;
} ProgramData;

typedef struct Program {
	CodeFormat fmt;
	ProgramData data;
} Program;

typedef uint32_t Opcode;
typedef uint64_t Argument;

void add_instruction(Program *program, Opcode opcode, Argument *args);