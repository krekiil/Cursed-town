#include "Player.hpp"

using namespace sf;

Player::Player()
    : texture()
    , sprite(texture)
    , currentFrame(0)
    , animationTimer(0.f)
    , animationSpeed(0.15f)
    , speed(300.f)
    , isMoving(false)
    , frameSize(73, 133)
    , frameSpacing(50, 50)
    , framesPerRow(3)
    , direction(Down)
{
    if (!texture.loadFromFile("assets/player.png")) {
        // Keep running even if asset is missing; sprite will stay empty.
    }
    textureImage = texture.copyToImage();

    rightRowOffsets = std::vector<int>(framesPerRow, 0);

    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect({ 0, 0 }, frameSize));
    sprite.setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));
    const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sprite.setPosition({
        desktop.size.x / 2.f,
        desktop.size.y / 2.f
    });
}

void Player::handleInput(float dt) {
    Vector2f movement(0.f, 0.f);
    isMoving = false;

    if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
        movement.y -= speed * dt;
        direction = Up;
        isMoving = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
        movement.y += speed * dt;
        direction = Down;
        isMoving = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
        movement.x -= speed * dt;
        direction = Left;
        isMoving = true;
        sprite.setScale({ -1.f, 1.f });
        sprite.setOrigin(sf::Vector2f(
            static_cast<float>(frameSize.x),
            static_cast<float>(frameSize.y)
        ));
    }

    if (Keyboard::isKeyPressed(Keyboard::Key::D)) {
        movement.x += speed * dt;
        direction = Right;
        isMoving = true;
        sprite.setScale({ 1.f, 1.f });
        sprite.setOrigin(sf::Vector2f(0.f, static_cast<float>(frameSize.y)));
    }

    sprite.move(movement);
}

void Player::update(float dt) {
    if (isMoving) {
        animationTimer += dt;

        if (animationTimer >= animationSpeed) {
            animationTimer = 0.f;
            currentFrame++;

            int maxFrames = 3;
            if (currentFrame >= maxFrames)
                currentFrame = 0;
        }
    }
    else {
        animationTimer = 0.f;
        currentFrame = 0;
    }

    int row = 0;

    switch (direction) {
    case Down: row = 0; break;
    case Up: row = 1; break;
    case Left: row = 2; break;
    case Right: row = 2; break;
    }

    int top = row * (frameSize.y + frameSpacing.y);

    if (direction == Right) {
        int offset = 0;
        if (currentFrame >= 0 && currentFrame < (int)rightRowOffsets.size())
            offset = rightRowOffsets[currentFrame];

        top += offset;
    }

    sprite.setTextureRect(IntRect(
        { currentFrame * (frameSize.x + frameSpacing.x), top },
        frameSize
    ));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}