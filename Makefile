CC=gcc
CFLAGS=-O2 -c -Wall -Werror -pedantic -Iinclude
LD=gcc
LFLAGS=

SRC=pollute.c
OBJ=$(SRC:.c=.o)

RM=del
RFLAGS=/f /q

EXE=pollute.exe

all: $(EXE)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

$(EXE): $(OBJ)
	$(LD) $(LFLAGS) $^ -o $@

clean:
	$(RM) $(RFLAGS) $(OBJ)
	$(RM) $(RFLAGS) $(EXE)