#include "levels.h"

int main(void)
{    
    Level *blank = createBlankLevel();
    writeLevel(blank, "blank.slv");
    free(blank);

    Level *box = createBoxLevel();
    writeLevel(box, "box.slv");
    free(box);

    return 0;
}
