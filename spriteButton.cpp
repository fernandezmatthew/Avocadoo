#include "spriteButton.h"

spriteButton::spriteButton() {

}

spriteButton::spriteButton(string fileName, string hoverFileName) {
    button.setTexture(TextureManager::GetTexture(fileName));
    buttonHovered.setTexture(TextureManager::GetTexture(hoverFileName));
    xPos = 0;
    yPos = 0;
}

void spriteButton::setPosition(sf::Vector2f pos) {
    button.setPosition(pos);
    buttonHovered.setPosition(pos);
}

sf::Vector2f spriteButton::getPosition() {
    sf::Vector2f result = sf::Vector2f(xPos, yPos);
    return result;
}

void spriteButton::drawTo(sf::RenderWindow& window) {
    if (isMouseOver(window)) {
        window.draw(buttonHovered);
    }
    else {
        window.draw(button);
    }
}

bool spriteButton::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    if (button.getGlobalBounds().contains(position.x, position.y)) {
        return true;
    }
    else {
        return false;
    }
}

void spriteButton::clickedOn() {
    //don't think I need anything here actually
}

void spriteButton::scale(float x, float y) {
    button.scale(x, y);
    buttonHovered.scale(x, y);
}