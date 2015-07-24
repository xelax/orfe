#include <stdio.h>

/* trucchetto per dichiarare le variabili globali eliminando l'extern */
#define extern
#include "globals.h"

void aspetta (void)
{
	printf("... premi un tasto\n");
	(void) getchar();
}

void main(int argc, char *argv[])
{
	char str[128];
	int ret,n;

	fileToGrafo("grafo.txt");
	grafoToStazioni();
	fileToTreni("treni.txt");
	fileToTab ("tab.txt");
  /*	stampaTabella(); */

	SGR(CLEAR,BLUE,46);
	ED();
	ret = -1;
	while (ret == -1) {
		printf ("Stazione di partenza ?   ");
		scanf ("%s",str);
		sistemaNome (str);
		ret = strtonum (str);
		if (ret == -1 ) {
			printf ("Non conosco la stazione %s\n\n",str);
		}
	}
	part = ret;

	ret = -1;
	while (ret == -1) {
		printf ("Stazione di arrivo ?   ");
		scanf ("%s",str);
		sistemaNome (str);
		ret = strtonum (str);
		if (ret == -1 ) {
			SGR(CLEAR,RED,46);
			printf ("Non conosco la stazione %s\n\n",str);
		}
	}
	arr = ret;

	SGR(CLEAR,BLUE,46);
	printf (" Scegli la fascia oraria :\n");
	printf (" 1) dalle  0:00  alle   6:00 \n");
	printf (" 2) dalle  6:00  alle  12:00 \n");
	printf (" 3) dalle 12:00  alle  18:00 \n");
	printf (" 4) dalle 18:00  alle  24:00 \n");
	printf ("\n        ------>  ");


	ret = 0;
	n=0;
	while (ret != 1 || n <= 0 || n> 4) {
		if ((ret = scanf ("%d",&n)) != 1) {
			scanf("%*s");
			SGR(INTER,RED,46);
			printf(" Fascia errata, riprova");
		}
	}

	inizioFascia = (n-1) * 360;
	fineFascia   =  n    * 360;;

	aspetta();
	SGR(0,0,0);
	ED();
	faiPercorsi(pollicino(part,arr));
	SGR(0,0,0);
}
