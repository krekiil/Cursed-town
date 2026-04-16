#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Zombie {
public:
    Zombie(const sf::Vector2f& startPosition);

    void update(float dt, const sf::Vector2f& targetPosition);
    void draw(sf::RenderWindow& window) const;
    bool isValid() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

private:
    static sf::Texture sharedTexture;
    static sf::Image sharedTextureImage;
    static bool textureReady;

    sf::Sprite sprite;

    int currentFrame;
    float animationTimer;
    float animationSpeed;
    float speed;
    bool isMoving;

    sf::Vector2i frameSize;
    sf::Vector2i frameSpacing;
    int framesPerRow;
    std::vector<int> rightRowOffsets;

    enum Direction { Down, Up, Left, Right };
    Direction direction;
};
