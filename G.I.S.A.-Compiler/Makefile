CC = gcc
CFLAGS = -g
TARGET = gisa_compiler
SRCS = gisa_compiler_driver.c gisa_lexer.c gisa_semantic_analyzer.c gisa_tag_generator.c gisa_code_generator.c gisa_code_emitter.c gisa_compiler_debug_func.c

all: $(SRCS) gisa_parser.c gisa_compiler.h
	$(CC) $(CFLAGS) $(SRCS) gisa_parser.c -o $(TARGET)

parse_tree: $(SRCS) gisa_parse_tree_parser.c gisa_compiler.h
	$(CC) $(CFLAGS) $(SRCS) gisa_parse_tree_parser.c -o $(TARGET)