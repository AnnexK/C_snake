#ifndef SNAKE_H_SNAKE
#define SNAKE_H_SNAKE

#include <stdlib.h>
#include <math.h>

#include "deque.h"
#include "levels.h"

#define DOTPR(vec1, vec2) ((vec1).y * (vec2).y + (vec1).x * (vec2).x)

typedef struct
{
    int y, x;    
} vector;

typedef struct
{
    deque *snk;
    int len;
    vector direction;
    data prevcell;
} Snake;


typedef struct gd
{
    Snake *S;
    data food;

    Snake *(*create)(struct gd *);
    void (*delete)(Snake *);
    
    void (*mv)(struct gd *);
    void (*grow)(Snake *);
    int (*checkcol)(struct gd *);
    int (*genfood)(struct gd *);

    unsigned score;
    unsigned foodEaten;
    
    Level *curlevel;
} GameData;

Snake *createSnake(GameData *);
vector calculateVector(GameData *, Snake *);

void deleteSnake(Snake *);

void moveSnake(GameData *);
void growSnake(Snake *);
int generateFood(GameData *);

int isSnakeCollision(GameData *);
int isWallCollision(GameData *);
int isCollision(GameData *);

/* createSnake: creates a snake according to information in level struct */
Snake *createSnake(GameData *gd)
{
    if (!gd->curlevel) return NULL;
    
    Snake *ret = malloc(sizeof(Snake));

    ret->snk = initDeque();
    ret->len = gd->curlevel->snkncells;
    
    for (int i = 0; i < ret->len; i++)
    {
	prepend(ret->snk, gd->curlevel->snkcells[i]);
    }
    
    ret->direction = calculateVector(gd, ret);
    ret->prevcell.y = 0;
    ret->prevcell.x = 0;
    
    return ret;
}

vector calculateVector(GameData *gd, Snake *S)
{
    dnptr head = S->snk->head;
    
    vector ret;
    if (S->len > 1) // more than one snk cell - vector is calculatable
    {
	ret.y = head->d->y - head->next->d->y;
	ret.x = head->d->x - head->next->d->x;
    }
    else // one cell - vector is uncalculatable, defaulting to UP direction
    {
	ret.y = -1;
	ret.x = 0;
    }

    int flag = 0;
    
    return ret;
}

void moveSnake(GameData *gd)
{
    Snake *S = gd->S;
    
    data newHead = {S->snk->head->d->y, S->snk->head->d->x};
    newHead.y += (S->direction).y;
    newHead.x += (S->direction).x;
    if (newHead.y < 0) newHead.y = gd->curlevel->lvy - 1;
    else if (newHead.y >= gd->curlevel->lvy) newHead.y = 0;
    
    if (newHead.x < 0) newHead.x = gd->curlevel->lvx - 1;
    else if (newHead.x >= gd->curlevel->lvx) newHead.x = 0;
    
    data *toFree = popTail(S->snk);
    S->prevcell.y = toFree->y;
    S->prevcell.x = toFree->x;
    free(toFree);

    prepend(S->snk, newHead);
}

void growSnake(Snake *S)
{
    append(S->snk, S->prevcell);
    (S->len)++;
}

/* isSnakeCollision: checks whether there is a collision of snake cells */
int isSnakeCollision(GameData *gd)
{
    dnptr head = gd->S->snk->head;
    for (dnptr cell = head->next; cell; cell = cell->next)
    {
	if (head->d->y == cell->d->y && head->d->x == cell->d->x)
	    return 1;
    }
    return 0;
}

int isWallCollision(GameData *gd)
{
    data *head = gd->S->snk->head->d;

    for (int i = 0; i < gd->curlevel->ncells; i++)
    {
	if (head->y == gd->curlevel->cells[i].y && head->x == gd->curlevel->cells[i].x)
	    return 1;
    }
    return 0;
}

int isCollision(GameData *gd)
{
    return isSnakeCollision(gd) || isWallCollision(gd);
}

void deleteSnake(Snake *S)
{
    killDeque(S->snk);
    S->len = 0;
    (S->direction).y = 0;
    (S->direction).x = 0;
    free(S);
}

int generateFood(GameData *gd)
{
    int nempty = gd->curlevel->lvy * gd->curlevel->lvx - gd->curlevel->ncells - gd->S->len;
    if (!nempty) return -1; // basically a game-over

    data *empty = malloc(nempty * sizeof(data));
    int cempty = 0;
    int flag = 0;

    for (int i = 0; i < gd->curlevel->lvy; i++)
    {
	for (int j = 0; j < gd->curlevel->lvx; j++)
	{
	    flag = 1; // no collisions yet
	    for (int k = 0; k < gd->curlevel->ncells && flag; k++)
	    {
		if (i == gd->curlevel->cells[k].y && j == gd->curlevel->cells[k].x)
		    flag = 0;
	    }
	    for (dnptr snkc = gd->S->snk->head; snkc && flag; snkc = snkc->next)
	    {
		if (i == snkc->d->y && j == snkc->d->x)
		    flag = 0;
	    }
	    if (flag)
	    {
		empty[cempty].y = i;
		empty[cempty].x = j;
		cempty++;
	    }
	}
    }

    int choice = rand() % nempty;
    gd->food.y = empty[choice].y;
    gd->food.x = empty[choice].x;
    free(empty);

    return 0;
}
#endif
