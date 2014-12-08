all: duetracking

duetracking: duetracking.cpp serial_talk.o
	g++ -g -o0 -o duetracking `pkg-config --cflags opencv` duetracking.cpp serial_talk.o `pkg-config --libs opencv`

serial_talk.o: serial_talk.h serial_talk.cpp
	g++ -g -o0 -c serial_talk.cpp -lserialib -o serial_talk.o

clean:
	rm -rf *o duetracking
