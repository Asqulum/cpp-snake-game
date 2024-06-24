//header of snek.cc
#ifndef snek_h
#define snek_h
#define MAXIMUM 17 //always odd and >= 5
#include <cstdlib>
#include <curses.h>

class snek {
public:
   snek(int difficulty);
   ~snek();
   int speed;
   void preGame();
   bool runGame();
private:
   bool win(), lose;
   int board[MAXIMUM][MAXIMUM];
   int length, headPosX, headPosY, lastDirection;
   //-----
   void print();
   int checkDirection();
   void deleteSnake();
   void moveHead(int direction);
   void decrease();
   void randomFood();
   void increaseLength();
   WINDOW* screenie;
};

#endif // snek_h
