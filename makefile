# Nome do programa
PROGRAM = main

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -g -Wall

# Arquivo fonte
SRC = main.c

# Arquivo objeto
OBJ = $(SRC:.c=.o)

# Regra padrão
all: $(PROGRAM)

# Regra para compilar o programa
$(PROGRAM): $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJ)

# Regra para compilar os arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos objeto e o programa
clean:
	rm -f $(OBJ) $(PROGRAM)

# Regra para recompilar tudo
rebuild: clean all

