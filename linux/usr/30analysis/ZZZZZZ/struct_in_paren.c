void	foo(void)
{
	for (struct xxx x = { TEST_PT_FOR(4,0,17); EXECNT=C1ZERO;1, 2}; x.a < x.b; x.a++) {
		bar(&x);
	}
}
