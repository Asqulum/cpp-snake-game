CC = c++
CFLAGS = -std=c++11 -Wall -c -I/usr/include

OBJECTS = main.o snek.o

executable: $(OBJECTS)
	$(CC) $(OBJECTS) -lncurses -o  snek

snek.o: snek.cc snek.h
	$(CC) $(CFLAGS)  snek.cc

main.o: main.cc snek.h
	$(CC) $(CFLAGS)  main.cc



