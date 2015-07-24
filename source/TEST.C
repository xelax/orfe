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

void ED(void)
{
	printf("\x1B[2J");
}
void EL(void)
{
	printf("\x1B[K");
}

void SGR(int a, int b, int c)
{
	printf("\x1B[%d;%d;%dm",a,b,c);
}

void CUP(int riga,int colonna)
{
	printf("\x1B[%d;%dH",riga,colonna);
}

void HVP(int riga,int colonna)
{
	printf("\x1B[%d;%df",riga,colonna);
}

main ()
{
	ED();
	SGR(CLEAR,WHITE,BLACK+BACK);
	printf("PROVA");
	SGR(UNDER,RED,BLACK+BACK);
	printf("PROVA");
	SGR(4,32,40);
	printf("PROVA");
	SGR(5,33,40);
	printf("PROVA");
	SGR(7,34,40);
	printf("PROVA");
	CUP(10,10);
	printf("1,10");
	CUP(10,20);
	printf("10,1");
	HVP(15,60);
	EL();
	printf("PROVA");
	SGR(0,0,0);
}
