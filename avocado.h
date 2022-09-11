#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include "textureManager.h"
using namespace std;
class avocado {
private:
    sf::Sprite avo;
    sf::Sprite avoSelected;
    sf::Sprite avoHighlighted;
    vector<sf::Vector2f> neighbors;
    bool selected;
    bool highlighted;
    bool bigger;
    float xPos;
    float yPos;
    string city;
public:

    avocado();
    avocado(string cityName);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition();
    string getName();
    void setSelected(bool ToF);
    bool isSelected();
    bool isHighlighted();
    bool isBigger();
    //sf::Sprite getImage();
    //sf::Sprite getImageSelected();
    bool isMouseOver(sf::RenderWindow& window);
    void clickedOn();
    void drawTo(sf::RenderWindow& window);
    void setHighlighted(bool ToF);
    vector<sf::Vector2f> getNeighbors();
    void addNeighbor(sf::Vector2f pos);
};
