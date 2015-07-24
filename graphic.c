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
/*	printf("\x1B[2J"); */
	printf("\f");

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
