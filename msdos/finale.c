#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"

typedef struct {
	int x;
	int y;
	int linea;
	int verso;
} lineaCapi;

typedef struct lampo {
	int nPercorso;
	lineaCapi *spez;
	struct lampo *lamposeg;
} Lampo;

#define MAXSPEZZATA 40
#define MAXTRENINI   4
#define TEMPOCAMBIO  2
#define TEMPOCRITICO 5

extern int part, arr;
extern int inizioFascia, fineFascia;
static Tabella tabcor;
/* variabile globale settata da stampa */
static int percorsi;


/* costruisce una singola spezzata ottimizzata */
lineaCapi *costruisci(Sasso **tasca,int c)
{
	lineaCapi spezzata[MAXSPEZZATA], *ptr;
	Sasso *pcor;
	Sasso *ppros;
	int nodoCorrente;
	int nodoSeguente,n,line,indspez,way;

	line = 0;
	indspez = way = -1;
	pcor = tasca[c];
	ppros=pcor -> next;

	while(ppros != NULL){
		nodoCorrente=pcor -> nodo;
		nodoSeguente=ppros -> nodo;
		n= grafoIdx ( nodoCorrente,nodoSeguente );
		if ( line != grafo[n].linea ){
			indspez++;
			spezzata[indspez].x = nodoCorrente;
			spezzata[indspez].y = nodoSeguente;
			line = grafo[n].linea;
			way  = grafo[n].verso;
			spezzata[indspez].linea = line;
			spezzata[indspez]. verso = way;
		}
		else {
			spezzata[indspez]. y = nodoSeguente;
			way = grafo[n].verso ;
			if ( way != spezzata[indspez].verso)
				printf("COSTRUISCI: versi non rispettati \n");
		}
		pcor = ppros;
		ppros = ppros -> next;
	}
	ptr = calloc(sizeof (lineaCapi),indspez+1);
	memcpy(ptr, spezzata, (indspez+1) * sizeof(lineaCapi));
	return ptr;
}



/*ottimizza i percorsi contenuti in tasca eliminando le stazioni intermedie */
/*sulla medesima linea	*/

Lampo *ottimizzaCollegamenti (Sasso **tasca)
{
	Lampo *pLampoCor,*lampi;
	int c;

	lampi = NULL;
	for(c=(MAXDIS -1);c >= 0;c--) {
		if ( tasca[c] == NULL )
			continue;
		pLampoCor = (Lampo *) malloc (sizeof(Lampo));
		pLampoCor -> nPercorso = (c + 1);
		pLampoCor -> lamposeg = lampi;
		pLampoCor -> spez = costruisci(tasca,c);
		lampi = pLampoCor;

	}
	return lampi;
}

/* Dando il numero della linea restituisce il puntatore alle relativa */
/*tabella							      */

Tabella *cercatabella (int linea )
{
	int i;

	for (i=0;i < MAXLINEE; i++)
		if (tabella[i].linea == linea)
			return (&tabella[i]);
	printf("CERCATABELLA: Non ho trovato la tabella richiesta!\n");
	return NULL;
}

/* dando il numero della stazione restituisce il suo indice nella */
/* tabella */

int cercastazioni (int numeroStazione)
{
	int i;
	for(i=0;i < tabcor.stazioni;i++)
		if(numeroStazione == tabcor.stazione[i])
			return i;

	printf("CERCASTAZIONI:Non ho trovato %d \n", numeroStazione);
	return -1;
}

/* restituisce -1 se non trova alcun treno nell'arco delle 24h con le opz */
/*  scandendo la riga indiceStazione restituisce l'indice della */
/* colonna in cui compare il I orario verificante le opzioni.*/

