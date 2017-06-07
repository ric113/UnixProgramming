#include <iostream>
#include "othello.h"

static int width;
static int height;
static int cx = 3;
static int cy = 3;
static bool opponentNoLegalPoints = false;
static bool gameOver = false;

using namespace std;

int
main()
{	
	// cout << "Yo" << endl;
	initscr();			// start curses mode 
	getmaxyx(stdscr, height, width);// get screen size

	cbreak();			// disable buffering
					// - use raw() to disable Ctrl-Z and Ctrl-C as well,
	halfdelay(1);			// non-blocking getch after n * 1/10 seconds
	noecho();			// disable echo
	keypad(stdscr, TRUE);		// enable function keys and arrow keys
	curs_set(0);			// hide the cursor

	init_colors();

restart:

	opponentNoLegalPoints = false;
	gameOver = false;

	clear();
	cx = cy = 3;
	init_board();
	draw_board();
	draw_cursor(cx, cy, 1);
	draw_score();
	refresh();

	attron(A_BOLD);
	move(height-1, 0);	printw("Arrow keys: move; Space: put GREEN; Return: put PURPLE; R: reset; Q: quit");
	attroff(A_BOLD);

	while(true) {			// main loop
		int ch = getch();
		int moved = 0;

		updateLegalPoints();

		if(!hasLegalPoints() && !gameOver){
			moved = 1;	// for refresh 顯示 .

			// opponent no legal points , too ?
			if(opponentNoLegalPoints){
				// Game over !
				// count black / white chess num .
				int black = 0 ;
				int white = 0 ;
				for(int i = 0 ; i < BOARDSZ ; i ++){
					for(int j = 0 ; j < BOARDSZ ; j ++){
						if(board[i][j] == PLAYER1) white ++;
						else if (board[i][j] == PLAYER2) black ++;
					}
				}
					
				if(white > black) cout << "Player1 (white) Win!";
				else if(white < black) cout << "Player2 (black) Win!";
				else cout << "Tie!";

				gameOver = true;

			} else {
				// tell opponent I dont have legal points .
				cout << "I dont have legal point!" ;
				opponentNoLegalPoints = true;
			}
			// turn to opponent .
			turn *= -1;
		}
		

		switch(ch) {
		// space .
		/*
		case ' ':
			board[cy][cx] = PLAYER1;
			draw_cursor(cx, cy, 1);
			draw_score();
			break;
		*/
		case 0x0d:
		case 0x0a:
		case KEY_ENTER:
		
			if(isLegalPoint(cy, cx)){
				board[cy][cx] = turn;
				// cx , cy 是在顯示板上得座標 . 
				// 注意, 在board D.S. 上的x是顯示板的y , y是顯示板的x .
				turnChess(cy, cx);
				draw_cursor(cx, cy, 1);
				draw_score();
				draw_board();
				refresh();
				resetLegalPoints();
				opponentNoLegalPoints = false;
			} else
				break;
			turn *= -1;

			break;
		case 'q':
		case 'Q':
			goto quit;
			break;
		case 'r':
		case 'R':
			goto restart;
			break;
		case 'k':
		case KEY_UP:
			draw_cursor(cx, cy, 0);
			cy = (cy-1+BOARDSZ) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'j':
		case KEY_DOWN:
			draw_cursor(cx, cy, 0);
			cy = (cy+1) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'h':
		case KEY_LEFT:
			draw_cursor(cx, cy, 0);
			cx = (cx-1+BOARDSZ) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		case 'l':
		case KEY_RIGHT:
			draw_cursor(cx, cy, 0);
			cx = (cx+1) % BOARDSZ;
			draw_cursor(cx, cy, 1);
			moved++;
			break;
		}

		if(moved) {
			refresh();
			moved = 0;
		}

		resetLegalPoints();
		napms(1);		// sleep for 1ms
	}

quit:
	endwin();			// end curses mode


	return 0;
}