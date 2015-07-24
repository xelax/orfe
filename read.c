/*  read.c
Created 3-9-1991
ch 4-9
ch 5-9
ch 7-9
ch 8-9
ch 13-9
ch 14-9
ch 19-9
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "globals.h"

void sistemaNome (char *str)
{
	int i = 0;
	
	str[0] = toupper (str[0]);

	for (i=1; str[i] != '\0'; i++)
		str[i] = tolower(str[i]);
		
}

int leggiSi(void)
{
	char str[MAXC];
	scanf("%s",str);
	(void) getchar();
	if (strchr(str,'s') != NULL || strchr (str,'S') != NULL)
		return 1;
	else
		return 0;
}
/*---------------  LINEA  ---------------*/
/* strutture locali condivise da fileToGrafo e grafoToTabella */
static int nlinee;
static int linea[MAXLINEE];

/*----------------   GRAFO ---------------*/
void stampaGrafo (void)
{
	int i;

	for (i=0; i<ntratti;i++) 
		printf("Da %d\tA %d\tpeso %d\tlinea %d\tverso %d\n",
		grafo[i].x,
		grafo[i].y,
		grafo[i].peso,
		grafo[i].linea,
		grafo[i].verso );

}

/*
* cerca il primo tratto che inizia per X
*/
int grafoFirstIdx (int x)
{
	int i;

	for (i=0; i < ntratti; i++) 
		if (grafo[i].x == x)
			return i;
	printf("grafoFirstIdx: non ho trovato  %d,\n",x);
	return -1;
}

static int trattoCmp(Tratto *a,Tratto *b)
{
	if (a->x < b->x)
		return -1;
	else if (a->x == b->x)
		return (a->y < b->y  ? -1 : (a->y == b->y ? 0 : +1 ));
	else
		return +1;
}
int grafoIdx (int x, int y)
{
	Tratto key;
	Tratto *t;

	key.x = x;
	key.y = y;
	t = (Tratto *) bsearch (&key,grafo,ntratti,sizeof(Tratto), (int (*)(const void*, const void*))trattoCmp);
	if (t == NULL) {
#ifdef DEBUG
		printf("grafoIdx: non ho trovato la coppia %d,%d\n",x,y);
#endif
		return -1;
	}
	else
		return (int) abs(t-grafo);
}


/*-------------- TRENI ----------------*/
void stampaTreno (int idx)
{
	printf("numero = %d\t",treno[idx].numero);
	printf("linea = %d\t",treno[idx].linea);
	printf("direzione = %d\t",treno[idx].direzione);
	printf("tipo = %c\t",treno[idx].tipo);
	printf("opz = %d\n",treno[idx].opz);
}

static int trenoCmp(Treno *x,Treno *y)
{
	if (x->linea < y-> linea )
		return -1;
	else if (x->linea == y->linea)
		if (x->direzione < y->direzione)
			return -1;
		else if (x->direzione == y->direzione)
			return (x->numero < y->numero ? -1 : (x->numero == y->numero ? 0 : 1)); 
		else
			return +1;
	else
		return +1;

}

int trenoIdx (int numero,int linea, int dir)
{
	Treno key = {0,0,0,0,0};
	Treno *ptr;
	
	key.numero = numero;
	key.linea = linea;
	key.direzione = dir;
	ptr = (Treno *) bsearch (&key,treno,ntreni,sizeof(Treno),(int (*)(const void*, const void*)) trenoCmp);
	if (ptr == NULL)
		return -1;
	else
		return abs(ptr - treno);
}

/*-------------TABELLA --------------*/
void stampaOra (int minuti)
{
	if (minuti % 60 < 10)
		printf("%2d:0%1d",(int) (minuti/60) % 24, minuti % 60);
	else
		printf("%2d:%2d",(int) (minuti/60) % 24, minuti % 60);
}

int leggiOra (int *minuti)
{
	int h,m,ret;

	ret = scanf("%d.%d",&h,&m);

	if (ret != 2) {
		scanf ("%*s");
		return 0;
	}

	if ( h <= 24 && h >=0 && m<60 && m>= 0) {
		*minuti = (h*60+m) % MDAY;
		return 1;
	}
	else
		return 0;;
}

static int tabCmp(Tabella *x,Tabella *y)
{
	return (x->linea < y->linea ? -1 : (x->linea == y->linea ? 0 : 1));
}

