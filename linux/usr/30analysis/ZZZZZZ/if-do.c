void	foo(void)
{
	if (x < 0) do { TEST_PT_DO(2,0,16); EXECNT=C1ZERO;
			x = bar();
		} while (x < 0);
}
