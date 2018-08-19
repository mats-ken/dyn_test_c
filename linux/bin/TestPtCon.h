#include	<stdio.h>


#ifndef	__FUNCTION__
  #define	__FUNCTION__	"FUNC_NOT_DEFINED"
#endif


#if	1	// Full version (output to file)

#define	TEST_PT(KW, FL, NO, TN, CNT)	do\
{	\
	while(TEST_LOCK){}	\
	TEST_LOCK = true;	\
	FILE*fp = fopen("test.log", "at");	\
	fprintf(fp, "TEST_PT:\t%d\t%d\t%d\t%d\t%d\t" __FILE__ "\t" __FUNCTION__ "\t" KW "\n", (FL)* 65536 + (NO), (FL), (NO), (TN), __LINE__);	\
	fclose(fp);	\
	TEST_LOCK = false;	\
}	while(0)

#elif	0	// Full version

#define	TEST_PT(KW, FL, NO, TN, CNT)	printf("TEST_PT:\t%d\t%d\t%d\t%d\t%d\t" __FILE__ "\t" __FUNCTION__ "\t" KW "\n", (FL) * 65536 + (NO), (FL), (NO), (TN), __LINE__)

#elif	0	// Lite version

#define	TEST_PT(KW, FL, NO, TN, CNT)	printf("TEST\t%d\t%d\t%d\t%d\n", (FL) * 65536 + (NO), (FL), (NO), (TN))

#else	// Minimum version

#define	TEST_PT(KW, FL, NO, TN, CNT)	printf("TEST\t%d\n", (FL) * 65536 + (NO))

#endif


#define	PT_FUNC		"func"
#define	PT_IF		"if"
#define	PT_FOR		"for"
#define	PT_WHILE	"while"
#define	PT_DO		"do"
#define	PT_CASE		"case"

#define	C1ZERO	0


extern	bool	TEST_LOCK;
