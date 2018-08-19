void foo(void)
{
#ifdef	DEBUG
	if(a &&
#else
	if(b &&
#endif
	        c) { TEST_PT_IF(5,0,18); EXECNT=C1ZERO;
		bar();
	}
	if(x < 0) { TEST_PT_IF(5,1,19); EXECNT=C1ZERO;
		bar();
	}
}
