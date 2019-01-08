#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4]; // b[4][4] 보드 상태 저장 전역변수
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
} //시작:게임오버하지 않고 모든 칸을 0으로 만든다.

void rotate(int b[4][4]){ // 시계방향으로 90도 회전 set board()함수에서 사용
	/* user code */ 
	int temp[4][4],i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			temp[i][j]=b[i][j];
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			b[i][j]=temp[3-j][i];
}

int set_board(int dir, int b[4][4]){ // 입력된 키대로 보드 이동시키기 올바르면 1 return
	/* user code init */
	int cut,l;
	int i,j,count=0;
	dir=(dir+3)%4;
	for(i=0;i<dir;i++)
		rotate(b);
	for(i=0;i<4;i++)
	{
		cut=-1;
		for(j=1;j<4;j++)
		{
			if(b[j][i] == 0)
				continue;
			else if(j-1 == cut)
			{
				continue;
			}
			else if(b[j-1][i] == 0)
			{                                               
				l=2;
				while(1)
				{
					if(j-l == cut)
					{
						b[j-l+1][i] = b[j][i];
						b[j][i]=0;
						break;
					}
					else if(b[j-l][i] == b[j][i])
					{
						b[j-l][i] = 2*b[j][i];
						b[j][i] = 0;
						cut=j-l;
						break;
					}
					else if(b[j-l][i] != 0)
					{
						b[j-l+1][i] = b[j][i];
						b[j][i] = 0;
						break;
					}
					l++;
				}
				count=1;
			}
			else if(b[j-1][i] == b[j][i])
			{	
				b[j-1][i] = 2*b[j][i];
				b[j][i]=0;
				cut= j-1;
				count=1;
			}
		}
	}
	for(i=0;i<4-dir;i++)
		rotate(b);
	if(count == 0)
		return 0; 
	return 1;
}

int is_game_over(){ //2 또는 4를 생성할 수 있는 지 없는 지 고려하지 않음
	/* user code */
	int i,j,k;
	int temp[4][4];

	for(k=0;k<4;k++)
	{
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				temp[i][j]=b[i][j];

		if(set_board(k,temp))
			return 1;
	}
	/* if game over return 0, else then return 1 */
	return 0;
}

void draw_board(int tot, int command){ // 호출될 때마다 4*4 새롭게 그리고 전체 생성된 tot 개수가 판에 나타난 숫자의 합과 일치하는 지 확인, 판이 변화될 때마다 output.txt에 점수,키,판 정보 출력
	int i, j, k, c[8][8], score;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */	
	/* calculate score & check sum of all block equasls variable tot */
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(score<b[i][j])
				score=b[i][j];
			tot-=b[i][j];
		}
	if(tot != 0)
		printf("Error!\n");

	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
		
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
}

int make_two_or_four(){ //빈공간에 2 또는 4 생성시키기, 빈공간 없으면 0 return, 생성한 2또는 4 return

	/* user code */
	int i=0,j=0,exist=0;
	int randomnum;
	/* return 2 or 4 that makes in this times */
	randomnum=(((rand()%3)%2)+1)*2;
	do{
		j=0;
		do{
			if(b[i][j]==0)
			{
				exist=1;
			}
			j++;
		}while(j<4 && exist == 0);
		i++;
	}while(i<4 && exist == 0);
	/* if can not make two or four, then return 0 */
	if(exist == 1)
	{
		do{
			i=rand()%4;
			j=rand()%4;
		}while(b[i][j] != 0);
		b[i][j] = randomnum;
		return randomnum;
	}
	return 0;	
}


int getch(void) // 입력받은 키를 반환
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() { // getch함수 호출에 얻은 키 따라 값 반환. d는 0으로 w는 1로, a는 2,s는 3 반환
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;

	/* make init board */
	/* user code */
	tot+=make_two_or_four();
	tot+=make_two_or_four();

	draw_board(tot, command);

	
	do {
		command = GetCommand();

		if (command != -1) {
			if( set_board(command, b) ){

				/* user code */
				tot+=make_two_or_four();
				game_over= (is_game_over()+1)%2;

				draw_board(tot, command);
			}
		}

	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}
