void	foo(void)
{ TEST_PT_FUNC(2,0,19); EXECNT=C1ZERO;
	if (x < 0) do { TEST_PT_DO(2,1,20); EXECNT=C1ZERO;
			x = bar();
		} while (x < 0);
}
