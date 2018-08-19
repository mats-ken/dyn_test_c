all:	logger_main

logger_main:	logger_main.cpp gpioPorts.h
	gcc -o logger_main logger_main.cpp -lwiringPi

clean:
	rm -f logger_main
