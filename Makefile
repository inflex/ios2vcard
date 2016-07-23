# 
# VERSION CHANGES
#

LOCATION=/usr/local
CFLAGS=-Wall -g -I. -O2 
LIBS=-lsqlite3

OBJ=ios2vcard
OFILES=
default: ios2vcard

.c.o:
	${CC} ${CFLAGS} $(COMPONENTS) -c $*.c

all: ${OBJ} 

ios2vcard: ${OFILES} ios2vcard.c 
#	ctags *.[ch]
#	clear
	${CC} ${CFLAGS} $(COMPONENTS) ios2vcard.c ${OFILES} -o ios2vcard ${LIBS}

install: ${OBJ}
	cp ios2vcard ${LOCATION}/bin/

clean:
	rm -f *.o *core ${OBJ}
