#pragma once

//button class (mostly) made by TermSpar on Youtube.
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

class button {
public:
    button();
    button(string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor);
    button(string t, float size, int charSize, sf::Color bgColor, sf::Color textColor);
    void setFont(sf::Font& font);
    void setHoverColor(sf::Color color);
    void setBackColor(sf::Color color);
    void setSelectedColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setPosition(sf::Vector2f pos);
    void drawTo(sf::RenderWindow& window);
    bool isMouseOver(sf::RenderWindow& window);
    void clickedOn();
    void setSelected(bool ToF);
    bool isSelected();
private:
    sf::RectangleShape recButton;
    sf::CircleShape circleButton;
    sf::Text text;
    bool isRectangle;
    sf::Color fillColor;
    sf::Color hoverColor;
    sf::Color selectedColor;
    bool selected;
};
