#ifndef SNAKE_H_WINDOWS
#define SNAKE_H_WINDOWS

#include <curses.h>
#include "game.h"
#include "snake.h"

typedef struct winhandle
{
    WINDOW *wptr;
    unsigned w, h, y, x;
    void (*display)(struct winhandle *, void *);
} WindowHandle;

WindowHandle *setwin(unsigned h, unsigned w, unsigned y, unsigned x)
{
    WindowHandle *ret = malloc(sizeof(WindowHandle));
    
    ret->wptr = newwin(w, h, y, x);
    ret->w = w;
    ret->h = h;
    ret->y = y;
    ret->x = x;

    return ret;
}

void display_aux_game(WindowHandle *this, void *data)
{
    werase(this->wptr);
    GameData *gd = data;

    mvwprintw(this->wptr, 0, 0, "Snake Demo\tScore: %u", gd->score);
    wrefresh(this->wptr);
}

void display_main_game(WindowHandle *this, void *data)
{
    werase(this->wptr);
    
    GameData *gd = (GameData *)data;
   
    mvwaddch(this->wptr, gd->food.y, gd->food.x, '*');
    for (dnptr cell = gd->S->snk->head; cell; cell = cell->next)
    {
	mvwaddch(this->wptr, cell->d->y, cell->d->x, 'o');
    }
    for (int i = 0; i < gd->curlevel->ncells; i++)
    {
	mvwaddch(this->wptr, gd->curlevel->cells[i].y, gd->curlevel->cells[i].x, '#');
    }
    
    wrefresh(this->wptr);
}

#endif
