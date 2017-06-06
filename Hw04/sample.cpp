#include <iostream>
#include "othello.h"

static int width;
static int height;
static int cx = 3;
static int cy = 3;

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

	// updateLegalPoints();

	while(true) {			// main loop
		int ch = getch();
		int moved = 0;
		
		

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
			updateLegalPoints();
			// has legal point ?
			// 
			if(turn == -1){
				board[cy][cx] = PLAYER2;
			}
			else{
				board[cy][cx] = PLAYER1;
			}
			turnChess(cy, cx);
			draw_cursor(cx, cy, 1);
			draw_score();
			draw_board();
			refresh();
  			turn *= -1;
			freeBoards();
			
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

		napms(1);		// sleep for 1ms
	}

quit:
	endwin();			// end curses mode

	return 0;
}