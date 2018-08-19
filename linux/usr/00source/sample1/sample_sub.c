// sample_sub.c


#include	<stdio.h>

#include	"sample_sub.h"
#include	"TestPt.h"


/*
 * Function foo()
 * Notice if n<10
 */
void	bar(int n)
{
	if(n < 10) {
		puts("n<10");
	} else {
		// Do nothing
	}
}