int cepitreopz (int orario,int indiceStazione,int verso,int opzioni)
{
	int j,i,trenoDiUtilizzo,indtrediuti, opTreno;
	int fOrario = orario;

	for(j=0; tabcor.orario[verso][j][indiceStazione] < fOrario; j++)
		if (j == tabcor.treni[verso] - 1 ) {
			j=0;
			break;
		}

	i=j;
	while (i != (j-1)) {
		trenoDiUtilizzo=tabcor.treno[verso][i];
		indtrediuti= trenoIdx (trenoDiUtilizzo,tabcor.linea, verso);
		opTreno=treno[indtrediuti].opz;
		if ( (opzioni & opTreno) == opzioni)
			return i;
		else if (i == (tabcor.treni[verso] - 1) )
			i = 0;
		else
			i++;
	}
	/* esce per i==(j-1) */
	trenoDiUtilizzo = tabcor.treno[verso][i];
	indtrediuti = trenoIdx (trenoDiUtilizzo,tabcor.linea, verso);
	opTreno=treno[indtrediuti].opz;
	if ((opzioni & opTreno) == opzioni)
		return i;
	else if (tabcor.stazione[indiceStazione] == part) {
		SGR(CLEAR,RED,40);
		printf("Non esistono treni su questo percorso con le opzioni specificate\n");
		return -1;
	}
	else {
		SGR(CLEAR,RED,40);
		printf("Da questo punto in poi non esistono coincidenze che rispettano ");
		printf("le opzioni richieste\n");
		return -1;
	}
}

void printa(int inizio,int fine,int jj,int verso)
{
	int nInizio,nFine,oraPart,oraArr;
	int nTreno,indtrediuti;
	char tipoTreno;

	nInizio = cercastazioni(inizio);
	nFine	= cercastazioni(fine);
	oraPart = tabcor.orario[verso][jj][nInizio];
	oraArr	= tabcor.orario[verso][jj][nFine];
	nTreno = tabcor.treno[verso][jj];
	indtrediuti =trenoIdx (nTreno,tabcor.linea, verso);
	tipoTreno = treno[indtrediuti].tipo;

	SGR(CLEAR,CYAN,40);
	printf("\n %-15s ",citta[inizio]);
	SGR(GRASS,WHITE,40);
	stampaOra  (oraPart);
	SGR(CLEAR,WHITE,40);
	printf(" ---| ");
	SGR(CLEAR,GREEN,40);
	printf(" %6d",nTreno);
	switch (tipoTreno)
	{
		case 'E':
			SGR(GRASS,GREEN,40);
			printf(" EUROCITY  ");
		break;
		case 'I':
			SGR(GRASS,GREEN,40);
			printf(" INTERCITY ");
		break;
		case 'X':
			SGR(CLEAR,GREEN,40);
			printf(" ESPRESSO  ");
		break;
		case 'D':
			SGR(CLEAR,GREEN,40);
			printf(" DIRETTO   ");
		break;
		case 'L':
			SGR(CLEAR,GREEN,40);
			printf(" LOCALE    ");
		break;
		case 'A':
			SGR(CLEAR,GREEN,40);
			printf(" AUTOBUS   ");
		break;
		default:
			printf("\nIL TIPO DEL TRENO NON E' RICONOSCIUTO\n");
	}
	SGR(CLEAR,WHITE,40);
	printf(" |--> ");
	SGR(CLEAR,CYAN,40);
	printf("%-15s  ",citta[fine]);
	SGR(GRASS,WHITE,40);
	stampaOra (oraArr);
	printf("\n");

}

/* Stampa tutti i trenini esistenti su questo percorso entro la fascia */
/*fino ad un max di MAXTRENINI se l'argomento 'trenini'  >=0, ne stampa*/
/* uno solo se il suddetto argomento  < 0  */

