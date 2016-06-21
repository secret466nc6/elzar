#include <string.h>
#include <stdint.h>
#include <limits.h>

#ifdef PRINTDEBUG
#include <stdio.h>
#endif

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t my_strlen(const char *s)
{
#ifdef PRINTDEBUG
    static int printdebugnum = 0;
    printf("[strlen : %6d]\n", printdebugnum++);
#endif

	const char *a = s;
	const size_t *w;
	for (; (uintptr_t)s % ALIGN; s++) if (!*s) return s-a;
	for (w = (const void *)s; !HASZERO(*w); w++);
	for (s = (const void *)w; *s; s++);
	return s-a;
}
