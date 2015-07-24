#define error(x) { printf(x); exit(5); }

/* massima lunghezza delle stringhe di caratteri */
#define MAXC	40

#define MDAY	1440
/*---------------   INPUT TRATTO  -----------*/
typedef struct {
	char x[MAXC],y[MAXC];
	int peso;
	int linea;
} InpTratto;

#define MAX_INP_TRATTI 200
extern InpTratto ingrafo [ MAX_INP_TRATTI ];

/*----------------   GRAFO ---------------*/
typedef struct {
	int x,y;
	int peso;
	int linea;
	int verso;
} Tratto;

#define MAXTRATTI (MAX_INP_TRATTI * 2)
extern Tratto grafo[MAXTRATTI];
extern int ntratti;

void stampaGrafo(void);

int grafoFirstIdx (int x);
int grafoIdx (int x, int y);


/*-------------- CITTA' ----------------*/
#define MAXNODI  MAX_INP_TRATTI
extern char *citta[MAXNODI];
extern int ncitta;

/*-------------- TRENI ----------------*/
typedef struct {
	int numero;
	int linea;
	int direzione;
	char tipo;
	int opz;
} Treno;

#define MAXTRENI  200
extern Treno treno[MAXTRENI];
extern int ntreni;

void stampaTreno (int idx);
int trenoIdx (int numero,int linea,int dir);

/*-------------TABELLA --------------*/
typedef struct {
	int linea;
	int stazioni;
	int treni[2];
	int *stazione;
	int *treno[2];	/* treno[treni][2 (andata e ritorno)] */
	int *orario[2][30];	/* orario[stazioni][2 (andata e ritorno)][treni]*/
} Tabella;

#define MAXLINEE 40
extern Tabella tabella[MAXLINEE];

extern int tabelle;

int tabIdx(int linea);

void stampaTabella (void);

/* -------- DA POLLICINO ---------------- */
typedef struct Sasso {
	int nodo;
	int peso;
	int npunt;
	struct Sasso *next;
} Sasso;

Sasso **pollicino(int x, int y);

#define MAXDIS 100

/*----- costanti per le opzioni ------ */
#define F_LETTO    1;
#define F_HANDICAP 2;
#define F_RISTORO  4;
#define F_FUMO     8;

/* -------GRAFICA ------------*/
#define CLEAR 0
#define GRASS 1
#define UNDER 4
#define INTER 5
#define REVER 7
#define HIDDN 8

#define BLACK 30
#define RED   31
#define GREEN 32
#define YELLW 33
#define BLUE  34
#define MAGEN 35
#define CYAN  36
#define WHITE 37

#define BACK  10

void ED(void);
void EL(void);
void SGR(int a, int b, int c);
void CUP(int riga,int colonna);
void HVP(int riga,int colonna);

int leggiSi(void);
int leggiOra (int *minuti);
void stampaOra (int minuti);

/*----- variabili che contengono l'input dell'utente ---- */
extern int inizioFascia, fineFascia;
extern int part, arr;
void aspetta(void);
