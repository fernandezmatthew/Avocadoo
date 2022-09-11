#pragma once

//button class (mostly) made by TermSpar on Youtube.
#include <iostream>
#include "textureManager.h"
#include <SFML/Graphics.hpp>
using namespace std;

class spriteButton {
public:
    spriteButton();
    spriteButton(string fileName, string hoverFileName);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();
    void drawTo(sf::RenderWindow& window);
    bool isMouseOver(sf::RenderWindow& window);
    void scale(float x, float y);
    void clickedOn();
private:
    sf::Sprite button;
    sf::Sprite buttonHovered;
    float xPos;
    float yPos;
};
