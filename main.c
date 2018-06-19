#include <stdio.h>
#include "deque.h"
#include "game.h"

int main(void)
{
    Game.init();

    int ret = Game.loop(Game.win, Game.gamedata);
    
    Game.kill();
    return ret;
}
