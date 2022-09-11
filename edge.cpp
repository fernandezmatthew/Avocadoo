#include "edge.h"

edge::edge() {
    from = sf::Vertex(sf::Vector2f(0, 0));
    to = sf::Vertex(sf::Vector2f(150, 150));
    line[0] = from;
    line[1] = to;
}

edge::edge(sf::Vector2f _from, sf::Vector2f _to) {
    from = _from;
    to = _to;
    line[0] = from;
    line[1] = to;
}
void edge::drawTo(sf::RenderWindow& window) {
    window.draw(line, 2, sf::Lines);
}

sf::Vector2f edge::getFrom() {
    return from.position;
}

sf::Vector2f edge::getTo() {
    return to.position;
}
