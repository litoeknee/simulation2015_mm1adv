mm1alt: mm1alt.o lcgrand.o simplerandom.o
	cc -o mm1alt mm1alt.o lcgrand.o simplerandom.o

test: test.o simplerandom.o
	cc -o test test.o simplerandom.o

test.o: test.c
	cc -c test.c

simplerandom.o:	simplerandom.c simplerandom-c.h
	cc -c simplerandom.c

lcgrand.o: lcgrand.c lcgrand.h
	cc -c lcgrand.c

mm1alt.o: mm1alt.c lcgrand.h
	cc -c mm1alt.c

clean:
	rm mm1alt test mm1alt.o lcgrand.o test.o simplerandom.o