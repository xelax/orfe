#define extern
#include "globals.h"

void main(int argc, char *argv[])
{
	int linea, dir;

	if (argc != 2) {
		printf ("USO: %s file_di_salvataggio\n",argv[0]);
		return;
	}

	fileToGrafo("grafo.txt");
	grafoToStazioni();
	fileToTreni("treni.txt");

	fileToTab(argv[1]);

	for(;;) {
		printf("Inserisci il numero di linea e di direzione  -> ");
		if (scanf("%d%d",&linea,&dir) != 2) {
			printf("\n\n---- Fine inserimento, salvo il file %s ----\n",argv[1]);
			break;
		}
		else
			faiOrario(linea,dir);
	}

	tabToFile (argv[1]);

/*	stampaTabella(); */
}
