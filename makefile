# Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g

# Source files
SRCS = candidatos.c main.c

# Header files (for dependency tracking)
HEADERS = candidatos.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = comissao

# Default target
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files to .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