int tabIdx(int linea)
{
	Tabella key, *ptr;


	key.linea = linea;
	ptr = (Tabella *) bsearch (&key,tabella,tabelle,sizeof(Tabella),(int (*)(const void*, const void*)) tabCmp);
	if (ptr == NULL)
		return -1;
	else
		return abs(ptr - tabella);
}

void stampaTabella (void)
{
	int i,j,k,s;

	for (i=0; i < nlinee; i++) {
		printf("------linea----- %d\n",tabella[i].linea);
		printf("stazioni   %d\n",tabella[i].stazioni);
		for (j=0; j<tabella[i].stazioni; j++)
			printf("stazione numero %d-> %d\n",j,tabella[i].stazione[j]);

		printf("treni Andata %d  Ritorno %d\n",tabella[i].treni[0],tabella[i].treni[1]);

		for (k=0; k<2; k++) 
			for (j=0; j<tabella[i].treni[k]; j++)
				printf("treno numero %d %d  -> %d\n",k,j,tabella[i].treno[k][j]);

		for (k=0; k<2; k++)
			for (j=0; j<tabella[i].treni[k]; j++)
				for (s=0; s<tabella[i].stazioni; s++) {
					printf("orario  %d %d %d -> ",k,j,s);
					stampaOra (tabella[i].orario[k][j][s]);
					printf("\n");
				}
	}
}

static oraCmp (int **x, int **y)
{
	return (*x[0] < *y[0] ? -1 : (*x[0] == *y[0] ? 0 : 1));
}

static oraCmpInv (int **x, int **y)
{
	return (-oraCmp(x,y));
}

/* ------------------- */

/*
*	Dato un puntatore alla stringa della citta' ritorna il numero
*/
static int pstrcmp(char ** x,char **y)   { return strcmp(*x,*y); }

int strtonum (char *str)
{
	char *s;
	char **t;

	s = str;
	t = (char **) bsearch (&s,citta,ncitta,sizeof(char **),(int (*)(const void*, const void*))pstrcmp);
	if (t == NULL) {
#ifdef DEBUG
		printf("\nstrtonum: Non ho trovato %s\n",str);
#endif
		return -1;
	}
	else 
		return (int) abs(t-citta);
}

/*
* confronta due interi, per fileToGrafo
*/
static int intcmp (int *x, int *y)
{
	return (*x < *y ? -1 : ( *x == *y ?  0 : +1));
}

