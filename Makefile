PROG=main
OBJECTS = knitlist.o
CFLAGS = -std=c11 -g -Wall -Wno-initializer-overrides -O3
LDLIBS =
CC = clang
DBG	= lldb

$(PROG): $(OBJECTS)

run: $(PROG)
	./$(PROG)

leaks: $(PROG)
	leaks -atExit -- ./$(PROG)

debug: $(PROG)
	$(DBG) $(PROG)

clear:
	rm $(PROG)
	rm -rf $(PROG).dSYM


