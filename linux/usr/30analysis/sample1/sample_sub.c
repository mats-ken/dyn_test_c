// sample_sub.c


#include	<stdio.h>

#include	"sample_sub.h"
#include	"TestPt.h"


/*
 * Function foo()
 * Notice if n<10
 */
void	bar(int n)
{ TEST_PT_FUNC(1,0,16); EXECNT=23;
	if(n < 10) { TEST_PT_IF(1,1,17); EXECNT=10;
		puts("n<10");
	} else { TEST_PT_IF(1,2,18); EXECNT=13;
		// Do nothing
	}
}