int StampaTrenino (lineaCapi *trenino,int trenin,int opzioni)
{
	int i,orariodipart,linea,inizio,fine,verso,primorapart;
	int nInizio,inizioRicerca,nFine,j;
	int orarioFineRiferimento;
	int ultimo;
	Tabella *ptr;
	int trenini;
	trenini = trenin;

	orariodipart = inizioFascia;

	if (opzioni != 0) {
			printf("Il primo treno che verifica le opzioni richieste ");
			printf("in questo percorso : \n");
	}

	while(trenini != -1) {
		ultimo = -1;
		for(i=0; i < MAXSPEZZATA  && ultimo != arr; ultimo = trenino[i++].y) {
			linea = trenino[i].linea;
			inizio= trenino[i].x;
			fine  = trenino[i].y;
			verso = trenino[i].verso;
			ptr = cercatabella(linea);
			if (ptr == NULL) {
				printf ("non ho trovato la linea %d\n",linea);
				return -1;
			}
			tabcor = *ptr;

			/* cerca gli indici dentro la tabella delle stazioni */

			nInizio=cercastazioni (inizio);
			nFine  =cercastazioni (fine);

		 /* situazione di primo tratto di spezzata */

			if (inizio == part) {
				j = cepitreopz(orariodipart,nInizio,verso,opzioni);

				/*se non hai vie di scampo cambia percorso*/
				if(j == -1) {
					return trenini;
				}

				primorapart = tabcor.orario[verso][j][nInizio];

				orariodipart = primorapart + 1;
				orarioFineRiferimento = fineFascia;

				if (primorapart > orarioFineRiferimento ) {
					SGR(CLEAR,RED,40);
					printf("Non esistono altri treni lungo questo ");
					printf("percorso entro la fascia richiesta\n");
					return trenini;
				}
				printa(inizio,fine,j,verso);

			  /* orario di start per la ricerca al primo cambio */

				inizioRicerca = tabcor.orario[verso][j][nFine] + TEMPOCAMBIO ;
				continue;
			}
			else {

			/* situazione di un tratto != primo di spezzata */

				j = cepitreopz(inizioRicerca,nInizio,verso,opzioni);
				/* Se non hai vie di scampo cambia percorso */
				if (j == -1) {
					 return trenini;
				}
				primorapart = tabcor.orario[verso][j][nInizio];
				if ( (primorapart+MDAY - inizioRicerca) % MDAY < TEMPOCRITICO )  {
					SGR(INTER,RED,40);
					printf("\nHai poco tempo per il cambio\n");
				}
				printa(inizio,fine,j,verso);
				inizioRicerca=tabcor.orario[verso][j][nFine] + TEMPOCAMBIO;
			}
		}
		if (ultimo == arr) {
			trenini++;
			SGR(GRASS,MAGEN,40);
			printf("    ----------------------------------------------------------------\n\n");
			aspetta();
			return trenini;
		}
		else {
			printf("GRAVE ERRORE:Sono uscito durante una spezzata\n");
			return -1;
		}
	}
	return trenini;
}


/*stampa output generale prendendo un lampo(percorso) alla volta */

void stampa (Lampo *lampi, int opCliente)
{
	Lampo *orarLampi;
	lineaCapi *trenino;
	int ntrenini;
	int itinerario=0;

	orarLampi=lampi;

	for(ntrenini=0; (ntrenini < MAXTRENINI) && (orarLampi != NULL); orarLampi = orarLampi -> lamposeg) {
		itinerario++;
		SGR(GRASS,YELLW,40);
		printf("\n\n----------------------******* PERCORSO N. %d ******---------------------\n",itinerario);
		trenino = orarLampi -> spez;
		ntrenini = StampaTrenino(trenino,ntrenini,0);
		if (opCliente != 0){
			SGR(GRASS,MAGEN,40);
			StampaTrenino(trenino,-2,opCliente);
		}
	}

	if ( ntrenini >= MAXTRENINI) {
		/*	
		SGR(GRASS,MAGEN,40);
		printf(" Ho trovato %d trenini\n",trenini);
		*/
	}
	else {
		/*
		SGR(CLEAR,MAGEN,40);
		printf(" Non esistono altri percorsi e neppure altri");
		printf(" orari entro la tolleranza del peso\n");
		*/
	}
	percorsi = itinerario;
}

/* stampa il biglietto avendo selezionato il percorso in numeroPercorso */
void stampaBiglietto (Lampo *lampi,Sasso **tasca, int numeroPercorso)
{
	Lampo *lampus;
	Sasso *punTasca;
	int numeroIndiceTasca,kilo;
	int prezzo;

	lampus=lampi;
	SGR(CLEAR,YELLW,40);
	ED();
	while (lampus != NULL && numeroPercorso != lampus->nPercorso)
		lampus = lampus->lamposeg;

	if (lampus == NULL ) {
		printf("Il percorso digitato non esiste!\n");
	}
	else {
		numeroIndiceTasca = ((lampus -> nPercorso) - 1);
		punTasca = tasca[numeroIndiceTasca];

		while (punTasca != NULL && punTasca->nodo != part)
			punTasca = punTasca -> next;

		if( punTasca == NULL ) {
			printf("Stampa biglietto:C'e' un errore nel percorso");
			printf(" contenuto in 'tasca'\n");
		}
		else {
			kilo = punTasca -> peso;
			prezzo =
			 ((200 + kilo*150 -(int)pow((double)kilo,1.2)*20)/100)*100;
			SGR(GRASS,BLACK,47);
			ED();
			printf("\n\nSTAZIONE DI PARTENZA:");
			SGR(GRASS,RED,47);
			printf(" %s \n\n\n",citta[part]);
			SGR(GRASS,BLACK,47);
			printf("STAZIONE DI ARRIVO  :");
			SGR(GRASS,RED,47);
			printf(" %s \n\n\n",citta[arr]);
			SGR(GRASS,BLACK,47);
			printf("DISTANZA :  Km   %d \t\t\t L. %d \n",kilo,abs(prezzo));
			SGR(INTER,MAGEN,47);
			printf("\n        #    B U O N   V I A G G I O   #   \n\n");
			aspetta();
		}
	}
}


