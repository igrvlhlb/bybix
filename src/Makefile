.PHONY : all clean

all: queue.o optparse.o

optparse.o: optparse.c optparse.h queue.* flags.h
	${CC} ${CFLAGS} -c $< -o $@

queue.o: queue.c queue.h flags.h
	${CC} ${CFLAGS} -c $< -o $@

tcheck.o: tcheck.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm *.o