/*
* questa funzione legge il file e riempie le strutture globali:
* grafo [ ntratti ]	ordinata
* citta [ ncitta ]	ordinata
* linea [ nlinee ]	ordinata
*/
void fileToGrafo (char *nomefile)
{
	FILE *fp;
	int i,j,k,l,peso,nlinea,nintratti,ret;
	char *pcitta,x[MAXC],y[MAXC];
	char *cx,*cy;

	/*----------------------------------*/
	/* Lettura file, partenza           */
	/*----------------------------------*/
	if ((fp = fopen(nomefile,"r")) == NULL) {
		error ("errore apertura file");
	}
	i = 0;
	while ( i < MAX_INP_TRATTI   &&
	        (ret=fscanf (fp, "%s %s %d %d",x,y,&peso,&nlinea)) != EOF )
	{
		if (ret != 4) {
			printf("Errore nel file dati %s\n",nomefile);
			return;
		}
		else {
			sistemaNome (x);
			sistemaNome (y);			
			strcpy (ingrafo[i].x , x);
			strcpy (ingrafo[i].y , y);
			ingrafo[i].peso = peso;
			ingrafo[i].linea = nlinea;
			++i;
		}
	}
	fclose (fp);

	/*-------------------------------------*/
	/* settaggio variabile globale ntratti */
	/*-------------------------------------*/
	nintratti = i;
	ntratti = nintratti * 2;

	/*----------------------------------*/
	/*     costruzione linea[ nlinee]   */
	/*----------------------------------*/
	/* cerca i numeri delle linee */
	l = 0;
	for (i=0; i < nintratti; i++) {
		nlinea = ingrafo[i].linea;

		if (l==0) {
			linea[l] = nlinea;
			l++;
		}
		else {
			for (j=0; j<l; j++) {
				if(linea[j] == nlinea)
					break;
			}
			if (j==l) { 
				linea[l] = nlinea;
				l++;
			}
		}
	}
	nlinee = tabelle = l;
	qsort ((void *)linea,nlinee,sizeof(int),(int (*)(const void*, const void*)) intcmp);

	/* copia i numeri di linea in tabella[] */
	for (i=0; i<tabelle; i++)
		tabella[i].linea = linea[i];

#ifdef DEBUG
	printf("\n nlinee %d\n",nlinee);
	for (i=0; i<l; i++)
		printf("linea %d\n",linea[i]);
#endif

	/*----------------------------------*/
	/*  costruzione di citta[ ncitta ]  */
	/*----------------------------------*/
	/* cerca citta' sulla X  */
	k = 0;
	for (i=0; i < nintratti; ++i) {
		pcitta = ingrafo[i].x;

		if (k==0) {
			citta[0]=pcitta;
			k++;
		}
		else {
			for (j=0; j<k; j++) {
				if(strcmp (citta[j],pcitta) == 0)
					break;
			}
			if (j==k) { 
				citta[j]=pcitta;
				k++;
			}
		}
	}
	/* ora continua la ricerca sulla Y */
	for (i=0; i < nintratti; ++i) {
		pcitta = ingrafo[i].y;

		if (k==0) {
			citta[0]=pcitta;
			k++;
		}
		else {
			for (j=0; j<k; j++) {
				if(strcmp (citta[j],pcitta) == 0)
					break;
			}
			if (j==k) { 
				citta[j]=pcitta;
				k++;
			}
		}
	}

	ncitta = k;
	/* ordina le citta */
	qsort ((void *) citta,ncitta,sizeof(char *),(int (*)(const void*, const void*)) pstrcmp);

#ifdef DEBUG
	printf ("\nncitta = %d\n",ncitta);
	for (i=0; i<ncitta;i++) 
		printf("Citta:   %s\n",citta[i]);
	printf("\n\n");
#endif

	/*----------------------------------*/
	/* costruzione di grafo [ ntratti ] */
	/*----------------------------------*/
	for (i=0; i < nintratti; i++) {
		/* in ordine corretto */
		cx=ingrafo[i].x;
		cy=ingrafo[i].y;
		grafo[i*2].x       = strtonum(cx);
		grafo[i*2].y       = strtonum(cy);
		grafo[i*2].peso    = ingrafo[i].peso;
		grafo[i*2].linea   = ingrafo[i].linea;
		grafo[i*2].verso   = 0;
		/* e in ordine inverso */
		grafo[i*2+1].y     = strtonum(cx);
		grafo[i*2+1].x     = strtonum(cy);
		grafo[i*2+1].peso  = ingrafo[i].peso;
		grafo[i*2+1].linea = ingrafo[i].linea;
		grafo[i*2+1].verso = 1;
	}

	/* ordina grafo */
	qsort ((void *)grafo, ntratti, sizeof (Tratto),(int (*)(const void*, const void*)) trattoCmp);
}

