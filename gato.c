/*
Juego del gato o tres en raya programado en lenguaje C
Esta generalizado para tableros de MxM puede setear la macro #TAM para cambiar la dimension del tablero

Ademas tiene 3 modalidades de juego,

Maquina Usuario
Usuario Usuario
Maquina Maquina

TODO: Verificar si la jugada es valida para cualquier caso
*/

#define TAM 	25	

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ONE_PLAYER 	1
#define TWO_PLAYERS 	2
#define AUTOMATIC 	3

#define BLUE(s) "\x1b[34m" s "\x1b[0m"
#define RED(s) "\x1b[31m" s "\x1b[0m"

static int size;

//funcion que cuenta las casillas ocupadas del tablero
int count_check(int const board[TAM][TAM]){
	int count = 0;
	for(int i=0; i<TAM;i++)
		for(int j=0; j<TAM;j++)
			if(board[i][j]) count++;
return count;
}

//funcion que ejecuta una jugada
int play(int board[TAM][TAM], int move, int s){
	int i = (move-1) / TAM;
	int j = (TAM - ((i+1)*TAM - move))-1;

	//si tengo una jugada libre
	if(!board[i][j])
	{
		board[i][j] = s;
		return 0;
	}

return count_check(board);
}

//funcion que verifica si hay un ganador
int check_win(int const board[TAM][TAM], int s){
	int winrows;
	int wincols;
	int windgn1, windgn2;

	
	//diagonales	
	windgn1=1;
	windgn2=1;
	for(int i=0; i<TAM;i++){
		if(board[i][i] != s) 
			windgn1=0;
			
		if(board[i][TAM-(i+1)] != s) 
			windgn2=0;
	}
	//si hay algina giagonale ganadora
	if(windgn1 || windgn2) return 1;

	
	//filas y columnas
	for(int i=0; i<TAM;i++){
		//filas
		winrows = 1;
		wincols = 1;

		for(int j=0; j<TAM;j++){
			if(board[i][j] != s) 
				winrows = 0;
			if(board[j][i] != s) 
				wincols = 0;
			
			
			
			
		}

		if(winrows || wincols) return 1;
	}


return 0;
}

//funcion que verifica si hay una jugada ganadora en el proximo turno
int win_move(int s, int const board[TAM][TAM]){
	//copio el estado actual del tablero;
	int board2[TAM][TAM];
	for(int i=0; i<TAM;i++)
		for(int j=0; j<TAM;j++)
			board2[i][j] = board[i][j];

	for(int i=0; i<TAM;i++){
		for(int j=0; j<TAM;j++){
			int check = board[i][j];
			//casilla libre pruebo si es una combinacion ganadora
			if(!check){
				board2[i][j] = s;
				if(check_win(board2,s)) 
					return ((i*TAM)+j)+1;
				else 
					board2[i][j] = 0;
			}
		}
	}
return 0;		
}

//funcion que determina la proxima jugada para la maquina de forma aletoria
//esta funcion se hizo para poner a una maquina tonta a jugar y demostrar que el algoritmo 
//nunca pierde
int next_rnd_move(int const board[TAM][TAM], int s){
	int bussy = 1;
	int max = TAM*TAM;	
	int win;
	int sop;

	//si en la proxima jugada gano
	win = win_move(s, board);
	if(win) return win;

	//si en la proxima jugada gana mi oponente
	sop = (s==2) ? 1 : 2; 
	win = win_move(sop, board);
	if(win) return win;
	
	while(bussy){ 	
		int rnd = 1 + (rand() % (max-1));
		int i = (rnd-1) / TAM;
		int j = (TAM - ((i+1)*TAM - rnd))-1;

		int check = board[i][j];
		//printf("(%d,%d)=%d\n",i,j,check);
		if(!check) return rnd;
		if(count_check(board) == max) return 0;
	}
//no hay proxima jugada	
return 0;
}

