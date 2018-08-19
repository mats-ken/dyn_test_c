void	foo(void)
{
	for (struct xxx x = {1, 2}; x.a < x.b; x.a++) {
		bar(&x);
	}
}
