ARM_PREFIX = arm-linux-gnueabi

AR = $(ARM_PREFIX)-ar
CC = $(ARM_PREFIX)-gcc
CPP = $(ARM_PREFIX)-g++
RANLIB = $(ARM_PREFIX)-ranlib

main: main.o ../Support/libev3.a
	$(CPP) -o main -L../Support main.o -lev3

main.o: main.cpp ../Support/ev3dev.h
	$(CPP) -c -I../Support main.cpp

