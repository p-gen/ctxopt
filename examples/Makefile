OBJS	= hello.o ctxopt.o
SOURCE	= hello.c ctxopt.c
HEADER	= ctxopt.h
OUT	= hello
CC	 = gcc
FLAGS	 = -g -c -O0 -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

hello.o: hello.c
	$(CC) $(FLAGS) hello.c 

ctxopt.o: ../ctxopt.c
	$(CC) $(FLAGS) ../ctxopt.c 

clean:
	rm -f $(OBJS) $(OUT)
