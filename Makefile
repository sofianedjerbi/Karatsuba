CC=clang
CFLAGS=-O2
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
	rm ./$(EXEC)
