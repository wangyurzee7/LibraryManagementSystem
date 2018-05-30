# Build options
CC=g++
CFLAGS=-std=c++11 -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx



# Build targets
all: field.o object.o database.o md5.o encryptor.o password.o user.o book.o practicalbook.o record.o content.o txtcontent.o server.o

field.o: field.cpp field.h
	c++ ${CFLAGS} -c $< -o $@

object.o: object.cpp object.h field.h
	c++ ${CFLAGS} -c $< -o $@

user.o: user.cpp user.h object.h field.h
	c++ ${CFLAGS} -c $< -o $@
	
book.o: book.cpp book.h object.h field.h
	c++ ${CFLAGS} -c $< -o $@

practicalbook.o: practicalbook.cpp practicalbook.h object.h field.h
	c++ ${CFLAGS} -c $< -o $@

record.o: record.cpp record.h object.h field.h
	c++ ${CFLAGS} -c $< -o $@

md5.o: md5.cpp md5.h
	c++ ${CFLAGS} -c $< -o $@

encryptor.o: encryptor.cpp encryptor.h md5.h
	c++ ${CFLAGS} -c $< -o $@

password.o: password.cpp password.h encryptor.h
	c++ ${CFLAGS} -c $< -o $@

database.o: database.cpp database.h user.h book.h practicalbook.h record.h object.h field.h errorcode.h
	c++ ${CFLAGS} -c $< -o $@

content.o: content.cpp content.h
	c++ ${CFLAGS} -c $< -o $@

txtcontent.o: txtcontent.cpp txtcontent.h content.h
	c++ ${CFLAGS} -c $< -o $@

server.o: server.cpp server.h field.h object.h database.h password.h user.h book.h practicalbook.h record.h content.h txtcontent.h errorcode.h searchstrategy.h
	c++ ${CFLAGS} -c $< -o $@