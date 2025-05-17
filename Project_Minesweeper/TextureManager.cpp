#include "TextureManager.h"

//redeclare the variable, so this file knows it exists and can use it.
unordered_map<string, Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName) {
    string path = "images/";
    path += fileName + ".png";

    textures[fileName].loadFromFile(path);
    //if fileName = "smiley"
    //final path = "images/smiley.png"
}

Texture& TextureManager::GetTexture(string textureName) {

    //if the texture doesn't exist
    if (textures.find(textureName) == textures.end())
        LoadTexture(textureName);
    return textures[textureName];
}

void TextureManager::Clear() {
    textures.clear(); // Get rid of all stored objects.
}