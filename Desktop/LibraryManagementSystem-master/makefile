# Build options
CC=g++
CFLAGS=-std=c++11 -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx
DBFLAGS=


all: main

all_o: .CORE .SERVER .CLIENT main.o

# Core Build targets
.CORE: field.o abstractobject.o object.o md5.o encryptor.o password.o user.o book.o practicalbook.o record.o content.o txtcontent.o search.o

md5.o: object/password/md5.cpp object/password/md5.h
	${CC} ${CFLAGS} -c $< -o $@

encryptor.o: object/password/encryptor.cpp object/password/encryptor.h object/password/md5.h
	${CC} ${CFLAGS} -c $< -o $@

password.o: object/password/password.cpp object/password/password.h object/password/encryptor.h
	${CC} ${CFLAGS} -c $< -o $@

field.o: object/field.cpp object/field.h
	${CC} ${CFLAGS} -c $< -o $@

abstractobject.o: object/abstractobject.cpp object/abstractobject.h object/field.h
	${CC} ${CFLAGS} -c $< -o $@

object.o: object/object.cpp object/object.h object/abstractobject.h object/field.h
	${CC} ${CFLAGS} -c $< -o $@

user.o: object/user.cpp object/user.h object/object.h object/field.h object/abstractobject.h
	${CC} ${CFLAGS} -c $< -o $@
	
book.o: object/book.cpp object/book.h object/object.h object/field.h object/abstractobject.h
	${CC} ${CFLAGS} -c $< -o $@

practicalbook.o: object/practicalbook.cpp object/practicalbook.h object/object.h object/field.h object/abstractobject.h
	${CC} ${CFLAGS} -c $< -o $@

record.o: object/record.cpp object/record.h object/object.h object/field.h object/abstractobject.h
	${CC} ${CFLAGS} -c $< -o $@

search.o: object/search.cpp object/abstractobject.h object/search.h
	${CC} ${CFLAGS} -c $< -o $@

content.o: content/content.cpp content/content.h
	${CC} ${CFLAGS} -c $< -o $@

txtcontent.o: content/txtcontent.cpp content/txtcontent.h content/content.h
	${CC} ${CFLAGS} -c $< -o $@

CORE_H=object/*.h object/password/*.h content/*.h

# Server Build targets
.SERVER: database.o server.o

database.o: database.cpp database.h object/user.h object/book.h object/practicalbook.h object/record.h object/object.h object/field.h object/abstractobject.h object/password/password.h object/search.h errorcode.h
	${CC} ${CFLAGS} ${DBFLAGS} -c $< -o $@

SERVER_H=server.h errorcode.h

server.o: server.cpp server.h database.h ${CORE_H}
	${CC} ${CFLAGS} ${DBFLAGS} -c $< -o $@

# Client Build targets
.CLIENT: controller.o client.o

controller.o: controller.cpp controller.h ${CORE_H} ${SERVER_H}
	${CC} ${CFLAGS} ${DBFLAGS} -c $< -o $@

client.o: client.cpp client.h controller.h ${CORE_H}
	${CC} ${CFLAGS} ${DBFLAGS} -c $< -o $@

# Main Build targets
main.o: main.cpp database.h server.h client.h
	${CC} ${CFLAGS} ${DBFLAGS} -c $< -o $@

ALL_O=./*.o
main: all_o
	# ${CC} ${CFLAGS} *.cpp object/*.cpp content/*.cpp object/password/*.cpp -o $@
	${CC} ${ALL_O} ${CFLAGS} ${DBFLAGS} -o $@

test: all_o
	${CC} ${CFLAGS} ${DBFLAGS} -c test.cpp -o test.o
	rm main.o
	${CC} ${ALL_O} ${CFLAGS} ${DBFLAGS} -o $@
	rm test.o



# Clean targets
clean:
	rm *.o
