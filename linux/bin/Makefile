CC=gcc
CFLAGS=-std=gnu99

CP=g++
CPFLAGS=

all:	bstyle InsertTestPts histogramme InsertCoverage

bstyle:	bstyle.cpp
	$(CP) $(CPFLAGS) -o bstyle bstyle.cpp

InsertTestPts:	InsertTestPts.cpp
	$(CP) $(CPFLAGS) -o InsertTestPts InsertTestPts.cpp

histogramme:	histogramme.cpp
	$(CP) $(CPFLAGS) -o histogramme histogramme.cpp

InsertCoverage:	InsertCoverage.cpp
	$(CP) $(CPFLAGS) -o InsertCoverage InsertCoverage.cpp


clean:
	rm -f InsertTestPts histogramme InsertCoverage
