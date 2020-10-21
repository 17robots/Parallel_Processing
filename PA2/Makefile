CC = gcc

all: pa2a pa2b pa2c
.PHONY : all

pa2a: pa2a.c
	$(CC) -pthread -o pa2a $@.c
.PHONY: pa2a

pa2b: pa2b.c
	$(CC) -pthread -o pa2b $@.c
.PHONY: pa2b

pa2c: pa2c.c
	$(CC) -pthread -o pa2c $@.c
.PHONY: pa2c

clean:
	rm pa2a pa2b pa2c
.PHONY: clean