/*
* riempie alcuni campi di tabella e ne alloca la memoria necessaria
* tabella
*	int linea       <- numero della linea 
*	int stazioni	<- numero di stazioni sulla linea
*	int treni		<- vuoto
*	int (*stazione) []  	<- puntatore all'array allocato
*	int (*treno) [][]		<- NULL
*	Ora (*orario) [][][]	<- NULL
*/ 
void grafoToStazioni ()
{
	int  staz[MAXLINEE][MAXNODI];
	int i,j,k,ncapo;
	void *ptr;

	/* costruzione di staz [ nlinee ] [ ...MaxNodi] */  	
	for (i=0; i < nlinee; i++) {
		/* cerchiamo il capolinea, indietro ...  */
		for (j=0; j < ntratti; j++)
			if (grafo[j].linea == linea[i])
				break;
		ncapo = grafo[j].y;
		k = grafoFirstIdx(ncapo);
		while (grafo[k].x == ncapo) {
			if (grafo[k].linea == linea[i]  && grafo[k].verso == 1) {
				if (grafo[k].y == ncapo) {
					printf("nodo anomalo %d\n",ncapo);
					break;
				}
				else {
					ncapo = grafo[k].y;
					k = grafoFirstIdx(ncapo);
				}
			}
			else
				k++;
		}

#ifdef DEBUG		
		printf("Linea %d, capolinea %d\n",linea[i],ncapo);
#endif
		
		/* ncapo ha raggiunto il capolinea, ora avanti ! */
		j = 0;
		staz[i][0] = ncapo;
		k = grafoFirstIdx(ncapo);
		while (grafo[k].x == staz[i][j]) {
			if (grafo[k].linea == linea[i] && grafo[k].verso == 0) {
				if (grafo[k].y == staz[i][j]) {
					printf("nodo anomalo %d\n",staz[i][j]);
					break;
				}
				else {
					++j;
					staz[i][j] = grafo[k].y;
					k = grafoFirstIdx(staz[i][j]);
				}
			}
			else
				k++;
		}

#ifdef DEBUG
		for (k=0; k <= j; k++)
			printf("linea %d\tstazione%d\n",linea[i],staz[i][k]);
#endif

		tabella[i].linea = linea[i];
		tabella[i].stazioni = j+1;

		/* alloca la memoria per l'array delle stazioni e lo copia */
		if ((ptr = calloc(sizeof(int) , j+1) ) == NULL)
			error("grafoToTab: calloc failed\n");
		memcpy (ptr,staz[i],sizeof(int) * (j+1)); 

		tabella[i].stazione = (int *) ptr;

	}

}

void fileToTreni(char *nomefile)
{
	FILE *fp;
	int i,ret;
	Treno tmp = { 0,0,0,0,0 };
	char opz[MAXC];

	/*----------------------------------*/
	/* Lettura file, partenza           */
	/*----------------------------------*/
	if ((fp = fopen(nomefile,"r")) == NULL) {
		error ("errore apertura file\n");
	}

	for (i=0; i < MAXTRENI   && (ret=fscanf (fp, "%d %d %d %1s %s",&tmp.numero, &tmp.linea, &tmp.direzione, &tmp.tipo, opz)) != EOF;
	     ++i) 
	{
		if (ret == 5) {
			tmp.opz = 0;
			if (strchr(opz,'L') != NULL)
				tmp.opz |= F_LETTO;
			if (strchr(opz,'H') != NULL)
				tmp.opz |= F_HANDICAP;
			if (strchr(opz,'R') != NULL)
				tmp.opz |= F_RISTORO;
			if (strchr(opz,'F') != NULL)
				tmp.opz |= F_FUMO;
			treno[i] = tmp;
		}
		else {
			printf("fileToTreni: errore nel file dati %s, ret=%d\n",nomefile,ret);
			stampaTreno(i);
			return;
		}

	}
	fclose (fp);
	ntreni = i;
	qsort ((void *)treno, ntreni, sizeof (Treno),(int (*)(const void*, const void*)) trenoCmp);

#ifdef DEBUG
	printf("numero treni = %d\n",ntreni);
	for (i=0; i<ntreni; i++)
		stampaTreno(i);
#endif
}


