#include "Player.hpp"
using namespace sf;
using namespace std;

Player::Player() : speed(300.f)
{
    shape.setRadius(40.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({ 40.f, 40.f });
    shape.setPosition({ 960, 540 });
}

void Player::handleInput(float dt)
{
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
        shape.move({ 0, -speed * dt });

    if (Keyboard::isKeyPressed(Keyboard::Key::S))
        shape.move({ 0, speed * dt });

    if (Keyboard::isKeyPressed(Keyboard::Key::A))
        shape.move({ -speed * dt, 0 });

    if (Keyboard::isKeyPressed(Keyboard::Key::D))
        shape.move({ speed * dt, 0 });
}

void Player::draw(RenderWindow& window)
{
    window.draw(shape);
}