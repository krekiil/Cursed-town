#include "Projectile.hpp"

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f dir)
    : velocity(dir * 600.f)
    , lifetime(2.f)
{
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin({ 5.f, 5.f });
    shape.setPosition(pos);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
    lifetime -= dt;
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Projectile::isAlive() const {
    return lifetime > 0.f;
}

sf::FloatRect Projectile::getBounds() const {
    return shape.getGlobalBounds();
}   