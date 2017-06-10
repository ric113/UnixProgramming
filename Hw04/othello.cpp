#include <map>
#include <vector>
#include <iostream>

#include "othello.h"

#define	PLAYER1SYM	('O')
#define	PLAYER2SYM	('X')
#define DIR_NUM 8

using namespace std;

int board[BOARDSZ][BOARDSZ];
int mySign;
vector<int> legalPoints;

static int const box_top = 1;
static int const box_left = 2;
static int const boxwidth = 3;

static int use_color = 0;
static int colorborder;
static int colorplayer1;
static int colorplayer2;
static int colorcursor;
static int colormsgwarn;
static int colormsgok;

const int Directions[DIR_NUM][2] = {
	{-1,0},
	{-1,1},
	{0,1},
	{1,1},
	{1,0},
	{1,-1},
	{0,-1},
	{-1,-1},
};

void
init_board() {
	bzero(board, sizeof(board));
	board[3][3] = board[4][4] = PLAYER1;
	board[3][4] = board[4][3] = PLAYER2;
}

void
init_colors() {
	int coloridx = 0;	// color idx 0 is default color
	if(has_colors() == FALSE)
		return;
	start_color();
	//
	colorborder = ++coloridx;
	init_pair(colorborder, COLOR_WHITE, COLOR_BLACK);

	colorplayer1 = ++coloridx;
	init_pair(colorplayer1, COLOR_BLACK, COLOR_GREEN);

	colorplayer2 = ++coloridx;
	init_pair(colorplayer2, COLOR_BLACK, COLOR_MAGENTA);

	colorcursor = ++coloridx;
	init_pair(colorcursor, COLOR_YELLOW, COLOR_BLACK);

	colormsgwarn = ++coloridx;
	init_pair(colormsgwarn, COLOR_RED, COLOR_BLACK);

	colormsgok = ++coloridx;
	init_pair(colormsgok, COLOR_GREEN, COLOR_BLACK);
	//
	use_color = 1;
	return;
}

static chtype
BCH(int x, int y) {
	if(board[y][x] == PLAYER1) return PLAYER1SYM|COLOR_PAIR(colorplayer1);
	if(board[y][x] == PLAYER2) return PLAYER2SYM|COLOR_PAIR(colorplayer2);
	return ' ';
}

static void
draw_box(int x, int y, int ch, int color, int highlight) {
	int i;
	attron(highlight ? A_BOLD : A_NORMAL);
	attron(COLOR_PAIR(color));
	//
	move(box_top + y*2 + 0, box_left + x*(boxwidth+1));
	if(y == 0) addch(x == 0 ? ACS_ULCORNER : ACS_TTEE);
	else       addch(x == 0 ? ACS_LTEE : ACS_PLUS);
	for(i = 0; i < boxwidth; i++) addch(ACS_HLINE);
	if(y == 0) addch(x+1 == BOARDSZ ? ACS_URCORNER : ACS_TTEE);
	else       addch(x+1 == BOARDSZ ? ACS_RTEE : ACS_PLUS);
	//
	move(box_top + y*2 + 1, box_left + x*(boxwidth+1));
	addch(ACS_VLINE);
	for(i = 0; i < boxwidth/2; i++) addch(' ');
	addch(ch);
	for(i = 0; i < boxwidth/2; i++) addch(' ');
	addch(ACS_VLINE);
	//
	move(box_top + y*2 + 2, box_left + x*(boxwidth+1));
	if(y+1 == BOARDSZ) addch(x == 0 ? ACS_LLCORNER : ACS_BTEE);
	else               addch(x == 0 ? ACS_LTEE : ACS_PLUS);
	for(i = 0; i < boxwidth; i++) addch(ACS_HLINE);
	if(y+1 == BOARDSZ) addch(x+1 == BOARDSZ ? ACS_LRCORNER : ACS_BTEE);
	else               addch(x+1 == BOARDSZ ? ACS_RTEE : ACS_PLUS);
	//
	attroff(COLOR_PAIR(color));
	attroff(highlight ? A_BOLD : A_NORMAL);
}

void
draw_message(const char *msg, int highlight) {
	move(0, 0);
	attron(highlight ? A_BLINK : A_NORMAL);
	attron(COLOR_PAIR(highlight ? colormsgwarn : colormsgok));
	printw(msg);
	attroff(COLOR_PAIR(highlight ? colormsgwarn : colormsgok));
	attroff(highlight ? A_BLINK : A_NORMAL);
	return;
}

void
draw_cursor(int x, int y, int show) {
	draw_box(x, y, BCH(x, y), show ? colorcursor : colorborder, show);
	return;
}

void
draw_board() {
	int i, j;
	for(i = 0; i < BOARDSZ; i++) {
		for(j = 0; j < BOARDSZ; j++) {
			draw_box(i, j, BCH(i, j), colorborder, 0);
		}
	}
	return;
}

