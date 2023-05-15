CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra -g
LFLAGS = -lm

all: banhammer

banhammer: banhammer.o node.o bv.o bf.o ht.o parser.o speck.o bst.o
	$(CC) -o banhammer banhammer.o bv.o ht.o bf.o node.o parser.o speck.o bst.o $(LFLAGS)

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

bst.o: bst.c
	$(CC) $(CFLAGS) -c bst.c

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f banhammer *.o

#cfa inspired by another student
cfa: clean format all


