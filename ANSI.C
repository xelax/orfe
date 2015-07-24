/* ansi.c by Alex Cozzi */
/* tutte le funzioni della libreria ANSI non comprese nella libreria lattice */
/* created 5/7/1990	*/

#include <stddef.h>
/*
* bsearch	ricerca binaria su un array , last change: 6/7/1990
*			fa dei giochini molto sporchi con i puntatori ma funziona.
*/
void *bsearch (const void *key, void *base, size_t n, size_t size,
			 int (*cmp) (const void *, const void *))
{
	int  cond;
	char *low, *mid, *high;

	low = base;
	high = low + n * size;
	while (low < high) {
		mid = low + (((high-low)/size) / 2)*size;

		if ( (cond = cmp (key, mid)) < 0)
			high = mid;
		else if (cond > 0)
			low = mid + 1 * size;
		else {
			return mid;
		}
	}
	return NULL;
}
