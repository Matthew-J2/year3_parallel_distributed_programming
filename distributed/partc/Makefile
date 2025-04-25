CC 	 = mpic++ -std=c++11
CFLAGS 	 = -fopenmp
LLIBS    = -lm -lgomp
LDFLAGS  = 
LIB_DIRS =
#----------

EXE = bin

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#----------

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LIB_DIRS) $(LLIBS)$(LDFLAGS) -o $(EXE)

$(OBJS): %.o : %.cpp
	$(CC) $(CFLAGS) -c $<
#----------
clean:
	rm -f *.o *.*~ *~  $(EXE) 
