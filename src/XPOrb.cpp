#include "XPOrb.hpp"
#include <cmath>

XPOrb::XPOrb(sf::Vector2f pos)
    : collected(false)
{
    shape.setRadius(6.f);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin({ 6.f, 6.f });
    shape.setPosition(pos);
}

void XPOrb::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f dir = playerPos - shape.getPosition();
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < 200.f && dist > 1.f) {
        dir /= dist;
        shape.move(dir * 300.f * dt);
    }
}

void XPOrb::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool XPOrb::isCollected() const {
    return collected;
}

sf::FloatRect XPOrb::getBounds() const {
    return shape.getGlobalBounds();
}