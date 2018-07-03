#ifndef SNAKE_H_LEVELS
#define SNAKE_H_LEVELS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "deque.h"

#define LFIELDSIZE 30
#define HEADER "SNK"

#define DEFAULTY 23
#define DEFAULTX 80
#define DEFAULTNAMEBLANK "Blank Level"
#define DEFAULTNAMEBOX "Box Level"
#define DEFAULTAUTHOR "genlev"

typedef struct
{
    char *name;
    char *author;
    unsigned lvy, lvx;
    unsigned ncells;
    data *cells;
    unsigned snkncells;
    data *snkcells;
} Level;

int outOfBounds(data *cells, unsigned ncells, unsigned maxy, unsigned maxx)
{
    for (unsigned i = 0; i < ncells; i++)
    {
	if (cells[i].y >= maxy || cells[i].x >= maxx)
	    return 1;
    }
    return 0;
}

Level *readLevel(char *filename)
{
    FILE *read = fopen(filename, "rb");
    if (!read) return NULL;

    char *buf = malloc(strlen(HEADER) + 1);
    unsigned ubuf;
    
    fread(buf, sizeof(char), strlen(HEADER), read);        
    buf[strlen(HEADER)] = 0;
    if (strcmp(buf, HEADER))
    {
	free(buf);
	return NULL;
    }

    Level *ret = malloc(sizeof(Level));
    ret->name = calloc(LFIELDSIZE + 1, sizeof(char));
    ret->author = calloc(LFIELDSIZE + 1, sizeof(char));

    buf = realloc(buf, LFIELDSIZE + 1);
    buf[LFIELDSIZE] = 0;
    
    fread(buf, sizeof(char), LFIELDSIZE + 1, read);
    strcpy(ret->name, buf);
    fread(buf, sizeof(char), LFIELDSIZE + 1, read);
    strcpy(ret->author, buf);
    
    free(buf);

    fread(&ubuf, sizeof(unsigned), 1, read);
    ret->lvy = ubuf;
    fread(&ubuf, sizeof(unsigned), 1, read);
    ret->lvx = ubuf;
    
    fread(&ubuf, sizeof(unsigned), 1, read);
    ret->ncells = ubuf;
    ret->cells = malloc(ret->ncells * sizeof(data));
    fread(ret->cells, sizeof(data), ret->ncells, read);

    fread(&ubuf, sizeof(unsigned), 1, read);
    ret->snkncells = ubuf;
    ret->snkcells = malloc(ret->snkncells * sizeof(data));
    fread(ret->snkcells, sizeof(data), ret->snkncells, read);

    /* TODO: various level checks */
    
    fclose(read);
    return ret;
}

void freeLevel(Level **lev)
{
    free((*lev)->name);
    free((*lev)->author);
    free((*lev)->cells);
    free((*lev)->snkcells);
    free(*lev);
    
    *lev = NULL;
}
int writeLevel(Level *lev, char *filename)
{
    FILE *write = fopen(filename, "wb");
    if (!write) return -1;

    /* TODO: various level checks */
    
    char *head = HEADER;
    fwrite(head, sizeof(char), strlen(HEADER), write);
    fwrite(lev->name, sizeof(char), LFIELDSIZE + 1, write);
    fwrite(lev->author, sizeof(char), LFIELDSIZE + 1, write);
    fwrite(&(lev->lvy), sizeof(unsigned), 1, write);
    fwrite(&(lev->lvx), sizeof(unsigned), 1, write);
    fwrite(&(lev->ncells), sizeof(unsigned), 1, write);
    fwrite(lev->cells, sizeof(data), lev->ncells, write);
    fwrite(&(lev->snkncells), sizeof(unsigned), 1, write);
    fwrite(lev->snkcells, sizeof(data), lev->snkncells, write);

    fclose(write);
    return 0;
}

Level *createBlankLevel(void)
{
    Level *ret = malloc(sizeof(Level));

    ret->name = calloc(LFIELDSIZE + 1, sizeof(char));
    ret->author = calloc(LFIELDSIZE + 1, sizeof(char));
    
    strcpy(ret->name, DEFAULTNAMEBLANK);
    strcpy(ret->author, DEFAULTAUTHOR);

    ret->lvx = DEFAULTX;
    ret->lvy = DEFAULTY;

    ret->ncells = 0;
    ret->cells = NULL;

    ret->snkncells = 3;
    ret->snkcells = malloc(ret->snkncells * sizeof(data));
    for (int i = 1; i < 4; i++)
    {
	ret->snkcells[i - 1].y = 1;
	ret->snkcells[i - 1].x = i;	
    }

    return ret;
}

Level *createBoxLevel(void)
{
    Level *ret = malloc(sizeof(Level));
    ret->name = calloc(LFIELDSIZE + 1, sizeof(char));
    ret->author = calloc(LFIELDSIZE + 1, sizeof(char));
    
    strcpy(ret->name, DEFAULTNAMEBOX);
    strcpy(ret->author, DEFAULTAUTHOR);

    ret->lvx = DEFAULTX;
    ret->lvy = DEFAULTY;

    ret->ncells = (DEFAULTY + DEFAULTX) * 2 - 4;
    ret->cells = malloc(ret->ncells * sizeof(data));
    int c = 0;
    
    for (int i = 0; i < DEFAULTY; i++)
    {
	if (!i || i == DEFAULTY - 1)
	{
	    for (int j = 0; j < DEFAULTX; j++)
	    {
		ret->cells[c].y = i;
		ret->cells[c].x = j;
		c++;
	    }
	}
	else
	{
	    ret->cells[c].y = i;
	    ret->cells[c].x = 0;
	    c++;
	    ret->cells[c].y = i;
	    ret->cells[c].x = DEFAULTX - 1;
	    c++;
	}
    }

    ret->snkncells = 3;
    ret->snkcells = malloc(ret->snkncells * sizeof(data));
    for (int i = 1; i < 4; i++)
    {
	ret->snkcells[i - 1].y = 1;
	ret->snkcells[i - 1].x = i;	
    }

    return ret;
}

void printLevelData(Level *lev)
{
    printf("Level: %s\nAuthor: %s\nDimensions:\n\ty: %u\n\tx: %u\n", lev->name, lev->author, lev->lvy, lev->lvx);
    printf("Wall cells: %u\n[", lev->ncells);
    for (unsigned i = 0; i < lev->ncells; i++)
    {
	printf("{%u; %u}, ", lev->cells[i].y, lev->cells[i].x);
    }
    printf("%s]\nSnake cells: %u\n[", lev->ncells ? "\b\b" : "\0", lev->snkncells);
    for (unsigned i = 0; i < lev->snkncells; i++)
    {
	printf("{%u; %u}, ", lev->snkcells[i].y, lev->snkcells[i].x);
    }
    printf("\b\b]\n");
}
#endif
