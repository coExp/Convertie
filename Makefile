#################################################
#                                               #
#                   CONFIG                      #
#                                               #
#################################################

CFLAGS	= -Wall -W -g
CC	= gcc
EXE	= projet.exe

#################################################

SRC :=	 	$(wildcard *.c) 

HEADER :=  	$(wildcard *.h)
 
OBJ    := $(SRC:%.c=%.o)

.c.o: $(SRC)
	$(CC) $(CFLAGS) $< -c -o $@

all: $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

clean:
	rm -f *.o */*.o */*~ $(EXE)


#################################################
