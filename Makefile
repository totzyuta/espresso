CC	      = gcc
CFLAGS    = -O -Wall
HDRS	    = define.h
LDFLAGS	  =
LIBS	    =
OBJS	    = main.o lex.o oparser.o parser.o
PROGRAM	  = esp

#all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(HDRS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;		rm -f *.o *~

###
main.o: main.c define.h
lex.o:  lex.c  define.h
oparser.o:  oparser.c  define.h
parser.o: parser.c  define.h
