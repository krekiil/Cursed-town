#include "zombie.hpp"

#include <cmath>

sf::Texture Zombie::sharedTexture;
sf::Image Zombie::sharedTextureImage;
bool Zombie::textureReady = false;
void Zombie::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}
Zombie::Zombie(const sf::Vector2f& startPosition)
    : sprite(sharedTexture)
    , currentFrame(0)
    , animationTimer(0.f)
    , animationSpeed(0.15f)
    , speed(120.f)
    , isMoving(false)
    , frameSize(73, 133)
    , frameSpacing(50, 50)
    , framesPerRow(3)
    , direction(Down)
    , health(50.f)
    , maxHealth(50.f)
    , alive(true)
{
    if (!textureReady) {
        textureReady = sharedTexture.loadFromFile("assets/zombie.png");
        if (textureReady) {
            sharedTextureImage = sharedTexture.copyToImage();

        }
    }

    rightRowOffsets = std::vector<int>(framesPerRow, 0);

    sprite.setTexture(sharedTexture);

    sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
    sprite.setOrigin({ frameSize.x * 0.5f, frameSize.y * 0.5f });
    sprite.setPosition(startPosition);
}

void Zombie::update(float dt, const sf::Vector2f& targetPosition)
{
    if (!isValid())
        return;

    sf::Vector2f delta = targetPosition - sprite.getPosition();
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    isMoving = false;

    if (length < 0.001f)
    {
        currentFrame = 0;
        animationTimer = 0.f;
    }
    else
    {
        isMoving = true;

        if (std::fabs(delta.x) > std::fabs(delta.y))
        {
            direction = (delta.x < 0.f) ? Left : Right;
        }
        else
        {
            direction = (delta.y < 0.f) ? Up : Down;
        }

        delta /= length;
        sprite.move(delta * speed * dt);
    }

    if (isMoving)
    {
        animationTimer += dt;

        if (animationTimer >= animationSpeed)
        {
            animationTimer = 0.f;
            currentFrame++;

            if (currentFrame >= framesPerRow)
                currentFrame = 0;
        }
    }
    else
    {
        currentFrame = 0;
    }

    int row = 0;

    switch (direction)
    {
    case Down:  row = 0; break;
    case Up:    row = 1; break;
    case Left:  row = 2; break;
    case Right: row = 3; break;
    }

    int top = row * (frameSize.y + frameSpacing.y);

    sf::IntRect rect(
        sf::Vector2i(
            currentFrame * (frameSize.x + frameSpacing.x),
            top
        ),
        frameSize
    );

    sprite.setTextureRect(rect);
}

void Zombie::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Zombie::isValid() const {
    return textureReady && sharedTexture.getSize().x > 0 && sharedTexture.getSize().y > 0;
}

sf::Vector2f Zombie::getPosition() const {
    return sprite.getPosition();    
}

sf::FloatRect Zombie::getBounds() const {
    return sprite.getGlobalBounds();
}
void Zombie::takeDamage(float dmg) {
    health -= dmg;
    if (health <= 0.f) {
        alive = false;
    }
}

bool Zombie::isAlive() const {
    return alive;
}
