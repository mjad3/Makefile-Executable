CC = gcc
CFLAGS = -Wall -g
EXE = 537make
OBJECTS = build_specs.o create_exec.o representation.o parser.o

537make: build_specs.o create_exec.o representation.o parser.o
	$(CC) $(CFLAGS) -o $(EXE) main.c $(OBJECTS)

#Second Layer
build_specs.o: create_exec.h representation.h
	$(CC) $(CFLAGS) -c build_specs.c
create_exec.o: parser.h representation.h
	$(CC) $(CFLAGS) -c create_exec.c

# Base Files
representation.o:
	$(CC) $(CFLAGS) -c representation.c
parser.o:
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm $(OBJECTS) $(EXE)
