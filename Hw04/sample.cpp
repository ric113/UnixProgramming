// win後msg沒跑出來?

#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "othello.h"
#include "socket.h"


#define BUF_SIZE 128
#define DEBUG_MODE

static int width;
static int height;
static int cx = 3;
static int cy = 3;
static bool opponentNoLegalPoints = false;
static bool gameOver = false;
static bool opponentExit = false;

char* const short_options = "s:c:";  

struct option long_options[] = {  
     { "server",     1,   NULL,    's'     },  
     { "client",  1,   NULL,    'c'     },    
     {      0,     0,     0,     0},  
}; 

using namespace std;

int sockfd;
bool peerExit = false;
bool myTurn;
bool restart;


void * thr_fn(void *arg) {
	while(1){
		int nbytes;
		char buf[BUF_SIZE];

		
		nbytes = read(sockfd,buf,BUF_SIZE);

		if(nbytes < 1){	// socket close .
			peerExit  = true;
			break;
		}
		buf[nbytes] = '\0';
		
		// Update borad .
		int position;
		sscanf(buf,"%d",&position);
		// draw_message(to_string(position).c_str(), 0);
		// draw_board();
		// refresh();
		if(position == -1)
			opponentNoLegalPoints = true;
		else if(position == -2)
			opponentExit = true;
		else if(position == -3)
			gameOver = true;
		else if(position == -4)
			restart = true;
		else {
			int x = position / 8;
			int y = position % 8;
			board[x][y] = -1 * mySign;
			turnChess(x, y, -1 * mySign);
			draw_cursor(y, x, 1);
			draw_score();
			draw_board();
			draw_message("", 0);
			draw_message("Your turn!        ", 0);
			refresh();
			resetLegalPoints();
			updateLegalPoints();
		}

		myTurn = true;
	}
}



int
main(int argc, char *argv[])
{	
	int c;  
	char *l_opt_arg = NULL;  

	bool isServer = true;

	if(argc < 2){
		cout << "Usage : ./othello [-s|--server <Port> ] [-c|--client <IP-of-Player-1>:<Port-of-Player-1>]" << endl;
        return 0;
    }
	
    while((c = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)  { 
		switch (c){  
        case 's':  
        	l_opt_arg = optarg;
            break;  
        case 'c':  
        	isServer = false;
        	l_opt_arg = optarg;
            break;  
        default:
        	cout << "Usage : ./othello [-s|--server <Port> ] [-c|--client <IP-of-Player-1>:<Port-of-Player-1>]" << endl;
         	return 0;
        }  
    }  
    
    if(isServer){
    	int port;
    	sscanf(l_opt_arg, "%d", &port);

    	sockfd = serverTCP(port); 
    	// myTurn = true;
    	mySign = 1;
	} else{
    	int port;
    	char serverName[BUF_SIZE];
    	sscanf(l_opt_arg, "%[^:]:%d", serverName,&port);

    	sockfd = clientTCP(serverName, port); 
    	// myTurn = false;
    	mySign = -1;
	}

	// thread : listening oppenent msg .
	pthread_t ntid;
	pthread_create(&ntid, NULL, thr_fn, NULL);


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
	restart = false;
	

	clear();
	cx = cy = 3;
	init_board();
#ifdef DEBUG_MODE
	for(int i = 0 ; i < BOARDSZ ;i ++){
		if(i == 3 || i == 4 ) continue;
		for(int j = 0 ; j < BOARDSZ ; j ++){
			board[i][j] = 1;
		}
	}
#endif

	myTurn = isServer ? true:false;
	if(myTurn){
		draw_message("", 0);
		draw_message("Your turn!        ", 0);
	} else {
		draw_message("", 0);
		draw_message("Wait for opponent!", 1);
	}

	draw_board();
	draw_cursor(cx, cy, 1);
	draw_score();
	refresh();

	attron(A_BOLD);
	move(height-1, 0);	printw("Arrow keys: move; Space: put GREEN; Return: put PURPLE; R: reset; Q: quit");
	attroff(A_BOLD);

	while(true) {			// main loop

		int ch = getch();

		if(!myTurn){
			napms(1);	
			continue;
		}

		if(opponentExit){
			endwin();
			return 0;
		}

		if(gameOver){
			showFinalMsg();
			// pause();
		}

		if(restart)
			goto restart;

		
		int moved = 0;

		updateLegalPoints();

		if(!hasLegalPoints() && !gameOver){
			moved = 1;	// for refresh 顯示 .

			// opponent no legal points , too ?
			if(opponentNoLegalPoints){
				// Game over !
				showFinalMsg();

				// Tell oppenent .
				write(sockfd,"-3\0",3);
				gameOver = true;
				// myTurn = false;
				
				// pause();

			} else {
				// tell opponent I dont have legal points .
				// cout << "I dont have legal point!" ;
				write(sockfd,"-1\0",3);
				myTurn = false;
			}
		}
		
		switch(ch) {
		case 0x0d:
		case 0x0a:
		case KEY_ENTER:
			if(isLegalPoint(cy, cx)){
				board[cy][cx] = mySign;
				// cx , cy 是在顯示板上得座標 . 
				// 注意, 在board D.S. 上的x是顯示板的y , y是顯示板的x .
				turnChess(cy, cx, mySign);
				draw_cursor(cx, cy, 1);
				draw_score();
				draw_board();
				draw_message("", 0);
				draw_message("Wait for opponent!", 1);
				refresh();
				resetLegalPoints();
				opponentNoLegalPoints = false;

				string msgToSocket = to_string(cy * 8 + cx);
				// write postion info to oppent
				write(sockfd,msgToSocket.c_str(),msgToSocket.size() + 1);
				
				myTurn = false;
			} else
				break;

			break;
		case 'q':
		case 'Q':
			goto quit;
			break;
		case 'r':
		case 'R':
			if(gameOver){
				write(sockfd,"-4\0",3);
				goto restart;
			}
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
	write(sockfd,"-2\0",3);
	endwin();			// end curses mode


	return 0;
}