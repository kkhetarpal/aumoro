all: serial_talk.o
	g++ my_example.cpp serial_talk.o -o my_example -L/usr/lib -lGL -lGLU -lpthread

serial_talk.o: serial_talk.h serial_talk.cpp
	g++ -c serial_talk.cpp -lserialib -o serial_talk.o

