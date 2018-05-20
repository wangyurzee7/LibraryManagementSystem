# Build options
CC=g++
CFLAGS=-std=c++11
DBFLAGS=-I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx

# Build targets
database.o: database.cpp database.h
	c++ ${CFLAGS} ${DBFLAGS} -c $< -o $@

