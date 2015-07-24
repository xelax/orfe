#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "globals.h"

Sasso *tasca1[MAXDIS];
Sasso *tasca2[MAXDIS];

#define MAXBISACCIA 300
static Sasso *bisaccia[MAXBISACCIA];
 
static int cercaPesoMin (int start)
{
	int minpeso = INT_MAX;
	int minindice = 0;
	int indice = start;
	int bisacciaPiena = 0;

	while (indice < MAXBISACCIA) {
		if (bisaccia [indice] != NULL) {
			bisacciaPiena = 1;
			if (( bisaccia[indice] ->  peso) < minpeso ) {
				minindice = indice;
				minpeso = ( bisaccia[indice] -> peso );  
			}
		} 
		indice++;
	}

	if ( !bisacciaPiena )
		return -1;
	else
		return minindice;
}

void stampaSasso (Sasso *pSasso)
{
	if (pSasso == NULL) {
		printf("stampaSasso: NULL pointer\n");
		return;
	}
	printf("Sasso nodo %d\n",pSasso->nodo);
	printf("Sasso peso %d\n",pSasso->peso);
	printf("Sasso puntato %d\n",pSasso->next->nodo);
	printf("Sasso npunt %d\n\n",pSasso->npunt);
}

void stampaPercorso (Sasso *pSasso)
{
	printf("Inizio\n");
	while (pSasso != NULL) {
		stampaSasso(pSasso);
		pSasso = pSasso->next;
	}
	printf("Fine\n\n");
}

void freeSasso( Sasso *pSassoCor)
{
	Sasso *pSassoFree;
	Sasso *pSassoPros;
	
	
	pSassoFree = pSassoCor;
	while (pSassoFree -> npunt <= 1 ) {
		pSassoPros = pSassoFree -> next;

#ifdef DEBUG
		printf("freeSasso:libero il sasso\n");
		stampaSasso(pSassoFree);
#endif

		free(pSassoFree);
		pSassoFree = pSassoPros;
	}
	pSassoFree -> npunt --;
}

void gettaSasso (Sasso *ptrS)
{
	int i;

	for(i = 0; i < MAXBISACCIA-1; i++)
		if (bisaccia[i] == ptrS) {
			bisaccia[i]=NULL;
			break;
		}

}          

