/**TODO:
 - Snekgroei correct laten verlopen -> bij beide versies implementeren
**/

/**
 BUGS:
 **/

/**
 newwin(groottey, groottex, posy, posx)
 wborder(window, links, rechts, top, bottom, lb, rb, lo, ro)
 **/

//main for snek.cc
#include <iostream>
#include "snek.h"
using namespace std;

void print_menu(WINDOW* menu, int selection, int n_options,
                char options[3][13]){
   int y = 4;
   for (int i = 1; i <= n_options; i++){
      if (selection == i){
         wattron(menu, A_REVERSE);
         wattron(menu, A_BOLD);
         mvwprintw(menu, y, 1, "%s", options[i-1]);
         wattroff(menu, A_BOLD);
         wattroff(menu, A_REVERSE);
      }
      else{mvwprintw(menu, y, 1, "%s", options[i-1]);}
      y++;
   }
}//print_menu

bool menu2(int &difficulty){
   WINDOW* menu;
   menu = newwin(9, 45, 0, 0);
   box(menu, 0, 0);
   keypad(menu, 1);
   int input, n_options = 4, selection = 1;
   bool optionselected = 0;
   char options[4][13] = {"Snail", "Snake", "Bullet Train", "Go Back"};
   mvwprintw(menu, 1, 1, "Use arrow keys or W & S to go up and down.");
   mvwprintw(menu, 2, 1, "Press enter to select a choice.");
   refresh();
   print_menu(menu, selection, n_options, options);
   while (!optionselected){
      input = wgetch(menu);
      switch (input) {
         case KEY_UP:
         case 119:
            if (selection == 1){selection = n_options;}
            else {selection--;}
            break;
         case KEY_DOWN:
         case 115:
            if (selection == n_options){selection = 1;}
            else {selection++;}
            break;
         case 10:
            optionselected = 1;
            if (selection == 1){difficulty = 3;}
            else if (selection == 2){difficulty = 2;}
            else if (selection == 3){difficulty = 1;}
            else if (selection == 4){difficulty = -1;}
            break;
         default:
            break;
      }
      print_menu(menu, selection, n_options, options);
   }
   wclear(menu);
   wrefresh(menu);
   endwin();
   if (selection == 4){return false;}
   return true;
}//menu2

bool menu1(int &nplayers){
   WINDOW* menu;
   menu = newwin(8, 46, 0, 0);
   box(menu, 0, 0);
   keypad(menu, 1);
   int input,n_options = 3, selection = 1;
   bool optionselected = 0;
   char options[3][13] = {"1 player", "2 players", "Exit"};
   mvwprintw(menu, 1, 1, "Use arrow keys or W and S to go up and down.");
   mvwprintw(menu, 2, 1, "Press enter to select a choice.");
   refresh();
   print_menu(menu, selection, n_options, options);
   while (!optionselected){
      input = wgetch(menu);
      switch (input) {
         case KEY_UP:
         case 119:
            if (selection == 1){selection = n_options;}
            else {selection--;}
            break;
         case KEY_DOWN:
         case 115:
            if (selection == n_options){selection = 1;}
            else {selection++;}
            break;
         case 10:
            optionselected = 1;
            if (selection == 3){nplayers = -1;}
            else {/*nplayers = selection;*/nplayers = 1;}///VERANDER ZODRA MULTIPLAYER
            break;
         default:
            break;
      }
      print_menu(menu, selection, n_options, options);
   }
   wclear(menu);
   wrefresh(menu);
   endwin();
   if (selection == 3){return false;}
   return true;
}//menu1

bool postGame(bool win){
   WINDOW* post;
   post = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
   flushinp();
   cbreak();
   int flush = 0;
   if (!win){mvwprintw(post, 0, 0, "You Lost!\n");}
   else {mvwprintw(post, 0, 0, "You Won!\n");}
   wprintw(post, "Press enter to leave or R to restart.");
   wrefresh(post);
   while (flush != 10 && flush != 114){flush = getch();}
   wclear(post);
   wrefresh(post);
   endwin();
   if (flush == 10){return 0;}
   return 1;
}

int main() {
   system("clear");
   initscr(); noecho(); curs_set(0); cbreak(); start_color();//init screen settings
   int difficulty = -1, nplayers = -1;
   bool win;
   snek* game;
   do {
      bool firstmenu, secondmenu = 0;
      while (!secondmenu){
         firstmenu = menu1(nplayers);
         if (!firstmenu){return 1;}
         secondmenu = menu2(difficulty);
      }
      game = new snek(difficulty);
      game->preGame();
      halfdelay(game->speed);
      win = game->runGame();
      delete game; game = NULL;
   } while (postGame(win));
   system("stty sane");
   return 0;
}//main
