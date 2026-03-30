#include "Player.hpp"

using namespace sf;

Player::Player():
    currentFrame(0),
    animationTimer(0.f),
    animationSpeed(0.15f),
    speed(300.f),
    isMoving(false),
    frameSize(43, 58)
{
    texture.loadFromFile("assets/player.png");

    sprite.setTextureRect(IntRect({ 0, 0 }, frameSize));

    sprite.setOrigin(frameSize.x / 2.f, frameSize.y / 2.f);

    sprite.setPosition({ 960, 540 });
}
void Player::handleInput(float dt)
{
    Vector2f movement(0.f, 0.f);
    isMoving = false;

    if (Keyboard::isKeyPressed(Keyboard::Key::W))
    {
        movement.y -= speed * dt;
        isMoving = true;
    }

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