void faiOrario (int linea, int dir)
{
	int i;
	char *strcapo,*strcoda;
	int num,tpart,tarr,kmt;
	float media;
	int thisTreno;
	int itreno,treni,itab;
	int *ptr;
	int nstaz,*staz;
	int *tempo[MAXTRENI];
	int tempTreno[MAXTRENI];


	itab = tabIdx(linea);
	nstaz = tabella[itab].stazioni;
	staz  = tabella[itab].stazione;

	if (itab == -1) {
		printf("non esiste la linea %d\n",linea);
		return;
	}

	/* metti i treni gia' immessi in tempTreno */
	if ( (treni = tabella[itab].treni[dir]) != 0 && tabella[itab].treno[dir] != NULL) {
		memcpy(tempTreno, tabella[itab].treno[dir],treni * sizeof(int));
		free (tabella[itab].treno[dir]);
		for (i=0; i<treni; i++) {
			tempo[i] = tabella[itab].orario[dir][i];
		}

	}

	thisTreno = 0;

	while (treni <MAXTRENI) {
		num = 0;
		printf ("linea %d, direzione  %d\n",linea, dir);
		printf ("numero treno ?\t");
		if (scanf  ("%d",&num) != 1) {
			scanf("%*s");
			break;
		}
		if ( (itreno = trenoIdx(num,linea,dir)) == -1) {
			printf ("Errore: il treno numero %d non esiste in tabella\n\n",num);
			continue;
		}
		if (treno[itreno].linea != linea || treno[itreno].direzione != dir) {
			printf ("il treno %d non effettua servizio su questa linea in questa direzione\n\n",treno[itreno].numero);
			continue;
		}

		for (thisTreno=0; thisTreno < treni; thisTreno++)
			if (tempTreno[thisTreno] == num)
				break;

		if (thisTreno != treni) {
			printf ("Hai gia' inserito un treno con questo numero, vuoi cambiarne gli orari ?  ");
			if ( ! leggiSi())
				continue;
		}

		if (dir == 0) {
			strcapo = citta[ staz [0] ];
			strcoda = citta[ staz [nstaz-1] ];
		}
		else {
			strcoda = citta[ staz [0] ];
			strcapo = citta[ staz [nstaz-1] ];
		}

		printf ("Il capolinea di partenza e'\t%s\n",strcapo);
		printf ("Il treno %d parte alle\t (h.m) = ",num);
		while (leggiOra(&tpart) != 1) {
			printf("\nErrore, orario non valido !, riprova -> ");
		}

		printf ("Il capolinea di arrivo e'\t%s\n",strcoda);
		printf ("Il treno %d arriva alle\t (h.m) = ",num);

		while (leggiOra(&tarr) != 1) {
			printf("\nErrore, orario non valido !, riprova -> ");
		}

		/* se il treno arriva il giorno dopo, aggiungo 1440 minuti = 24 ore */
		if (tpart >= tarr)
			tarr += 1440;

		kmt = 0;
		for (i=0; i< nstaz-1; i++)
			kmt += grafo[ grafoIdx(staz[i],staz[i+1]) ].peso;

		media = (float) (tarr-tpart) / (float)kmt;

		printf("\nkm = %d, ore %d, minuti = %d, media %f  km/ora\n\n",kmt, (tarr-tpart)/60, (tarr-tpart)%60,(1.0/media) * 60.0);


		/* se e' un nuovo treno */
		if (thisTreno == treni) {
			++treni;
			tempTreno [thisTreno] = num;

			/* allocazione dell'array di orari per questo treno */
			tempo[thisTreno] = calloc (sizeof(int), nstaz);
			if (tempo[thisTreno] == NULL) {
				printf("faiOrario: tempo calloc failed thisTreno=%d\n",thisTreno);
				return;
			}
			tabella[itab].orario[dir][thisTreno] = tempo[thisTreno];
		}

		if (dir == 0) {
			tempo [thisTreno][0] = tpart;
			for (i=0; i<nstaz-1; i++) {
				tempo [thisTreno][i+1] = (int) (tempo[thisTreno][i] +  grafo[ grafoIdx(staz[i],staz[i+1]) ].peso * media) % 1440;
				printf("%-20s alle ",citta[staz[i]]);
				stampaOra (tempo[thisTreno][i]);
				printf("\n");
			} 
			tempo [thisTreno][nstaz-1] = tarr % 1440;
			printf("%-20s alle ",citta[staz[nstaz-1]]);
			stampaOra (tempo[thisTreno][nstaz-1]);
			printf("\n");

		}
		else {

			tempo [thisTreno][nstaz-1] = tpart;

			printf("%-20s alle ",citta[staz[nstaz-1]]);
			stampaOra (tempo[thisTreno][nstaz-1]);
			printf("\n");

			for (i=nstaz-2;  i >= 0 ; i--) {
				tempo [thisTreno][i] = (int) (tempo[thisTreno][i+1] +  grafo[ grafoIdx(staz[i+1],staz[i]) ].peso * media) % 1440;
				printf("%-20s alle ",citta[staz[i]]);
				stampaOra (tempo[thisTreno][i]);
				printf("\n");
			}
			tempo [thisTreno][0] = tarr % 1440;
 		}
	}


	tabella[itab].treni[dir] = treni;
	
	if (treni == 0)
		return;

	if ((ptr  = calloc (sizeof(int), treni)) == NULL) {
		printf("faiOrario: treno calloc failed\n");
		return;
	}

	memcpy(ptr, tempTreno, treni * sizeof(int));

	tabella[itab].treno[dir] = ptr;
	
}