Sasso ** pollicino( int part,int arr)
{
	
	const int TOLL=100;
	const int MAXPERCORSI = 12;
	int i,k,l,m,ramosecco;
	int ciclo;
	int minPeso,percorsi,percorsi1,indGraf,indBis,n,indTasca,maxtol;
	int marca[MAXTRATTI];
	Sasso *pSassoCor;
	Sasso *pSassoPros;
	Sasso **tasca;

	
percorsi = 0;


for (ciclo = 1; ciclo <= 2; ++ciclo ) {
	if (ciclo == 1 ) {
		tasca = tasca1;
		/* pulisce le marche lasciando quelle per i nodi doppi */
		for (i=0; i<MAXTRATTI; i++)
			marca[i] = 0;
	}
	else {
		for (i=0; i < MAXTRATTI;  i++) 
			if (marca[i] == 2 || marca[i] == -2)
				marca[i] = - marca[i];
			else
				marca[i] = 0;

		percorsi1 = percorsi;
		tasca = tasca2;

	}

	pSassoCor =  ( Sasso *) malloc(sizeof (Sasso));
	if (pSassoCor == NULL) {
		printf("pollicino: malloc failed\n");
		return NULL;
	}
	pSassoCor -> nodo = arr;
	pSassoCor -> peso = 0;
	pSassoCor -> next = NULL;
	pSassoCor -> npunt= 0;

	bisaccia[0] = pSassoCor;

	indGraf = percorsi = minPeso = 0;

	/* finche' c'e' qualcosa nella bisaccia */
	while ( (l =  cercaPesoMin(0)) != -1) {
		m =  cercaPesoMin(l+1);

		/* rilevazione nodi doppi adiacenti di peso minimo */
		if ( m != -1 && 
		     bisaccia[l]->peso == bisaccia[m]->peso && 
		     grafoIdx( bisaccia[l]->nodo , bisaccia[m]->nodo) != -1 &&
		     marca[grafoIdx( bisaccia[l]->nodo , bisaccia[m]->nodo)] == 0 ) {

			marca[grafoIdx( bisaccia[l]->nodo , bisaccia[m]->nodo)] = +2;
			marca[grafoIdx( bisaccia[m]->nodo , bisaccia[l]->nodo)] = -2;

		}

		pSassoCor = bisaccia[l];

#ifdef DEBUG		
		printf("Nodo di lavoro\n");
		stampaSasso (pSassoCor);
#endif

		/* eliminazione dei sassi che superano la tolleranza */
		if(!(minPeso == 0)) {
			maxtol = (((minPeso * TOLL)/100) + minPeso);
			if (maxtol < pSassoCor -> peso || percorsi > MAXPERCORSI) {
				freeSasso(pSassoCor);
				gettaSasso(pSassoCor);
				pSassoCor = NULL;
				continue;
			}
		}

		/* controllo arrivo, mette il sasso finale in tasca */
		if (pSassoCor -> nodo == part) {
			percorsi++ ;

			/* metti il sasso in tasca */
			indTasca = 0;
	   		while (indTasca <MAXDIS && tasca[indTasca]!=NULL)
				indTasca++;
			if( indTasca == MAXDIS ) 
				printf("tasca overflow\n");
			else
				tasca[indTasca] = pSassoCor;

#ifdef DEBUG
			printf("percorso definitivo\n");
			stampaSasso (pSassoCor);
#endif		
		
			if (minPeso == 0) {
				minPeso = pSassoCor -> peso;
			}
			gettaSasso(pSassoCor);
			continue;
    		}

		n = grafoFirstIdx(pSassoCor -> nodo);
	
		/* eliminazione rami secchi */
		k = n;
		ramosecco = 1;
		while(grafo[k].x == pSassoCor-> nodo){
			if (marca[k] >= 0)
				ramosecco = 0;
		        k++;
		}	
		if (ramosecco) {
			freeSasso(pSassoCor);
			gettaSasso(pSassoCor);
			continue;
		}

		/* creazione nuovi sassi che si diramano */
		for(indGraf=n; indGraf < ntratti; indGraf++) {
			/* se esiste un tratto e non e' marcato contromano */
			if ( (grafo[indGraf].x == pSassoCor->nodo) &&
			     (marca[indGraf] >= 0)                  ) {
				/* crea il sasso */
				pSassoPros =(Sasso *)malloc(sizeof(Sasso));
				pSassoPros -> nodo = grafo[indGraf].y;
				pSassoPros -> peso = pSassoCor->peso + grafo[indGraf].peso;
				pSassoPros -> next = pSassoCor ;
				pSassoPros -> npunt = 0;
				pSassoCor ->  npunt++;

				/* se non e' marcato, marcalo */
				if( marca[indGraf] == 0) {
					marca[indGraf] = 1;
					marca[grafoIdx((grafo[indGraf].y),(grafo[indGraf].x))] = -1;
				}

				/* cerca un posto nella bisaccia */
				for ( indBis = 0;
				      (indBis < MAXBISACCIA-1) && (!(bisaccia[indBis] == NULL));
				      indBis++
				);
				/* verifica che la bisaccia non debordi */
				if (indBis >= (MAXBISACCIA-1) ) {
					printf("Pollicino: OVERFLOW DI BISACCIA \n ");
					break;
				}
				/* se c'e' spazio metti il sasso in bisaccia */
				else {  
#ifdef DEBUG	 
					printf("METTO IN BISACCIA \n");
					stampaSasso(pSassoPros);
#endif
					bisaccia[indBis] = pSassoPros;
				}
			}
		}
		gettaSasso (pSassoCor);
	}
}
	if (percorsi >= percorsi1) {
/*		printf("Meglio la seconda\n"); */
		tasca = tasca2;
	}
	else
	{
		percorsi = percorsi1;
/*		printf("Meglio la prima\n"); */
		tasca = tasca1;
	}
#ifdef DEBUG1
	/* stampa dei percorsi trovati */
	i = 0;
	printf("\n %d percorsi trovati:\n",percorsi);
	while (tasca[i] != NULL) {
		printf("-------inizio percorso--------\n");
		stampaPercorso (tasca[i++]);
	}
#endif

	return tasca;

}