void
draw_score() {
	int i, j;
	int black = 0, white = 0;
	for(i = 0; i < BOARDSZ; i++) {
		for(j = 0; j < BOARDSZ; j++) {
			if(board[i][j] == PLAYER1) white++;
			if(board[i][j] == PLAYER2) black++;
		}
	}
	attron(A_BOLD);
	move(box_top+3, box_left + 4*BOARDSZ + 10);
	printw("Player #1 ");
	addch(PLAYER1SYM|COLOR_PAIR(colorplayer1));
	printw(" : %d", white);
	move(box_top+5, box_left + 4*BOARDSZ + 10);
	printw("Player #2 ");
	addch(PLAYER2SYM|COLOR_PAIR(colorplayer2));
	printw(" : %d", black);
	attroff(A_BOLD);
	return;
}

bool outOfBoard(int x, int y){
	if( x < 0 || x >= BOARDSZ || y < 0 || y >= BOARDSZ)
		return true;
	return false;
}

void resetLegalPoints(){
	legalPoints.clear();
}

void updateLegalPoints(){
	for(int i = 0 ; i < BOARDSZ ; i ++){
		for(int j = 0 ; j < BOARDSZ ; j ++){
			bool isLegalPoint = false;
			// copy(&board[0][0], &board[0][0]+BOARDSZ*BOARDSZ,&tempBoard[0][0]);
			
			if(board[i][j] == 0){
				// copy(&tempBoard[0][0], &tempBoard[0][0]+BOARDSZ*BOARDSZ,&recordBoard[0][0]);

				for(int k = 0 ; k < DIR_NUM ; k ++){
					// cout << Directions[k][0] << " " << Directions[k][1] << ",";
					int currentX = i;
					int currentY = j;

					if(board[currentX+Directions[k][0]][currentY+Directions[k][1]] == 0 || 
						board[currentX+Directions[k][0]][currentY+Directions[k][1]] == mySign)
							continue;
					// copy(&tempBoard[0][0], &tempBoard[0][0]+BOARDSZ*BOARDSZ,&recordBoard[0][0]);
					do{
						// cout << currentX << " "<<currentY << "|";
						currentX += Directions[k][0];
						currentY += Directions[k][1];
						// cout << tempBoard[currentX][currentY] << ",";
						// tempBoard[currentX][currentY] = turn;
						// cout << currentX << " "<<currentY << ", ";
					}while(board[currentX][currentY] != mySign && !outOfBoard(currentX, currentY) && board[currentX][currentY] != 0);
					
					if(!outOfBoard(currentX,currentY) && board[currentX][currentY] != 0){
						isLegalPoint = true;
						break;
					} 
					
					
				}

			}

			
			if(isLegalPoint)
				legalPoints.push_back(i*BOARDSZ + j);
		}
	}

}

// sign : 以哪個sign為主 .
void turnChess(int x, int y,int sign){
	int currentX, currentY;
	// cout << x << y << endl;
	for(int i = 0 ; i < DIR_NUM ; i ++){
		currentX = x ;
		currentY = y ;

		
		if(board[currentX+Directions[i][0]][currentY+Directions[i][1]] == 0 || 
						board[currentX+Directions[i][0]][currentY+Directions[i][1]] == sign)
							continue;
		// cout << turn << " "<<board[currentX+Directions[i][0]][currentY+Directions[i][1]] << endl;

		do{
			// cout << currentX << "," << currentY << endl;
			currentX += Directions[i][0];
			currentY += Directions[i][1];
		}while(board[currentX][currentY] != sign && !outOfBoard(currentX, currentY) && board[currentX][currentY] != 0);

		if(!outOfBoard(currentX,currentY) && board[currentX][currentY] != 0 ){
			do{
				
				currentX -= Directions[i][0];
				currentY -= Directions[i][1];
				// cout << currentX << "," << currentY << endl;
				board[currentX][currentY] = sign;
			}while(currentX != x || currentY != y);
		}

	}
}

bool hasLegalPoints(){
	return legalPoints.size() > 0;
}

bool isLegalPoint(int x, int y){
	int target = x * 8 + y;
	vector<int>::iterator it = find(legalPoints.begin(), legalPoints.end(), target);
	return (it != legalPoints.end());
}

void showFinalMsg(){
	int myChess = 0 ;
	int OppChess = 0 ;
	for(int i = 0 ; i < BOARDSZ ; i ++){
		for(int j = 0 ; j < BOARDSZ ; j ++){
			if(board[i][j] == mySign) myChess ++;
			else if (board[i][j] == -1 * mySign) OppChess ++;
		}
	}
	if(myChess > OppChess) draw_message("You Win! (Press \'R\' to Restart)\t\t\t\t\t\t\t", 0);
	else if(myChess < OppChess) draw_message("You Lose! (Press \'R\' to Restart)\t\t\t\t\t\t\t", 0);
	else draw_message("Tie (Press \'R\' to Restart)\t\t\t\t\t\t\t", 0);
	draw_board();
	refresh();
}




