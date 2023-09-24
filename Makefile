PROG = main 
OBJECTS = knitlist.o 
CFLAGS = -std=c11 -g -Wall -Wno-initializer-overrides -O0 
LDLIBS = 
CC = clang 
DBG = lldb

all: $(PROG) run clean

$(PROG): $(OBJECTS)

run: $(PROG)
	./$(PROG)

leaks: $(PROG) 
	leaks -atExit -- ./$(PROG)

debug: $(PROG) 
	$(DBG) $(PROG)

docs: 
	doxygen
													  
stat: 
	flawfinder .
	cppcheck .

clean: 
	rm $(PROG) $(OBJECTS)
	rm -rf $(PROG).dSYM 
