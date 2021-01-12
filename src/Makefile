.PHONY : all clean

all: queue.o optparse.o tcheck.o

optparse.o: optparse.c optparse.h queue.c queue.h flags.h
	${CC} ${CFLAGS} -c $< -o $@

queue.o: queue.c queue.h flags.h
	${CC} ${CFLAGS} -c $< -o $@

tcheck.o: tcheck.c tcheck.h flags.h
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm *.o
