#include "Player.hpp"

Player::Player() : speed(300.f)
{
    shape.setRadius(40.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({ 40.f, 40.f });
    shape.setPosition({ 960, 540 });
}

void Player::handleInput(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        shape.move({ 0, -speed * dt });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        shape.move({ 0, speed * dt });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        shape.move({ -speed * dt, 0 });

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        shape.move({ speed * dt, 0 });
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}