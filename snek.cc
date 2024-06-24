//snake clone game
#include <thread>
#include <chrono>
#include <ctime>
#include "snek.h"
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

snek::snek(int difficulty){
  // start_color();
   init_color(COLOR_RED, 1000, 1000, 0);
   init_pair(1, COLOR_RED, COLOR_RED);
   init_pair(2, COLOR_GREEN, COLOR_GREEN);
   init_pair(3, COLOR_WHITE, COLOR_WHITE);
   init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
   screenie = newwin(MAXIMUM, ((MAXIMUM-2)*2)+2, (getmaxy(stdscr)/2)-MAXIMUM/2, (getmaxx(stdscr)/2)-MAXIMUM);
   keypad(screenie, 1);
   wborder(screenie, 0, 0, 0, 0, 0, 0, 0, 0);
   length = 3; speed = difficulty;
   for (int i = 0; i < MAXIMUM; i++){//height
      for (int j = 0; j < MAXIMUM; j++){//width
         if (i == 0 || i == MAXIMUM-1 || j == 0 || j == MAXIMUM-1){
            board[i][j] = -1;
         }
         else {board[i][j] = 0;}
      }
   }
   board[(MAXIMUM/2)][(MAXIMUM/2)] = length;
   headPosX = MAXIMUM/2; headPosY = MAXIMUM/2;
   lastDirection = 3;
   randomFood();
}

snek::~snek(){
   wclear(screenie);
   wrefresh(screenie);
   endwin();
}

//0 = background, -1 = edges, -2 = snacc, >=1 = snek
void snek::print(){
   int k = 1;
   for (int i = 1; i < MAXIMUM-1; i++){
      for (int j = 1; j < MAXIMUM-1; j++){
         if (board[i][j] == -2){
            wattron(screenie, COLOR_PAIR(1));
            mvwprintw(screenie, i, k, "  ");
            wattroff(screenie, COLOR_PAIR(1));
         }
         else if (board[i][j] == length){
            wattron(screenie, COLOR_PAIR(4));
            mvwprintw(screenie, i, k, "  ");
            wattroff(screenie, COLOR_PAIR(4));
         }
         else if (board[i][j] >= 1){
            wattron(screenie, COLOR_PAIR(2));
            mvwprintw(screenie, i, k, "  ");
            wattroff(screenie, COLOR_PAIR(2));
         }
         else{
            wattron(screenie, COLOR_PAIR(3));
            mvwprintw(screenie, i, k, "  ");
            wattroff(screenie, COLOR_PAIR(3));
         }
         k+=2;
      }
      k = 1;
   }
   wrefresh(screenie);
}//print

bool snek::win(){
   for (int i = 1; i < MAXIMUM-1; i++){
      for (int j = 1; j < MAXIMUM-1; j++){
         if (board[i][j] == 0){return false;}
      }
   }
   return true;
}//win

void snek::preGame(){
   WINDOW* instructions;
   instructions = newwin(1, 41, getbegy(screenie)-1, getbegx(screenie)-4);
   mvwprintw(instructions, 0, 0, "Use arrow keys or WASD to move the snake.");
   wrefresh(instructions);
   print();
   int dir = -1;
   while (dir == -1){dir = checkDirection();}
   moveHead(dir);
   decrease();
   print();
   wclear(instructions);
   wrefresh(instructions);
}//preGame

bool snek::runGame(){
   while (!win() && !lose){
      system_clock::time_point tijd = system_clock::now();
      moveHead(checkDirection());
      decrease();
      flushinp();
      sleep_until(tijd + milliseconds(speed*100));
      print();
   }
   if (lose){return false;}
   return true;
}//runGame

void snek::decrease(){//verminder alle slangspaces met 1
   for (int i = 1; i < MAXIMUM-1; i++){//hoogte
      for (int j = 1; j < MAXIMUM-1; j++){//breedte
         if (board[i][j] >= 1){board[i][j]--;}
      }
   }
}//decreasw

void snek::moveHead(int direction){
   bool foodEaten = 0;
   switch (direction) {
      case -1:
         moveHead(lastDirection); return;
         break;
      case 0:
         if (board[headPosY-1][headPosX] == length-1){moveHead(lastDirection); return;}
         if (board[headPosY-1][headPosX] == -1 ||
             board[headPosY-1][headPosX] >= 1){lose = 1; return;}
         if (board[headPosY-1][headPosX] == -2){/*board[headPosY][headPosX]++;*/ foodEaten = 1;}
         headPosY--;
         break;
      case 1:
         if (board[headPosY][headPosX-1] == length-1){moveHead(lastDirection); return;}
         if (board[headPosY][headPosX-1] == -1 ||
             board[headPosY][headPosX-1] >= 1){lose = 1; return;}
         if (board[headPosY][headPosX-1] == -2){foodEaten = 1;}
         headPosX--;
         break;
      case 2:
         if (board[headPosY+1][headPosX] == length-1){moveHead(lastDirection); return;}
         if (board[headPosY+1][headPosX] == -1 ||
             board[headPosY+1][headPosX] >= 1){lose = 1; return;}
         if (board[headPosY+1][headPosX] == -2){foodEaten = 1;}
         headPosY++;
         break;
      case 3:
         if (board[headPosY][headPosX+1] == length-1){moveHead(lastDirection); return;}
         if (board[headPosY][headPosX+1] == -1 ||
             board[headPosY][headPosX+1] >= 1){lose = 1; return;}
         if (board[headPosY][headPosX+1] == -2){foodEaten = 1;}
         headPosX++;
         break;
   }
   lastDirection = direction;
   if (foodEaten){increaseLength(); /*length++;*/ randomFood();}
   board[headPosY][headPosX] = length + 1;
}//moveHead

int snek::checkDirection(){
   int input;
   input = wgetch(screenie);
   if (input == 119 || input == KEY_UP){return 0;}
   if (input == 97 || input == KEY_LEFT){return 1;}
   if (input == 115 || input == KEY_DOWN){return 2;}
   if (input == 100 || input == KEY_RIGHT){return 3;}
   return -1;
}//checkDirection

void snek::randomFood(){
   bool falsePlacement = 1;
   srand(time(NULL));
   while (falsePlacement){
      falsePlacement = 0;
      int place = rand()%((MAXIMUM-2)*(MAXIMUM-2)), counter = 0;
      for (int i = 1; i < MAXIMUM-1 && !falsePlacement; i++){
         for (int j = 1; j < MAXIMUM-1 && !falsePlacement; j++){
            if (counter == place){
               if (board[i][j] == 0){board[i][j] = -2; return;}
               else {falsePlacement = 1;}
            }
            else {counter++;}
         }
      }
   }
}//randomFood

void snek::increaseLength(){
   for (int i = 1; i < MAXIMUM-1; i++){
      for (int j = 1; j < MAXIMUM-1; j++){
         if (board[i][j] >= 1){board[i][j]++;}
      }
   }
   length++;
}//increaseLength
