CC=gcc
CFLAGS=-std=gnu99

CP=g++
CPFLAGS=

all:	sample

sample_sub.o:	TestPt.h sample_sub.c
	$(CP) $(CPFLAGS) -c sample_sub.c

sample_main.o:	TestPt.h sample_main.c
	$(CP) $(CPFLAGS) -c sample_main.c

sample:	sample_main.o sample_sub.o
	$(CP) -o sample sample_main.o sample_sub.o


clean:
	rm -f sample sample_main.o sample_sub.o
