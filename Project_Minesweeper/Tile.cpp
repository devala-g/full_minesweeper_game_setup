#include "Tile.h"

Tile::Tile() {
    hiddenTile.setTexture(TextureManager::GetTexture("tile_hidden")); //set to hidden by default
    revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
    flag.setTexture(TextureManager::GetTexture("flag"));
    mine.setTexture(TextureManager::GetTexture("mine"));
    isHidden = true;
    hasFlag = false;
    isAMine = false;
}

void Tile::Draw(RenderWindow& window, bool win) {
    if (isHidden) {
        hiddenTile.setPosition(position);
        window.draw(hiddenTile);
        if (hasFlag){
            //Draw flag on top of hidden tile:
            flag.setPosition(position);
            window.draw(flag);
        }
        if (win){
            flag.setPosition(position);
            window.draw(flag);
        }
    }
    else {//if revealed:
        revealedTile.setPosition(position);
        window.draw(revealedTile);
        if (isAMine) {
            mine.setPosition(position);
            window.draw(mine);
            if (win){
                hiddenTile.setPosition(position);
                window.draw(hiddenTile);
                flag.setPosition(position);
                window.draw(flag);
            }
        }
        else { //if it is not a mine and is revealed, display number of adjacent mines:
            if (adjacentMineCounter > 0 && adjacentMineCounter <= 8){
                string textureName = "number_" + to_string(adjacentMineCounter);
                numAdjMines.setTexture(TextureManager::GetTexture(textureName));
                numAdjMines.setPosition(position);
                window.draw(numAdjMines);
            }
        }
    }
    if (debugOn && isAMine && !win) { //if debug is on display OVER EVERYTHING ELSE:
        mine.setPosition(position);
        window.draw(mine);
    }
}

void Tile::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

Sprite& Tile::GetHiddenTile() { return hiddenTile; }