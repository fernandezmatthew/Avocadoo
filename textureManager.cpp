#include "textureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures; //re-declares this static variable

void TextureManager::LoadTexture(string filePath) {
    textures[filePath].loadFromFile(filePath);
}

sf::Texture& TextureManager::GetTexture(string filePath) {
    if(textures.find(filePath) == textures.end()) {
        LoadTexture(filePath);
    }
    return textures[filePath];
}

void TextureManager::Clear() {
    textures.clear();
}