//funcion que determina la proxima jugada para la maquina
int next_move(int const board[TAM][TAM], int s){
	int win;
	int sop;
	int check;
	int center;
	int count = 0;	
	
	//si en la proxima jugada gano
	win = win_move(s, board);
	if(win) return win;

	//si en la proxima jugada gana mi oponente
	sop = (s==2) ? 1 : 2; 
	win = win_move(sop, board);
	if(win) return win;


	//si el centro esta libre
	center = (TAM%2==0) ? 0 : ((TAM*TAM)/2)+1;

	//si es un numero par, el tablero no tiene un centro
	if(center)
	{
		int i = (center-1) / TAM;
		int j = (TAM - ((i+1)*TAM - center))-1;
	
		check = board[i][j];
		if(!check) return center; 
	}

	//si alguna esquina esta libre
	check = board[0][0];
	if(!check) return 1;
	
	check = board[0][TAM-1];
	if(!check) return TAM;
	
	check = board[TAM-1][0];
	if(!check) return (TAM*TAM)-(TAM -1);
	
	check = board[TAM-1][TAM-1];
	if(!check) return TAM*TAM;

	//si algun lateral desocupado
	for(int i=1; i<TAM-1;i++){
		check = board[0][i];
		if(!check) return i+1;
		
		check = board[i][0];
		if(!check) return (TAM*i)+1;
		
		check = board[TAM-1][i];
		if(!check) return ((TAM * (TAM-1) ) +i) +1;
		
		check = board[i][TAM-1];
		if(!check) return (TAM*i) + TAM;
	}
		
	//si algun otro elemtno esta desocupado lo devuelve
	for(int i=1; i<TAM-1;i++){
		for(int j=1; j<TAM-1;j++){
			check = board[i][j];
			if(!check) return ((TAM*i)+j)+1;
		}
	}
	
//no hay proxima jugada	
return 0;
}
//funcion que inicializa el tablero
void init_board(int board[TAM][TAM]){
	for(int i=0; i<TAM;i++)
		for(int j=0; j<TAM;j++)
			board[i][j] = 0;
		
}

//funcion que dibuja el tablero por pantalla
void display_board(int const board[TAM][TAM], char const *symbols){
	printf("\n");
	int count = 1;
	for(int i=0; i<TAM;i++){
		for(int j=0; j<TAM;j++){
			int check = board[i][j];
			if(!check)
				printf("%2d",count);
			else
			{
				if(check == 1)	
					printf(RED("%2c"),symbols[check]);
				else
					printf(BLUE("%2c"),symbols[check]);
			}
			if(j<TAM-1) printf("|");
			count++;
		}
	printf("\n");
	if(i < TAM-1)
		for(int i=0;i<TAM;i++)
			printf("---");
	printf("\n");
	}
}

//menu para seleccionar modo de juego
int select_mode(){
	int mode = 0;
	while(mode==0){
		printf("Seleccione Modalidad de Juego:\n");
		printf("1) Un Jugador\n");
		printf("2) Dos Jugadores\n");
		printf("3) Automatico\n");

		scanf("%d",&mode);
		if(mode == 1 || mode == 2 || mode == 3){
			return mode;
		}
	mode = 0;
	system("clear");
	}
}

//menu para seleccionar el symbolo del jugador, solo valido para uno y dos players
int select_symbol(){
	int symbol = 0;
	while(symbol==0){
		printf("Seleccione Simbolo:\n");
		printf("1) o\n");
		printf("2) x\n");

		scanf("%d",&symbol);
		if(symbol == 1 || symbol == 2 ){
			return symbol;
		}
	symbol = 0;
	system("clear");
	}
}

//menu que pide la proxima jugada, solo valido para el jugador humano
int select_move(){
	int move = 0;
	while(move==0){
		printf("Ingrese nro. de jugada:\n");
		scanf("%d",&move);

		if(move >=1 && move <= TAM*TAM)
			return move;

	move = 0;
	system("clear");
	}
}

//funcion principal
int main(){
	int turn = 1;
	int move;
	int symbol;
	int mode;
	int playing = 1;
	int check;
	int count = 0;
	
	int board[TAM][TAM];
	char symbols[3] = {' ','o','x'};

	init_board(board);
	
	mode = select_mode();
	symbol = (mode == AUTOMATIC) ? 1 : select_symbol();
	system("clear");
	printf("Tu simbolo es %c\nQue comience el juego\n",symbols[symbol]);
	display_board(board, symbols);
	while(playing){
		int change_move = 1;
		while(change_move){
			if(mode == ONE_PLAYER)
				move = (turn) ? select_move() :  next_move(board, symbol);
			else if(mode == TWO_PLAYERS)
				move = select_move();
			else
			{
			 //AUTOMATIC
				move = (turn) ? next_move(board, symbol) : next_rnd_move(board, symbol);
			}
			if(!move) break;
			
			change_move = play(board, move, symbol);
			if(change_move && change_move==TAM*TAM) 
				break;
		}	
		check = check_win(board,symbol); 
		count = count_check(board);	
		if(check || count==TAM*TAM)	
			playing = 0;
		
		sleep(1);
		system("clear");
		
		display_board(board, symbols);
		
		//cambio de turno y cambio de simbolo
		symbol = (symbol==1) ? 2 : 1;
		turn = !turn;
	}
return 0;
}
