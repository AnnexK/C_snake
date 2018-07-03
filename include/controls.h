#ifndef SNAKE_H_CONTROLS
#define SNAKE_H_CONTROLS

#include <curses.h>
#include "game.h"
#include "snake.h"
#include "windows.h"


int gameLoop(WindowHandle **wins, void *data)
{
    wtimeout(wins[0]->wptr, 1000); //ncurses
    GameData *gd = data;

    gd->S = gd->create(gd);
    gd->genfood(gd);
    
    int retval;
    vector curmov;
    
    while (!(retval = gd->checkcol(gd)))
    {
        wins[0]->display(wins[0], gd);
	wins[1]->display(wins[1], gd);
	
        curmov.x = 0;
	curmov.y = 0;
	int mov = wgetch(wins[0]->wptr);
	switch(mov)
	{
	case KEY_LEFT:	
	    curmov.x = -1;
	    break;
	case KEY_RIGHT:
	    curmov.x = 1;
	    break;
	case KEY_UP:
	    curmov.y = -1;
	    break;
	case KEY_DOWN:
	    curmov.y = 1;
	    break;
	default:
	    break;
	}

	if (gd->S->direction.y != curmov.y * -1 && gd->S->direction.x != curmov.x * -1 && (curmov.x || curmov.y))
	{
	    gd->S->direction.y = curmov.y;
	    gd->S->direction.x = curmov.x;
	}

	gd->mv(gd);
	if (gd->S->snk->head->d->y == gd->food.y && gd->S->snk->head->d->x == gd->food.x)
	{
	    gd->grow(gd->S);
	    gd->score += 10;
	    if (retval = gd->genfood(gd)) break;
	}
    }

    nocbreak();
    cbreak();

    werase(wins[1]->wptr);
    mvwprintw(wins[1]->wptr, 0, 0, "Game over! Your score: %u", gd->score);
    wrefresh(wins[1]->wptr);
    wgetch(wins[1]->wptr);

    gd->delete(gd->S);
    freeLevel(&(gd->curlevel));
    return retval;
}

#endif
