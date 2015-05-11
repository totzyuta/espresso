CC	      = gcc
CFLAGS        = -O -Wall
HDRS	      = define.h
LDFLAGS	      =
LIBS	      =
OBJS	      = main.o lex.o oparser.o
PROGRAM	      = oparser

#all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(HDRS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;		rm -f *.o *~

###
main.o: main.c define.h
lex.o:  lex.c  define.h
oparser.o:  oparser.c  define.h
