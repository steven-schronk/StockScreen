CC=gcc

LIBS =  -lglut -lm

CFLAGS_REL = -Wall -ansi -O3
CFLAGS_DBG = -Wall -pedantic -std=c99 -ansi -DDEBUG -g -O0 -ggdb3 -Wextra -Wno-missing-field-initializers
CFLAGS_SML = -Wall -ansi -Os

OBJS = stock.c
OUT = StockScreen

default: release

release: $(OBJS)
	$(CC) $(CFLAGS_SML) $(LIBS) -o $(OUT) $^ 

debug: $(OBJS)
	$(CC) $(CFLAGS_DBG) $(LIBS) -o $(OUT) $^

clean:
	rm -f $(OUT)
