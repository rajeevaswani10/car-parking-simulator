IDIR =./
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=src
LDIR =../lib

LIBS=-lm -lpthread

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o parking_buffer.o list.o queue.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cps: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

