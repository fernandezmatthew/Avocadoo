#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

//from COP3503
class TextureManager {
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string filePath); //LoadTexture(string key)
public:
    static sf::Texture& GetTexture(string filePath);
    static void Clear(); //call this at the end of main
};
