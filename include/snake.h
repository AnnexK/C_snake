#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include <stdlib.h>
#include <math.h>
#include "deque.h"

typedef struct
{
    signed y, x;    
} vector;

typedef struct
{
    deque *snk;
    unsigned len;
    vector direction;
    data prevcell;
} Snake;


typedef struct gd
{
    Snake *S;
    data food;

    Snake *(*create)(void);
    void (*delete)(Snake *);
    
    void (*mv)(Snake *);
    void (*grow)(Snake *);
    int (*checkcol)(struct gd *);
    void (*genfood)(struct gd *);

    unsigned score;
    unsigned fieldY, fieldX;
} GameData;

/* createSnake: creates a snake in a left upper corner with size of 3, directed to the right */
Snake *createSnake(void)
{
    Snake *ret = malloc(sizeof(Snake));

    ret->len = 3;
    ret->direction.y = 0;
    ret->direction.x = 1;
    ret->prevcell.y = 0;
    ret->prevcell.x = 0;

    ret->snk = initDeque();

    data app = {1, 1};
    for (int i = 1; i < 4; i++)
    {
	prepend(ret->snk, app);
	(app.x)++;
    }

    return ret;
}

void moveSnake(Snake *S)
{
    data newHead = {S->snk->head->d->y, S->snk->head->d->x};
    newHead.y += (S->direction).y;
    newHead.x += (S->direction).x;

    data *toFree = popTail(S->snk);
    S->prevcell.y = toFree->y;
    S->prevcell.x = toFree->x;
    free(toFree);

    prepend(S->snk, newHead);
}

void growSnake(Snake *S)
{
    append(S->snk, S->prevcell);
}

/* isCollision: checks whether there is a collision of snake cells */
int isCollision(GameData *gd)
{
    data head = {gd->S->snk->head->d->y, gd->S->snk->head->d->x};
    if (head.y < 1 || head.x < 1 || head.y >= gd->fieldY - 1 || head.x >= gd->fieldX - 1) return 1;
    
    for (dnptr cell = gd->S->snk->head->next; cell; cell = cell->next)
    {
	if (head.y == cell->d->y && head.x == cell->d->x)
	    return 2;
    }
    return 0;
}

void deleteSnake(Snake *S)
{
    killDeque(S->snk);
    S->len = 0;
    (S->direction).y = 0;
    (S->direction).x = 0;
    free(S);
}

int notInSnake(deque *s, unsigned y, unsigned x)
{
    for (dnptr cell = s->head; cell; cell = cell->next)
    {
	if (cell->d->y == y && cell->d->x == x)
	    return 0;
    }

    return 1;
}

void generateFood(GameData *gd)
{
    unsigned y, x;
    do
    {
	y = 1 + rand() % (gd->fieldY - 2);
	x = 1 + rand() % (gd->fieldX - 2);
    } while (!notInSnake(gd->S->snk, y, x));

    gd->food.y = y;
    gd->food.x = x;    
}
#endif
