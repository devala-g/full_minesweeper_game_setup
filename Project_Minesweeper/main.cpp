#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "Tile.h"
#include "TextureManager.h"
#include "Random.h"
#include "Board.h"

using namespace sf;
using namespace std;

void AddMines(int numCol, int numRows, int numMines, Tile **tiles, Board &board, int xPos, int yPos);

void LoadTestBoards(string filename, int numCol, int numRows, Tile **tiles, Board &board);

void ResetTiles(int numCol, int numRows, Tile **tiles);

void ResetBoard(int numCol, int numRows, int mineCounter, Tile **tiles, Board &board);

void RecursiveReveal(Tile &tiles);

void PositionTiles(int numCol, int numRows, Tile **tiles);

int main() {
    /***** Window setup *****/

    //1. Load config.cfg file
    ifstream cfg("boards/config.cfg");
    //2. Get information about the board dimensions and number of mines.
    string line; //for conversions

    getline(cfg, line);
    static int numCol = stoi(line);
    int windowWidth = numCol * 32;

    getline(cfg, line);
    static int numRows = stoi(line);
    int windowHeight = (numRows * 32) + 88;

    getline(cfg, line);
    int numMines = stoi(line);

    //3. create a window based upon this information.
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Minesweeper");



    /***** Board setup *****/

    //Create a pointer array of pointer arrays:
    Tile **tiles;
    tiles = new Tile *[numCol];
    for (int i = 0; i < numCol; i++) {
        tiles[i] = new Tile[numRows];
    }

    //position the tiles which also updates each tiles info about neighboring tiles.
    PositionTiles(numCol, numRows, tiles);

    Board board(windowWidth, windowHeight, numMines);

    ResetTiles(numCol, numRows, tiles); //sets mines to = 0. //fixme
//fixme    AddMines(numCol, numRows, numMines, tiles, board); //debugger is off by default.

    //While SMFL window is open
    while (window.isOpen()) {
        /***** The following code executes only when an event happens (such as clicking the mouse): *****/
        //communicates with the operating system.
        Event event{};
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePosition2((float) Mouse::getPosition(window).x, (float) Mouse::getPosition(
                        window).y); // inserted window to get relative mouse position.

                //if user left-clicks reset button:
                if (board.happyFace.getGlobalBounds().contains(mousePosition2) && Mouse::isButtonPressed(Mouse::Left)) {
                    ResetTiles(numCol, numRows, tiles); //sets mines to = 0. //fixme
                    ResetBoard(numCol, numRows, numMines, tiles, board);
                    board.firstMove = true;
//fixme                    AddMines(numCol, numRows, numMines, tiles, board);
                }

                    //if user left clicks on debugger
                else if (board.debugger.getGlobalBounds().contains(mousePosition2) &&
                         Mouse::isButtonPressed(Mouse::Left) && !board.win && !board.lost) {
                    if (!board.debugOn) { //If debugger is off, turn it on.
                        board.debugOn = true;
                        //reveal all tiles with mines on them:
                        for (int y = 0; y < numRows; ++y) {
                            for (int x = 0; x < numCol; ++x)
                                tiles[x][y].debugOn = true;
                        }
                    } else { //If debugger is on, turn it off
                        board.debugOn = false;
                        // hide all tiles with mines on them:
                        for (int y = 0; y < numRows; ++y) {
                            for (int x = 0; x < numCol; ++x)
                                tiles[x][y].debugOn = false;
                        }
                    }
                }

                    //If user left clicks on Test 1 button
                else if (board.test1.getGlobalBounds().contains(mousePosition2) && Mouse::isButtonPressed(Mouse::Left))
                    LoadTestBoards("boards/testboard1.brd", numCol, numRows, tiles, board);

                    //If user left clicks on Test 2 button
                else if (board.test2.getGlobalBounds().contains(mousePosition2) && Mouse::isButtonPressed(Mouse::Left))
                    LoadTestBoards("boards/testboard2.brd", numCol, numRows, tiles, board);

                    //If user left clicks on Test 3 button
                else if (board.test3.getGlobalBounds().contains(mousePosition2) && Mouse::isButtonPressed(Mouse::Left))
                    LoadTestBoards("boards/testboard3.brd", numCol, numRows, tiles, board);

                    //If the user clicks on the board of tiles:
                else {
                    for (int y = 0; y < numRows; ++y) {
                        for (int x = 0; x < numCol; ++x) {
                            if (tiles[x][y].GetHiddenTile().getGlobalBounds().contains(mousePosition2) && !board.lost &&
                                !board.win) {// if clicked on the board and not in a losing or winning situation. If you lose or win, you can't click on anything.
                                //Reveal tiles with left clicks
                                if (Mouse::isButtonPressed(Mouse::Left) && !tiles[x][y].hasFlag) {
                                    tiles[x][y].isHidden = false; //sets the boolean to false indicating that the tile at that location is revealed.
                                    if (board.firstMove) {
                                        AddMines(numCol, numRows, numMines, tiles, board, x, y); //fixme
                                        board.firstMove = false; //first move was made so set to false. //fixme
                                    }
                                    if (tiles[x][y].adjacentMineCounter == 0 && !tiles[x][y].isAMine) { //if it is empty
                                        RecursiveReveal(tiles[x][y]);
                                    } else if (tiles[x][y].isAMine) {
                                        //You lose sad face. Can't click on anything else.
                                        board.lost = true;

                                        //Reveal all mines when you lose
                                        for (int b = 0; b < numRows; ++b) {
                                            for (int a = 0; a < numCol; ++a)
                                                if (tiles[a][b].isAMine)
                                                    tiles[a][b].isHidden = false;
                                        }
                                    }
                                }
                                    //Add and remove flags with Right clicks
                                else if (Mouse::isButtonPressed(Mouse::Right)) {
                                    if (tiles[x][y].isHidden) {
                                        if (tiles[x][y].hasFlag) {
                                            tiles[x][y].hasFlag = false; //If it has a flag remove it.
                                            ++board.mineCounter;
                                        } else { //if the tile does not have a flag
                                            tiles[x][y].hasFlag = true; //If it does not have a flag, add one.
                                            --board.mineCounter;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /***** The following code executes continuously: *****/

        //Checking for winning condition:
        int numTilesRemaining = numCol * numRows;
        int mineCounter = 0;
        for (int y = 0; y < numRows; ++y) {
            for (int x = 0; x < numCol; ++x) {
                if (!tiles[x][y].isHidden && !tiles[x][y].isAMine)
                    --numTilesRemaining;
                else if (tiles[x][y].isAMine /*&& tiles[x][y].isHidden*/)
                    ++mineCounter;
            }
        }

        if (numTilesRemaining == mineCounter)
            board.win = true;

        // 1. clear the screen. Get rid of anything that was previously drawn
        window.clear();

        //2. Draw things to the screen

        //draw tiles
        for (int y = 0; y < numRows; ++y) { //print along the y-axis
            for (int x = 0; x < numCol; ++x) {
                tiles[x][y].Draw(window, board.win);
            }
        }

        board.Draw(window);



        //3. Display everything we drew:
        window.display();
    }

    TextureManager::Clear();

    delete[] tiles;
    return 0;
}

void AddMines(int numCol, int numRows, int numMines, Tile **tiles, Board &board, int xPos, int yPos) {

    //1. lay the mines:
    int mineCounter = 0;

    while (mineCounter < numMines) {
        int x = Random::Int(0, numCol - 1);
        int y = Random::Int(0, numRows - 1);
        if (!tiles[x][y].isAMine && (x != xPos || y != yPos)) {
            tiles[x][y].isAMine = true;
            ++mineCounter;
        }
    }

    //2. Increment Adjacent Mine Counters
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCol; ++x) {
            for (int i = 0; i < tiles[x][y].adjacentTilesVec.size(); ++i) {
                if (tiles[x][y].adjacentTilesVec[i]->isAMine)
                    ++tiles[x][y].adjacentMineCounter;
            }
        }
    }
    //3. Reset the board
    ResetBoard(numCol, numRows, mineCounter, tiles, board);
}

void LoadTestBoards(string filename, int numCol, int numRows, Tile **tiles, Board &board) {
    ResetTiles(numCol, numRows, tiles);

    ifstream file(filename);
    string line;

    int mineCounter = 0;

    //1. lay the mines:
    for (int y = 0; y < numRows; ++y) {
        getline(file, line);
        for (int x = 0; x < numCol; ++x) {
            if (line[x] == '1') {
                tiles[x][y].isAMine = true;
                ++mineCounter;
            } else
                tiles[x][y].isAMine = false;
        }
    }

    //2. Increment Adjacent Mine Counters
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCol; ++x) {
            for (int i = 0; i < tiles[x][y].adjacentTilesVec.size(); ++i) {
                if (tiles[x][y].adjacentTilesVec[i]->isAMine)
                    ++tiles[x][y].adjacentMineCounter;
            }
        }
    }

    ResetBoard(numCol, numRows, mineCounter, tiles, board);
}

void ResetTiles(int numCol, int numRows, Tile **tiles) {
    //clear the tiles of mines and resets variables to default
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCol; ++x) {
            tiles[x][y].isAMine = false;
            tiles[x][y].isHidden = true;
            tiles[x][y].adjacentMineCounter = 0;
            tiles[x][y].isChecked = false;
            tiles[x][y].hasFlag = false;
        }
    }
}

void ResetBoard(int numCol, int numRows, int mineCounter, Tile **tiles, Board &board) {
    board.mineCounter = mineCounter;
    board.lost = false;
    board.win = false;

    if (board.debugOn) {
        for (int y = 0; y < numRows; ++y) {
            for (int x = 0; x < numCol; ++x)
                tiles[x][y].debugOn = true;
        }
    }
}

void RecursiveReveal(Tile &tile) { //Assumes current tile is empty
    for (int i = 0; i < tile.adjacentTilesVec.size(); ++i) {
        if (!tile.adjacentTilesVec[i]->hasFlag) {
            tile.adjacentTilesVec[i]->isHidden = false;
            if (tile.adjacentTilesVec[i]->adjacentMineCounter == 0 &&
                !tile.adjacentTilesVec[i]->isChecked) {//if it has zero neighboring mines and has not been checked by recursion
                tile.adjacentTilesVec[i]->isChecked = true;
                RecursiveReveal(*tile.adjacentTilesVec[i]);
            }
        }
    }
}

void PositionTiles(int numCol, int numRows, Tile **tiles) {
    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCol; ++x) {
            tiles[x][y].SetPosition((float) x * 32, (float) y * 32);
            //Setting adjecent tiles without going out of bounds in the array.
            if ((x != 0) && (y != 0))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x - 1][y - 1]); //top left
            if (y != 0)
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x][y - 1]); //top middle
            if ((x != (numCol - 1)) && (y != 0))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x + 1][y - 1]); // top right
            if (x != 0)
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x - 1][y]); //middle left
            if (x != (numCol - 1))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x + 1][y]); //middle right
            if ((x != 0) && (y != (numRows - 1)))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x - 1][y + 1]); //bottom left
            if (y != (numRows - 1))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x][y + 1]); //bottom middle
            if ((x != (numCol - 1)) && (y != (numRows - 1)))
                tiles[x][y].adjacentTilesVec.push_back(&tiles[x + 1][y + 1]); //bottom right
        }
    }
}