#include	<unistd.h>


#define	GPIO_OUT_4_BIT(Z)			do{	\
	digitalWrite(0, 0);	\
	digitalWrite(1,  (Z)        & 1);	\
	digitalWrite(2, ((Z) >>  1) & 1);	\
	digitalWrite(3, ((Z) >>  2) & 1);	\
	digitalWrite(4, ((Z) >>  3) & 1);	\
	digitalWrite(0, 1);	\
	usleep(1);	\
	digitalWrite(0, 0);	\
	digitalWrite(1, ((Z) >>  4) & 1);	\
	digitalWrite(2, ((Z) >>  5) & 1);	\
	digitalWrite(3, ((Z) >>  6) & 1);	\
	digitalWrite(4, ((Z) >>  7) & 1);	\
	digitalWrite(0, 1);	\
	usleep(1);	\
	digitalWrite(0, 0);	\
	digitalWrite(1, ((Z) >>  8) & 1);	\
	digitalWrite(2, ((Z) >>  9) & 1);	\
	digitalWrite(3, ((Z) >> 10) & 1);	\
	digitalWrite(4, ((Z) >> 11) & 1);	\
	digitalWrite(0, 1);	\
	usleep(1);	\
	digitalWrite(0, 0);	\
	digitalWrite(1, ((Z) >> 12) & 1);	\
	digitalWrite(2, ((Z) >> 13) & 1);	\
	digitalWrite(3, ((Z) >> 14) & 1);	\
	digitalWrite(4, ((Z) >> 15) & 1);	\
	digitalWrite(0, 1);	\
} while(0)

#define	TEST_PT_IF(X, Y, Z)		GPIO4OUT(Z)
#define	TEST_PT_FOR(X, Y, Z)	GPIO4OUT(Z)
#define	TEST_PT_WHILE(X, Y, Z)	GPIO4OUT(Z)
#define	TEST_PT_DO(X, Y, Z)		GPIO4OUT(Z)
#define	TEST_PT_SWITCH(X, Y, Z)	GPIO4OUT(Z)


extern	int		EXECNT = 0;
#define	C1ZERO	0
