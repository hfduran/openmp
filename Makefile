# Compiler and flags
CC = gcc
CFLAGS = -fopenmp -Wall -O2

# Find all .c files in topic* directories
SOURCES = $(wildcard topic*/*.c)
# Generate executable names (remove .c extension)
EXECUTABLES = $(SOURCES:.c=)

# Default target: compile all
all: $(EXECUTABLES)

# Pattern rule: compile any topic*/file.c to topic*/file
topic%: topic%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $< -o $@

# Clean all executables
clean:
	@echo "Cleaning executables..."
	@rm -f $(EXECUTABLES)
	@echo "Done!"

# Show what would be compiled
list:
	@echo "Source files found:"
	@echo $(SOURCES) | tr ' ' '\n'
	@echo ""
	@echo "Executables to build:"
	@echo $(EXECUTABLES) | tr ' ' '\n'

.PHONY: all clean list
