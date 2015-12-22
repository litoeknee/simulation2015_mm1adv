mm1alt: mm1alt.o lcgrand.o
	cc -o mm1alt mm1alt.o lcgrand.o

lcgrand.o: lcgrand.c lcgrand.h
	cc -c lcgrand.c

mm1alt.o: mm1alt.c lcgrand.h
	cc -c mm1alt.c

clean:
	rm mm1alt mm1alt.o lcgrand.o