void stampaOpzioni (Sasso **tasca, int numeroPercorso)
{
	char str[MAXC];
	int n;
	int altreopz = 1;
	int prenotazione, supplementoPercorso,npartsup,narrsup,tiposup;
	int numeroIndiceTasca, pesoPartenza,pesoArrivo;
	Sasso *punTasca;
	unsigned long int totale = 0;
	unsigned long int prezzoCuccette = 0;
	unsigned long int prezzoIntercity = 0;
	unsigned long int prezzoEurocity = 0;
	int km = 0;


	SGR(CLEAR,CYAN,40);
	ED();
	printf("\n\nDESIDERA PRENOTARE UN POSTO A SEDERE ? (S / N)   ");

	if (leggiSi())
		prenotazione = 1;
	else
		prenotazione = 0;

	SGR(CLEAR,CYAN,40);
	printf("\n\nLungo il percorso da lei scelto esiste un tratto che prevede ");
	SGR(CLEAR,CYAN,40);
	printf("un incremento di prezzo quali : EUROCITY, INTERCITY o CUCCETTE ? (S / N)  ");

	if (leggiSi())
		supplementoPercorso = 1;
	else
		supplementoPercorso = 0;

	if (supplementoPercorso ) {
		SGR(CLEAR,CYAN,40);
		printf("La preghiamo di compilare il seguente quesito riguardo \n");
		SGR(CLEAR,CYAN,40);
		printf("il tratto da lei scelto coperto dalle opzioni : \n");

		numeroIndiceTasca = numeroPercorso - 1;

		while(altreopz) {
			for (;;) {
				SGR(CLEAR,CYAN,40);
				printf("STAZIONE DI PARTENZA : ");
				scanf ("%s",&str);
				sistemaNome (str);
				npartsup = strtonum(str);

				punTasca = tasca [numeroIndiceTasca];

				while ( punTasca != NULL && punTasca -> nodo != npartsup)
					punTasca = punTasca -> next;
				if (punTasca == NULL) {
					SGR(CLEAR,RED,40);
					printf("Errore: la stazione specificata non e' sul percorso, riprova\n");
					continue;
				}
				else {
					pesoPartenza = punTasca -> peso;
					break;
				}
			}

			for (;;) {
				SGR(CLEAR,CYAN,40);
				printf("STAZIONE DI ARRIVO : ");
				scanf ("%s",str);
				sistemaNome (str);
				narrsup = strtonum(str);

				punTasca = tasca [numeroIndiceTasca];

				while ( punTasca != NULL && punTasca -> nodo != narrsup)
					  punTasca = punTasca -> next;
				if (punTasca == NULL) {
					SGR(CLEAR,RED,40);
					printf("Errore: la stazione specificata non e' sul percorso, riprova\n");
					continue;
				}
				else {
					pesoArrivo = punTasca -> peso;
					break;
				}
			}

			for (;;) {
				SGR(CLEAR,CYAN,40);
				printf("TIPO DI SUPPLEMENTO :\n");
				SGR(CLEAR,CYAN,40);
				printf("  1) Cuccette\n");
				SGR(CLEAR,CYAN,40);
				printf("  2) Intercity\n");
				SGR(CLEAR,CYAN,40);
				printf("  3) Eurocity\n");
				SGR(CLEAR,CYAN,40);
				printf("  1-3 ? ");
				if (scanf("%d",&n) != 1)
					scanf ("*s");
				if (n==1)
					tiposup = 100;
				else if (n==2)
					tiposup = 150;
				else if (n==3)
					tiposup = 200;
				else {
					SGR(CLEAR,CYAN,40);
					printf("puoi inserire solo un numero da 1 a 3\n");
					continue;
				}
				break;
			}

			km = abs(pesoArrivo - pesoPartenza);

			if ( tiposup == 100)
				prezzoCuccette += (km * tiposup);
			else if (tiposup == 150)
				prezzoIntercity += (km * tiposup);
			else
				prezzoEurocity	+= (km * tiposup);
			SGR(CLEAR,CYAN,40);
			printf("Hai altri supplementi sulla linea ? (S / N)");
			if (leggiSi())
				altreopz = 1;
			else
				altreopz = 0;
		}
	}
	prezzoCuccette  = (prezzoCuccette  /100)*100;
	prezzoEurocity  = (prezzoEurocity  /100)*100;
	prezzoIntercity = (prezzoIntercity /100)*100;

	if(prezzoCuccette + prezzoIntercity + prenotazione != 0) {

		SGR(GRASS,RED,43);
		ED();
		printf("\n\n\n      ---------- S U P P L E M E N T I --------\n\n\n");

		SGR(CLEAR,BLACK,43);

		if (prezzoCuccette != 0)  {
			printf("Supplemento Cuccette :  %lu \n\n",prezzoCuccette);
		}
		if (prezzoIntercity != 0)   {
			printf("Supplemento Intercity :  %lu \n\n",prezzoIntercity);
		}
		if (prezzoEurocity != 0) {
			printf("Supplemento Eurocity :  %lu \n\n",prezzoEurocity);
		}
		if (prenotazione != 0) {
			printf("Supplemento Prenotazione :  5000 \n\n");
		}
		totale = prenotazione * 5000 + prezzoCuccette +prezzoIntercity+prezzoEurocity;
		printf("\t\t\t TOT. supplementi :  %lu\n",totale);
		printf("\n\n\n\n      B U O N   V I A G G I O   \n\n\n\n");
		aspetta();
	}
	else   {
		SGR(INTER,YELLW, 40);
		ED();
		printf("\n\n\n\n    G R A Z I E    A R R I V E D E R C I \n\n");
		aspetta();
	}
}


