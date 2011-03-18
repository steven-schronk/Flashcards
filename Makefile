CC=gcc

CFLAGS_DBG=-Wall -pedantic -std=c99 -ansi -DDEBUG -g -O0 -ggdb3 -Wextra -Wno-missing-field-initializers
CFLAGS_REL=-Os

OBJS=flashcards.c
PROG=flashcards

all: $(PROG)

flashcards: $(OBJS)
	$(CC) $(CFLAGS_DBG) $(OBJS) -o $(PROG)

release: $(OBJS)
	$(CC) $(CFLAGS_REL) $(OBJS) -o $(PROG)
	strip -R .comment -R .note -R .nont.ABI-tag $(PROG)

clean:
	rm -rf *o flashcards
