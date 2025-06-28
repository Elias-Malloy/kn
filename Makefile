HEADERS=lr1.h lr_tree.h bit_array256.h
OBJECTS=main.o lr_tree.o parse.o

CFLAGS=-std=c99 -Wall -O3 -m64 -mbmi

all: bin

bin: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJECTS) bin
