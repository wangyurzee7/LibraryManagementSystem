# Build options
CC=g++
CFLAGS=-std=c++11
DBFLAGS=-I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx



# Build targets
all: field.o object.o database.o 

field.o: field.cpp field.h
	c++ ${CFLAGS} -c $< -o $@

object.o: object.cpp object.h
	c++ ${CFLAGS} -c $< -o $@

database.o: database.cpp database.h
	c++ ${CFLAGS} ${DBFLAGS} -c $< -o $@