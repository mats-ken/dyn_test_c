// sample_main.c


#include	<stdio.h>
#include	<stdlib.h>

#include	"TestPt.h"


#include	"sample_sub.h"


bool	TEST_LOCK;
#ifdef	__cplusplus
  std::map<int, int>	TEST_FREQ;
#else
  int		TEST_FREQ[PT_MAX];
#endif	// __cplusplus


void	foo(void)
{
	for(int i = 0; i < 23; i++) {
		bar(i);	// Call foo()
		if(i < 5) {
			switch(i) {
			case 0:		puts("i==0");	break;
			case 1:		puts("i==1");	break;
			case 2:		puts("i==2");	break;
			case 3:		puts("i==3");	break;
			case 4:		puts("i==4");	break;
			case 5:		puts("i==5");	break;	// Never executed
			default:	puts("default");	break;	/* Never executed */
			}
		} else if(i < 15) {
			puts("i<15");
		} else if(i < 25) {
			puts("i<25");
		} else if(i < 35) {
			puts("i<35");	/* Never executed */
		} else {
			puts("else");	// Never executed
		}

		if((i < 10) && (20 < i)) {	// ||‚Æ&&‚ð‘‚«ŠÔˆá‚¦‚Ä‚¢‚é
			puts("OK");	// Never executed
		}
	}
}

#ifndef	_MSC_VER

// Main routine
int		main(void)
{
	foo();

	return	EXIT_SUCCESS;
}

#endif	// _MSV_VER
