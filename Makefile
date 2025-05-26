CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -Wall -I./include
LDFLAGS =

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source files
LEXER_SRC = $(SRC_DIR)/lexer/lexer.l
PARSER_SRC = $(SRC_DIR)/parser/parser.y
C_SRCS = $(wildcard $(SRC_DIR)/*/*.c)
OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Generated files
LEXER_C = $(BUILD_DIR)/lexer/lex.yy.c
PARSER_C = $(BUILD_DIR)/parser/parser.tab.c
PARSER_H = $(BUILD_DIR)/parser/parser.tab.h

# Target executable
TARGET = mini-c

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)/lexer
	@mkdir -p $(BUILD_DIR)/parser
	@mkdir -p $(BUILD_DIR)/semantic
	@mkdir -p $(BUILD_DIR)/codegen
	@mkdir -p $(BUILD_DIR)/symboltable

$(LEXER_C): $(LEXER_SRC)
	$(FLEX) -o $@ $<

$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(BISON) -d -o $(PARSER_C) $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(LEXER_C) $(PARSER_C) $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) 