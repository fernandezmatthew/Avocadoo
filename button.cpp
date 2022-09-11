#include "button.h"
//set up a toggle functionality
button::button() {

}

button::button(string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textColor) {
    text.setString(t);
    text.setFillColor(textColor);
    text.setCharacterSize(charSize);
    isRectangle = true;
    fillColor = bgColor;
    hoverColor = sf::Color(155, 155, 155);
    selectedColor = sf::Color::Black;
    recButton.setSize(size);
    recButton.setFillColor(bgColor);
    selected = false;
}

button::button(string t, float size, int charSize, sf::Color bgColor, sf::Color textColor) {
    text.setString(t);
    text.setFillColor(textColor);
    text.setCharacterSize(charSize);
    isRectangle = false;
    fillColor = bgColor;
    hoverColor = sf::Color(155, 155, 155);
    circleButton.setRadius(size);
    circleButton.setFillColor(bgColor);
    selected = false;
}

void button::setFont(sf::Font& font) {
    text.setFont(font);
}

void button::setBackColor(sf::Color color) {
    if (isRectangle) {
        recButton.setFillColor(color);
    }
    else {
        circleButton.setFillColor(color);
    }
}

void button::setTextColor(sf::Color color) {
    text.setFillColor(color);
}

void button::setHoverColor(sf::Color color) {
    hoverColor = color;
}

void button::setPosition(sf::Vector2f pos) {
    float xPos = 0;
    float yPos = 0;
    if (isRectangle) {
        recButton.setPosition(pos);
        xPos = pos.x + (recButton.getGlobalBounds().width / 3) - (text.getGlobalBounds().width / 2);
        yPos = pos.y + (recButton.getGlobalBounds().height / 3) - (text.getGlobalBounds().height / 2);
    }
    else {
        circleButton.setPosition(pos);
        xPos = pos.x + (circleButton.getGlobalBounds().width / 2.5) - (text.getGlobalBounds().width / 2);
        yPos = pos.y + (circleButton.getGlobalBounds().height / 2.5) - (text.getGlobalBounds().height / 2);
    }
    text.setPosition(xPos, yPos);
}

void button::drawTo(sf::RenderWindow& window) {
    if (selected) {
        recButton.setFillColor(selectedColor);
        circleButton.setFillColor(selectedColor);
    }
    else {
        if (!isMouseOver(window)) {
            recButton.setFillColor(fillColor);
            circleButton.setFillColor(fillColor);
        }
    }
    if (isRectangle) {
        window.draw(recButton);
    }
    else {
        window.draw(circleButton);
    }
    window.draw(text);
}

bool button::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    if (isRectangle) {
        if (recButton.getGlobalBounds().contains(position.x, position.y)) {
            recButton.setFillColor(hoverColor);
            return true;
        }
        else {
            recButton.setFillColor(fillColor);
            return false;
        }
    }
    else {
        if (circleButton.getGlobalBounds().contains(position.x, position.y)) {
            circleButton.setFillColor(hoverColor);
            return true;
        }
        else {
            circleButton.setFillColor(fillColor);
            return false;
        }
    }
}

void button::clickedOn() {
    selected = !selected;
}

void button::setSelected(bool ToF) {
    selected = ToF;
}

bool button::isSelected() {
    return selected;
}

void button::setSelectedColor(sf::Color color) {
    selectedColor = color;
}