#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

int volatile memvar;

void
foo ()
{
	int i;

	for (i = 0; i < 1000000; i++)
		memvar++;
}

void
bar ()
{
	int i;

	for (i = 0; i < 2000000; i++)
		memvar++;
}

void
handler ()
{
	int i;

	for (i = 0; i < 3000000; i++)
		memvar++;
}

void library_dispatcher (void (*func)(void))
	__attribute__ ((no_instrument_function));

void
library_dispatcher (void (*func)(void))
{
	(*func)();
}

void
chain1 (void)
{
	int i;
	for (i = 0; i < 4000000; i++)
		memvar++;
}

void
chain2 (void)
{
	int i;
	for (i = 0; i < 1000000; i++)
		memvar++;
	library_dispatcher (chain1);
}

void
sqrt_test (double x)
{
	memvar = sqrt (x);
	bar ();
}

int
main ()
{
	foo ();
	library_dispatcher (handler);
	bar ();
	library_dispatcher (chain2);
	bar ();
	sqrt_test (2);
	bar ();
	return (0);
}
