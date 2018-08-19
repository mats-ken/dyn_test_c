// sample_main.c


#include	<stdio.h>
#include	<stdlib.h>

#include	"TestPt.h"


#include	"sample_sub.h"


int		EXECNT = 0;
bool	TEST_LOCK;


void	foo(void)
{ TEST_PT_FUNC(0,0,0); EXECNT=C1ZERO;
	for(int i = 0; i < 23; i++) { TEST_PT_FOR(0,1,1); EXECNT=C1ZERO;
		bar(i);	// Call foo()
		if(i < 5) { TEST_PT_IF(0,2,2); EXECNT=C1ZERO;
			switch(i) {
			case 0: TEST_PT_SWITCH(0,3,3); EXECNT=C1ZERO;
				puts("i==0");
				break;
			case 1: TEST_PT_SWITCH(0,4,4); EXECNT=C1ZERO;
				puts("i==1");
				break;
			case 2: TEST_PT_SWITCH(0,5,5); EXECNT=C1ZERO;
				puts("i==2");
				break;
			case 3: TEST_PT_SWITCH(0,6,6); EXECNT=C1ZERO;
				puts("i==3");
				break;
			case 4: TEST_PT_SWITCH(0,7,7); EXECNT=C1ZERO;
				puts("i==4");
				break;
			case 5: TEST_PT_SWITCH(0,8,8); EXECNT=C1ZERO;
				puts("i==5");
				break;	// Never executed
			default: TEST_PT_SWITCH(0,9,9); EXECNT=C1ZERO;
				puts("default");
				break;	/* Never executed */
			}
		} else if(i < 15) { TEST_PT_IF(0,10,10); EXECNT=C1ZERO;
			puts("i<15");
		} else if(i < 25) { TEST_PT_IF(0,11,11); EXECNT=C1ZERO;
			puts("i<25");
		} else if(i < 35) { TEST_PT_IF(0,12,12); EXECNT=C1ZERO;
			puts("i<35");	/* Never executed */
		} else { TEST_PT_IF(0,13,13); EXECNT=C1ZERO;
			puts("else");	// Never executed
		}

		if((i < 10) && (20 < i)) { TEST_PT_IF(0,14,14); EXECNT=C1ZERO;	// ||‚Æ&&‚ð‘‚«ŠÔˆá‚¦‚Ä‚¢‚é
			puts("OK");	// Never executed
		}
	}
}

#ifndef	_MSC_VER

// Main routine
int		main(void)
{ TEST_PT_FUNC(0,15,15); EXECNT=C1ZERO;
	foo();

	return	EXIT_SUCCESS;
}

#endif	// _MSV_VER