/* stampa tutte le spezzate puntate da lampi */

void stampaLampi (Lampo *lampi)
{
	Lampo *printLampi;
	lineaCapi *printspez;
	int i;

	printLampi =lampi;

	while( printLampi != NULL ){
		printf("\nNumero percorso ottimizzato : %d\n", printLampi->nPercorso);
		printspez = printLampi->spez;

		for(i=0; (i != MAXSPEZZATA) && (printspez[i-1].y != arr); i++) {
			printf("Nodo di part-linea %d\n", printspez[i].x);
			printf("Nodo di arr -linea %d\n", printspez[i].y);
			printf("Linea utilizzata   %d\n", printspez[i].linea);
			printf("Verso utilizzato   %d\n", printspez[i].verso);
		}
		printLampi = printLampi -> lamposeg;
	}
}


void faiPercorsi (Sasso **tasca)
{
	Lampo *lampi;
	int opCliente = 0;
	int npercorso;
	int ret=0;

	lampi = ottimizzaCollegamenti (tasca);

#ifdef DEBUG
	stampaLampi (lampi);
#endif
	SGR(CLEAR,CYAN,40);

	printf("Desidera: \n\n");
	printf("cuccette ? (s/n)  ");
	if (leggiSi())
		opCliente |= F_LETTO;
	printf("vagone ristorante ? (s/n)  ");
	if (leggiSi())
		opCliente |= F_RISTORO;
	printf("servizio di trasporto invalidi in carrozzella ? (s/n)  ");
	if (leggiSi())
		opCliente |= F_HANDICAP;
	printf("vagone fumatori ? (s/n)  ");
	if (leggiSi())
		opCliente |= F_FUMO;

	stampa (lampi,opCliente);

	while (ret != 1) {
		SGR(CLEAR,CYAN,40);
		printf ("Numero del percorso scelto ?  ");
		if ((ret = scanf ("%d",&npercorso)) != 1)
			scanf("%*s");

		if (npercorso < 0 || npercorso > percorsi) {
			SGR(INTER,YELLW,40);
			printf("Non esiste il percorso numero %d, riproVi\n",npercorso);
			ret = 0;
		}
	}

	stampaBiglietto (lampi,tasca, npercorso);

	stampaOpzioni (tasca, npercorso);
}
