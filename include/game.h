#ifndef SNAKE_H_GAME
#define SNAKE_H_GAME

#include <time.h>
#include <stdlib.h>
#include <curses.h>

#include "snake.h"
#include "windows.h"
#include "controls.h"

#define NAMESIZE 3

typedef char name[NAMESIZE + 1];

typedef enum state
{
    MENU,
    GAME,
    HIGHSCORE,
    SETTINGS
} State;

void initcurses(void);
int exitcurses(void);

struct
{
    State state;
    int (*loop)(WindowHandle **, void *);
    
    WindowHandle *win[2];
    
    GameData *gamedata;
    
    FILE *highscore;

    void (*init)(void);
    int (*kill)(void);
} Game = {
    GAME,
    gameLoop,
    {
	NULL,
	NULL
    },
    NULL,
    NULL,
    initcurses,
    exitcurses
};

void initcurses(void)
{
    srand(time(NULL));
    
    initscr();
    cbreak();
    noecho();
	
    int y, x;
    getmaxyx(stdscr, y, x);

    Game.win[0] = setwin(x, y-2, 0, 0);
    Game.win[0]->display = display_main_game;
    Game.win[1] = setwin(x, 2, y-2, 0);
    Game.win[1]->display = display_aux_game;

    keypad(Game.win[0]->wptr, TRUE);
    //nodelay(Game.win[0]->wptr, TRUE);
	
    Game.gamedata = malloc(sizeof(GameData));
    Game.gamedata->S = createSnake();
    Game.gamedata->score = 0;
    Game.gamedata->mv = moveSnake;
    Game.gamedata->grow = growSnake;
    Game.gamedata->checkcol = isCollision;

    Game.gamedata->fieldY = Game.win[0]->w;
    Game.gamedata->fieldX = Game.win[0]->h;
    
    Game.gamedata->genfood = generateFood;
    Game.gamedata->genfood(Game.gamedata);
}

int exitcurses(void)
{
    delwin(Game.win[0]->wptr);
    delwin(Game.win[1]->wptr);
    free(Game.win[0]);
    free(Game.win[1]);

    deleteSnake(Game.gamedata->S);
    free(Game.gamedata);
    endwin();

    return 0;
}

#endif
