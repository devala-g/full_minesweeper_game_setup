#ifndef PROJECT_MINESWEEPER_TILE_H
#define PROJECT_MINESWEEPER_TILE_H

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace sf;

class Tile {
    Sprite hiddenTile;
    Sprite revealedTile;
    Sprite flag;
    Sprite mine;
    Sprite numAdjMines;
    Vector2f position;

public:
    Tile(); //default constructor
    void Draw(RenderWindow& window, bool win);

    void SetPosition(float x, float y);

    Sprite& GetHiddenTile();

    bool isHidden;
    bool hasFlag;
    bool isAMine;
    int adjacentMineCounter = 0;
    vector<Tile*> adjacentTilesVec; //for recursive algorithm
    bool isChecked = false; //whether of not it has been checked by the recursive algorithm. false by default.
    bool debugOn = false;
};


#endif
