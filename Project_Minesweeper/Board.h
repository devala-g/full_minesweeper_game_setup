#ifndef PROJECT_MINESWEEPER_BOARD_H
#define PROJECT_MINESWEEPER_BOARD_H

#include "Tile.h"
#include "Random.h"

struct Board {

    Sprite happyFace;
    Sprite loseFace;
    Sprite winFace;
    Sprite debugger;
    Sprite test1;
    Sprite test2;
    Sprite test3;
    Sprite digits;
    int mineCounter; //changes based upon player planting flags.
    bool debugOn = false;
    bool firstMove = true;
    bool lost = false;
    bool win = false;

    Board(int windowWidth, int windowHeight, int numMines);
    void Draw(RenderWindow& window);
};


#endif
