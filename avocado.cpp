#include "avocado.h"

avocado::avocado() {

}

avocado::avocado(string cityName) {
    city = cityName;

    string imagePath = "pics/avocado.png";
    string imageSelectedPath = "pics/avocadoSelected.png";
    string imageHighlightedPath = "pics/avocadoHighlighted.png";
    avo.setTexture(TextureManager::GetTexture(imagePath));
    avoSelected.setTexture(TextureManager::GetTexture(imageSelectedPath));
    avoHighlighted.setTexture(TextureManager::GetTexture(imageHighlightedPath));
    avo.setOrigin(avo.getLocalBounds().width / 2, avo.getLocalBounds().height / 2);
    avoSelected.setOrigin(avoSelected.getLocalBounds().width / 2, avoSelected.getLocalBounds().height / 2);
    avoHighlighted.setOrigin(avoHighlighted.getLocalBounds().width / 2, avoHighlighted.getLocalBounds().height / 2);
    avo.scale(.02, .02);
    avoSelected.scale(.02, .02);
    avoHighlighted.scale(.02, .02);
    avo.setPosition(100, 100);
    avoSelected.setPosition(100, 100);
    avoHighlighted.setPosition(100, 100);
    xPos = 100;
    yPos = 100;
    selected = false;
    bigger = false;
    highlighted = false;
}

void avocado::setPosition(sf::Vector2f pos) {
    avo.setPosition(pos.x, pos.y);
    avoSelected.setPosition(pos.x, pos.y);
    avoHighlighted.setPosition(pos.x, pos.y);
    xPos = pos.x;
    yPos = pos.y;
}

sf::Vector2f avocado::getPosition() {
    sf::Vector2f result = sf::Vector2f(xPos, yPos);
    return result;
}

string avocado::getName() {
    return city;
}

void avocado::setSelected(bool ToF) {
    selected = ToF;
}

bool avocado::isSelected() {
    return selected;
}

bool avocado::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    if (avo.getGlobalBounds().contains(position.x, position.y)) {
        if (!bigger) {
            avo.scale(2, 2);
            avoSelected.scale(2, 2);
            avoHighlighted.scale(2, 2);
            bigger = !bigger;
        }
        return true;
    }
    else {
        if (bigger) {
            avo.scale(.5, .5);
            avoSelected.scale(.5, .5);
            avoHighlighted.scale(.5, .5);
            bigger = !bigger;
        }
        return false;
    }
}

bool avocado::isBigger() {
    return bigger;
}

void avocado::drawTo(sf::RenderWindow& window) {
    if (selected) {
        window.draw(avoSelected);
    }
    else {
        if (highlighted) {
            window.draw(avoHighlighted);
        }
        else {
            window.draw(avo);
        }
    }

}

void avocado::clickedOn() {
    selected = !selected;
}

bool avocado::isHighlighted() {
    return highlighted;
}

void avocado::setHighlighted(bool ToF) {
    highlighted = ToF;
}

vector<sf::Vector2f> avocado::getNeighbors() {
    return neighbors;
}

void avocado::addNeighbor(sf::Vector2f pos) {
    neighbors.push_back(pos);
    //add edge later
}

