#include <stdio.h>
#include "deque.h"
#include "game.h"

int main(void)
{
    Game.init();

    int ret = Game.loop(Game.win, Game.gamedata);

    werase(Game.win[1]->wptr);
    mvwprintw(Game.win[1]->wptr, 0, 0, "Game over! Your score: %u", Game.gamedata->score);
    wrefresh(Game.win[1]->wptr);
    wgetch(Game.win[1]->wptr);
    
    Game.kill();
    return ret;
}
