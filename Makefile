CC = gcc
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List all your .c files here (source files, excluding header files)
SRC = main.c world.c parse.c cache.c mments.c write.c read.c vcache.c

# List corresponding compiled object files here (.o files)
#SYSOBJ = ${SYS:%.c=%.o}
SIM_OBJ = ${SRC:%.c=%.o}
 
#################################

# default rule

all: sim_cache
	@echo "my work is done here..."


# rule for making sim_cache

sim_cache: $(SIM_OBJ)
	$(CC) -o sim_cache $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH SIM_CACHE-----------"


# generic rule for converting any .c file to any .o file
 
.c.o:
	$(CC) $(CFLAGS)  -c $*.c


# type "make clean" to remove all .o files plus the sim_cache binary

clean:
	rm -f *.o sim_cache


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f *.o


