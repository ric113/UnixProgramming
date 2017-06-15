### Homework 04 - Two-Player Networked Othello (Reversi) Game

In this homework, we are going to implement a simple othello (reversi) game that allows two players play an othello game over the Internet. For the rules of the game, please refer to the links [ [English](https://en.wikipedia.org/wiki/Reversi) | [Chinese](https://zh.wikipedia.org/wiki/%E9%BB%91%E7%99%BD%E6%A3%8B) ] . <br><br>

The scenario of this homework is as follows. Suppose our program name is othello. We can have player #1 served as the server and player #2 served as the client. Player #1 launches the server using the command <br>

``` shell
$ ./othello -s 12345
```

to launch the server and wait for the connection from player #2 via port number 12345. Player #2 then launches the same program, but acts as a client, to connect to the server. The command for player #2 is: <br>

``` shell
$ ./othello -c <IP-of-Player-1>:12345
```

To simplify your implementation, the game board can be drawn using the [ncurses](http://man7.org/linux/man-pages/man3/ncurses.3x.html) library (libncurses5-dev package on Ubuntu). <br><br>

#### Requirements :

* Your program can act as either a server (player #1) or a client (player #2) by using the respective command arguments.
* A server has to wait for a client connection.
* A client can connect to the given server (IP address or host name).
* Once connected, display the game board. The game always starts from player #1 (server).
* Player can only put pieces (discs) on valid places (see game rule). 
* Display correct number of pieces on the game board for the both players.
* Implement the rest of game logics.
* When there is no more moves, display a message to show the player wins or loses.
* Ensure the both two players have the same view of game board. If either the client or the server quits, the peer has to be terminated as well.

#### Result :

* Server 
![](https://github.com/ric113/UnixProgramming/tree/master/Hw04/server.png)
* Client 
![](https://github.com/ric113/UnixProgramming/tree/master/Hw04/client.png)

