#ifndef PROJECT_MINESWEEPER_TEXTUREMANAGER_H
#define PROJECT_MINESWEEPER_TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

using namespace std;
using namespace sf;

class TextureManager {
    static unordered_map<string, Texture> textures; //static means it has global access. It is in memory the whole time the program runs.
    static void LoadTexture(string fileName); //e.g. "LoadTexture("Smiley");
public:
    static Texture& GetTexture(string textureName);
    static void Clear(); //Call this once at the end of main to clear all the textures.
};


#endif
