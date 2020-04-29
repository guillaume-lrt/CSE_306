PROG = textr
CC = g++
CPPFLAGS = -g –Wall –I/usr/class/cs193d/textrInc
LDFLAGS = -L/usr/class/cs193/lib –lCoolFns
OBJS = main.o blurb.o database.o
$(PROG) : $(OBJS)
    $(CC) $(LDFLAGS) -o $(PROG) $(OBJS)
main.o :
    $(CC) $(CPPFLAGS) -c main.cpp

clean:
    rm -f core $(PROG) $(OBJS)