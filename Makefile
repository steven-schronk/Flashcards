CC=gcc

CFLAGS_REL=-Wall -ansi -O0
#CFLAGS_DBG=-Wall -Werror -pedantic -std=c99 -ansi -DDEBUG -g -O0 -ggdb3 -Wextra -Wno-missing-field-initializers
CFLAGS_DBG=-Wall -pedantic -std=c99 -ansi -DDEBUG -g -O0 -ggdb3 -Wextra -Wno-missing-field-initializers
CFLAGS_SML=-Wall -ansi -Os

OBJS=flashcards.c
OUT=-o flashcards

asmcompile: $(OBJS)
	$(CC) $(CFLAGS_REL) $(OUT) $^

asmlink: asm.o
	gcc -o flashcards flashcards.o

asm: asmlink

small: $(OBJS)
	$(CC) $(CFLAGS_SML) $(OUT) $^ 

debug: $(OBJS)
	$(CC) $(CFLAGS_DBG) $(OUT) $^

clean:
	rm -f flashcards *.o

