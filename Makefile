CC=clang
CFLAGS=-O3
LDFLAGS=-lm
EXEC=out

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)


all: $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)


%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

run: all
	./$(EXEC)

clean:
	rm -f src/*.o
	[ ! -e $(EXEC) ] || rm ./$(EXEC)
