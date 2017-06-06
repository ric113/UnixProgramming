#ifndef __OTHELLO_H__
#define	__OTHELLO_H__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <map>
#include <vector>

#define	BOARDSZ		8
#define	PLAYER1		+1
#define	PLAYER2		-1

using namespace std;

extern int board[BOARDSZ][BOARDSZ];
extern vector<int> legalPoints;
extern int turn ;

void init_board();
void init_colors();

// msg顯示在第一行，highlight : 0 -> green ， 1 -> red flash .
void draw_message(const char *msg, int highlight);
// hide : 0 -> white , 1 -> yellow .
void draw_cursor(int x, int y, int hide);
void draw_board();
void draw_score();

void updateLegalPoints();
void resetLegalPoints();
bool outOfBoard(int x, int y);
void turnChess(int x, int y);
bool hasLegalPoints();


#endif	/* __OTHELLO_H__ */