#include "textBox.h"

textBox::textBox() {

}

textBox::textBox(int size, sf::Color color, bool sel) {
    backgroundBorder = sf::RectangleShape(sf::Vector2f(110, 22));
    backgroundBorder.setPosition(posX - 7.5, posY - 2);
    backgroundBorder.setFillColor(sf::Color::Black);
    background = sf::RectangleShape(sf::Vector2f(100, 20));
    background.setPosition(posX - 5, posY);
    background.setFillColor(sf::Color::White);

    box.setCharacterSize(size);
    box.setFillColor(color);
    isSelected = sel;
    userInput = "";
    if(sel) {
        box.setString("_");
    }
    else {
        box.setString("");
    }
    grayText.setCharacterSize(size);
    grayText.setFillColor(sf::Color(155, 155, 155, 155));
    grayText.setString("");
}

void textBox::inputLogic(int charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
        text << static_cast<char>(charTyped);
    }
    else if (charTyped == DELETE_KEY) {
        if (text.str().length() > 0) {
            deleteLastCharacter();
        }
    }
    else if (charTyped == ENTER_KEY) {
        userInput = text.str();
        text.str("");
        //setSelected(false);
    }
    else {
        //escape key
        setSelected(false);
    }
    //textBox will always an underscore at the end, remember this when taking input
    if (isSelected) {
        box.setString(text.str() + "_");
    }
}

void textBox::deleteLastCharacter() {
    string t = text.str();
    string newT = "";
    for (int i = 0; i < t.length() - 1; ++i) {
        newT += t[i];
    }
    text.str("");
    text << newT;

    box.setString(text.str());
}

void textBox::setFont(sf::Font& font) {
    box.setFont(font);
    grayText.setFont(font);
}

void textBox::setPosition(float x, float y) {
    sf::Vector2f point(x, y);
    sf::Vector2f pointOffset(x - 5, y);
    box.setPosition(point);
    grayText.setPosition(point);
    background.setPosition(pointOffset);
}

void textBox::setLimit(bool ToF) {
    hasLimit = ToF;
}

void textBox::setLimit(bool ToF, int lim) {
    hasLimit = ToF;
    limit = lim;
    if (ToF) {
        background.setSize(sf::Vector2f((float)limit * 9, 20));
        backgroundBorder.setSize(sf::Vector2f((float)limit * 9 + 5.5, 24));
    }
}

void textBox::setSelected(bool sel) {
    isSelected = sel;
    if (!sel) {
        string t = text.str();
        string newT = "";
        for (int i = 0; i < t.length(); ++i) {
            newT += t[i];
        }
        box.setString(newT);
    }
    else {
        box.setString(text.str() + "_");
    }
}

void textBox::setGrayText(string t) {
    grayText.setString(t);
}

string textBox::getCurrentString() {
    return text.str();
}

string textBox::getInputString() {
    return userInput;
}

sf::FloatRect textBox::getBounds() {
    return background.getGlobalBounds();
}

void textBox::drawTo(sf::RenderWindow& window) {
    window.draw(backgroundBorder);
    window.draw(background);
    window.draw(box);
    if (!isSelected) {
        if (text.str().empty()) {
            window.draw(grayText);
        }
    }
}

string textBox::typedOn(sf::Event input) {
    if (isSelected) {
        int charTyped = input.text.unicode;
        if (charTyped < 128) {
            if (hasLimit) {
                if(text.str().length() <= limit) {
                    inputLogic(charTyped);
                }
                else if (text.str().length() > limit && charTyped == DELETE_KEY) {
                    deleteLastCharacter();
                }
                else if (text.str().length() > limit && charTyped == ENTER_KEY) {
                    inputLogic(charTyped);
                }
                else if (text.str().length() > limit && charTyped == ESCAPE_KEY) {
                    inputLogic(charTyped);
                }
            }
            else {
                inputLogic(charTyped);
            }
        }
    }
    return userInput;
}

void textBox::clickedOn(sf::Event input, sf::RenderWindow& window, sf::Vector2i pos) {
    if (input.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i position = sf::Mouse::getPosition(window);
        if (getBounds().contains(position.x, position.y)) {
            setSelected(true);
        }
        else {
            setSelected(false);
        }
    }
}

