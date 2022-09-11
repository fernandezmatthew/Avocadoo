#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace std;
//textBox class (mostly) made by TermSpar on Youtube.
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class textBox {
public:
    textBox();
    textBox(int size, sf::Color color, bool sel);
    void setFont(sf::Font& font);
    void setPosition(float x, float y);
    void setLimit(bool ToF);
    void setLimit(bool ToF, int lim);
    void setSelected(bool sel);
    void setGrayText(string t);
    string getCurrentString();
    string getInputString();
    void drawTo(sf::RenderWindow& window);
    string typedOn(sf::Event input);
    sf::FloatRect getBounds();
    void clickedOn(sf::Event input, sf::RenderWindow& window, sf::Vector2i pos);
private:
    sf::Text box;
    sf::Text grayText;
    ostringstream text;
    bool isSelected = false;
    bool hasLimit = false;
    int limit;
    float posX = 30;
    float posY = 30;
    sf::RectangleShape background;
    sf::RectangleShape backgroundBorder;
    string userInput;

    void inputLogic(int charTyped);
    void deleteLastCharacter();
};

