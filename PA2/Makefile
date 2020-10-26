CC = gcc

all: pa2a pa2b
.PHONY : all

pa2a: pa2a.c
	$(CC) -pthread -o pa2a $@.c
.PHONY: pa2a

pa2b: pa2b.c
	$(CC) -pthread -o pa2b $@.c
.PHONY: pa2b

clean:
	rm pa2a pa2b
.PHONY: clean