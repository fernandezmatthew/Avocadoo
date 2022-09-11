#pragma once
#include <iostream>
#include "textureManager.h"
#include <SFML/Graphics.hpp>

class edge {
private:
    sf::Vertex from;
    sf::Vertex to;
    sf::Vertex line[2];
    sf::Color color = sf::Color::Red;
public:
    edge();
    edge(sf::Vector2f _from, sf::Vector2f _to);
    void drawTo(sf::RenderWindow& window);
    sf::Vector2f getFrom();
    sf::Vector2f getTo();
};