void tabToFile (char *nomefile)
{
	FILE *fp;
	int i,j,k;
	int indx;

	if ((fp = fopen(nomefile,"w")) == NULL) {
		printf ("tabToFile: errore apertura file\n");
		return;
	}

	fprintf(fp,"%d\n",tabelle);

	for (indx=0; indx < tabelle; indx++) {
		fprintf(fp,"%d\n%d  %d %d\n\n",tabella[indx].linea,tabella[indx].stazioni,tabella[indx].treni[0],tabella[indx].treni[1]);

		for (i=0; i < tabella[indx].stazioni; i++)
			fprintf(fp,"%d ",tabella[indx].stazione[i]);
	
		fprintf(fp,"\n\n");

		for (i=0; i < 2; i++) {
			for (j=0; j < tabella[indx].treni[i]; j++)
				fprintf (fp, "%d ",tabella[indx].treno[i][j]);
			fprintf(fp,"\n");
		}

		fprintf(fp,"\n");

/*		qsort ((void *)tabella[indx].orario[0], tabella[indx].treni[0], sizeof (int *),(int (*)(const void*, const void*)) oraCmp); */
/*		qsort ((void *)tabella[indx].orario[1], tabella[indx].treni[1], sizeof (int *),(int (*)(const void*, const void*)) oraCmpInv); */

		for (i=0; i < 2; i++) {
			for (j=0; j < tabella[indx].treni[i]; j++) {
				for (k=0; k <tabella[indx].stazioni; k++) 
					fprintf (fp, "%d ",tabella[indx].orario[i][j][k]);
				fprintf(fp,"\n");
			}
			fprintf(fp,"\n");
		}
	}	
	fclose (fp);
}


void fileToTab (char *nomefile)
{
	FILE *fp;
	int i,j,k;
	void *ptr;
	int indx;

	if ((fp = fopen(nomefile,"r")) == NULL) {
		printf ("fileToTab: errore apertura file\n");
		return;
	}

	fscanf(fp,"%d",&tabelle);
	if (tabelle != nlinee) {
		printf("fileToTab: nlinee non uguale a tabelle\n");
		printf("il file non e' aggiornato, cancellalo ! \n");
		return;
	}

	for (indx = 0; indx < tabelle; indx++) {
		fscanf(fp,"%d%d%d%d",&tabella[indx].linea,&tabella[indx].stazioni,&tabella[indx].treni[0],&tabella[indx].treni[1]);

		/* alloca la memoria per l'array delle stazioni e lo copia */
		if ((ptr = calloc(sizeof(int) , tabella[indx].stazioni )) == NULL) {
			printf("fileToTab: stazioni calloc failed\n");
			return;
		}
		tabella[indx].stazione = (int *) ptr;

		for (i=0; i < tabella[indx].stazioni; i++)
			fscanf(fp,"%d",&tabella[indx].stazione[i]);

		for (i=0; i < 2; i++) {
			if (tabella[indx].treni[i] != 0) {
				if ((ptr  = calloc (sizeof(int), tabella[indx].treni[i])) == NULL) {
					printf("fileToTab: treno calloc failed i = %d\n",i);
					return;
				}
				tabella[indx].treno[i] = (int *) ptr;

				for (j=0; j < tabella[indx].treni[i]; j++)
					fscanf (fp, "%d",&tabella[indx].treno[i][j]);
			}
			else
				tabella[indx].treno[i] = NULL;
		}

		for (i=0; i < 2; i++) {
			for (j=0; j < tabella[indx].treni[i]; j++) {
				/* allocazione dell'array di orari per questo treno */
				ptr = calloc (sizeof(int), tabella[indx].stazioni);
				if (ptr == NULL) {
					printf("fileToTab: orario calloc failed i=%d j=%d\n",i,j);
					return;
				}

				tabella[indx].orario[i][j] = (int *)ptr;

				for (k=0; k <tabella[indx].stazioni; k++)
					fscanf (fp, "%d",&tabella[indx].orario[i][j][k]);
			}
		}
/*		qsort ((void *)tabella[indx].orario[0], tabella[indx].treni[0], sizeof (int *),(int (*)(const void*, const void*)) oraCmp); */
/*		qsort ((void *)tabella[indx].orario[1], tabella[indx].treni[1], sizeof (int *),(int (*)(const void*, const void*)) oraCmpInv); */
	}

	fclose (fp);
}
