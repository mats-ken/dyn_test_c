void foo(void)
{
#ifdef	DEBUG
	if(a &&
#else
	if(b &&
#endif
	        c) {
		bar();
	}
	if(x < 0) {
		bar();
	}
}
