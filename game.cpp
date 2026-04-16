#include "Game.hpp"
#include <SFML/Config.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <utility>

using namespace sf;

namespace {
float rectLeft(const sf::FloatRect& rect) {
#if SFML_VERSION_MAJOR >= 3
    return rect.position.x;
#else
    return rect.left;
#endif
}

float rectTop(const sf::FloatRect& rect) {
#if SFML_VERSION_MAJOR >= 3
    return rect.position.y;
#else
    return rect.top;
#endif
}

float rectWidth(const sf::FloatRect& rect) {
#if SFML_VERSION_MAJOR >= 3
    return rect.size.x;
#else
    return rect.width;
#endif
}

float rectHeight(const sf::FloatRect& rect) {
#if SFML_VERSION_MAJOR >= 3
    return rect.size.y;
#else
    return rect.height;
#endif
}

sf::Vector2f rectCenter(const sf::FloatRect& rect) {
    return {
        rectLeft(rect) + rectWidth(rect) * 0.5f,
        rectTop(rect) + rectHeight(rect) * 0.5f
    };
}

bool intersectsRect(const sf::FloatRect& lhs, const sf::FloatRect& rhs) {
#if SFML_VERSION_MAJOR >= 3
    return lhs.findIntersection(rhs).has_value();
#else
    return lhs.intersects(rhs);
#endif
}
}

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
    , view(window.getDefaultView())
    , spawnTimer(0.f)
    , spawnInterval(3.f)
    , maxZombies(200)
    , currentHealth(100.f)
    , maxHealth(100.f)
    , isHudFontLoaded(false)
    , contactDamage(5.f)
    , damageTickInterval(0.5f)
    , damageTickTimer(0.5f)
    , hitKnockbackDistance(30.f)
    , randomEngine(std::random_device{}())
    , angleDistribution(0.f, 2.f * 3.14159265f)
    , radiusDistribution(450.f, 700.f)
    , isGameOver (false)
{
    const sf::Vector2f mapSize = map.getSize();
    const sf::Vector2f mapCenter(mapSize.x * 0.5f, mapSize.y * 0.5f);

    player.setPosition(mapCenter);
    view.setCenter(mapCenter);
    zombies.reserve(maxZombies);

    healthBarBackground.setSize({ 260.f, 24.f });
    healthBarBackground.setPosition({ 20.f, 20.f });
    healthBarBackground.setFillColor(Color(40, 40, 40, 220));
    healthBarBackground.setOutlineThickness(2.f);
    healthBarBackground.setOutlineColor(Color::White);

    healthBarFill.setSize({ 252.f, 16.f });
    healthBarFill.setPosition({ 24.f, 24.f });
    healthBarFill.setFillColor(Color(220, 40, 40));

#if SFML_VERSION_MAJOR >= 3
    isHudFontLoaded = hudFont.openFromFile("assets/arial.ttf");
    if (!isHudFontLoaded) {
        isHudFontLoaded = hudFont.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
#else
    isHudFontLoaded = hudFont.loadFromFile("assets/arial.ttf");
    if (!isHudFontLoaded) {
        isHudFontLoaded = hudFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }
#endif

    if (isHudFontLoaded) {
#if SFML_VERSION_MAJOR >= 3
        healthBarText.emplace(hudFont);
#else
        healthBarText.emplace();
        healthBarText->setFont(hudFont);
#endif
        healthBarText->setCharacterSize(16);
        healthBarText->setFillColor(Color::White);
    }

    updateHealthBar();
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
#if SFML_VERSION_MAJOR >= 3
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
#else
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
#endif
}
    
void Game::update(float dt) {
    if (isGameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            currentHealth = maxHealth;
            player.setPosition(map.getSize() * 0.5f);
            zombies.clear();
            isGameOver = false;
            updateHealthBar();
        }
        return;
    }
    player.handleInput(dt);
    player.update(dt);
    sf::Vector2f pos = player.getPosition();
    sf::FloatRect bounds = player.getBounds();
    const sf::Vector2f mapSize = map.getSize();

    float halfWidth = bounds.size.x * 0.5f;
    float halfHeight = bounds.size.y * 0.5f;

    pos.x = std::clamp(pos.x, halfWidth, mapSize.x - halfWidth);
    pos.y = std::clamp(pos.y, halfHeight, mapSize.y - halfHeight);

    player.setPosition(pos);
    player.setPosition(pos);
    updateZombies(dt);
    const bool wasColliding = pushPlayerOutOfZombies();

    damageTickTimer += dt;
    if (wasColliding && damageTickTimer >= damageTickInterval) {
        currentHealth = std::max(0.f, currentHealth - contactDamage);
        damageTickTimer = 0.f;
        if (currentHealth <= 0.f) {
            isGameOver = true;
        }
        sf::Vector2f knockbackDirection(0.f, 0.f);
        const sf::FloatRect playerBounds = player.getBounds();
        const sf::Vector2f playerCenter = rectCenter(playerBounds);
        for (const Zombie& zombie : zombies) {
            const sf::FloatRect zombieBounds = zombie.getBounds();
            if (!intersectsRect(playerBounds, zombieBounds)) {
                continue;
            }

            sf::Vector2f away = playerCenter - rectCenter(zombieBounds);
            const float len = std::sqrt(away.x * away.x + away.y * away.y);
            if (len > 0.001f) {
                away /= len;
                knockbackDirection += away;
            }
        }

        const float knockLen = std::sqrt(knockbackDirection.x * knockbackDirection.x + knockbackDirection.y * knockbackDirection.y);
        if (knockLen > 0.001f) {
            knockbackDirection /= knockLen;
            player.setPosition(player.getPosition() + knockbackDirection * hitKnockbackDistance);
            pushPlayerOutOfZombies();
        }
            
        updateHealthBar();
    }

    const sf::Vector2f halfView = view.getSize() * 0.5f;
    const sf::Vector2f playerPos = player.getPosition();

    const float clampedX = std::clamp(playerPos.x, halfView.x, mapSize.x - halfView.x);
    const float clampedY = std::clamp(playerPos.y, halfView.y, mapSize.y - halfView.y);

    view.setCenter({ clampedX, clampedY });
}

void Game::updateZombies(float dt) {
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;
        spawnZombie();
    }

    const sf::Vector2f playerPos = player.getPosition();
    for (Zombie& zombie : zombies) {
        zombie.update(dt, playerPos);
    }
    resolveZombieCollisions();
}

void Game::spawnZombie() {
    if (zombies.size() >= maxZombies) {
        const sf::Vector2f playerPos = player.getPosition();
        std::size_t farthestIndex = 0;
        float farthestDistanceSq = -1.f;

        for (std::size_t i = 0; i < zombies.size(); ++i) {
            const sf::Vector2f delta = zombies[i].getPosition() - playerPos;
            const float distanceSq = delta.x * delta.x + delta.y * delta.y;
            if (distanceSq > farthestDistanceSq) {
                farthestDistanceSq = distanceSq;
                farthestIndex = i;
            }
        }

        zombies.erase(zombies.begin() + farthestIndex);
    }

    const sf::Vector2f playerPos = player.getPosition();
    const float angle = angleDistribution(randomEngine);
    const float radius = radiusDistribution(randomEngine);

    sf::Vector2f spawnPosition = {
        playerPos.x + std::cos(angle) * radius,
        playerPos.y + std::sin(angle) * radius
    };

    const sf::Vector2f mapSize = map.getSize();
    spawnPosition.x = std::clamp(spawnPosition.x, 0.f, mapSize.x);
    spawnPosition.y = std::clamp(spawnPosition.y, 0.f, mapSize.y);

    Zombie zombie(spawnPosition);
    if (zombie.isValid()) {
        zombies.push_back(std::move(zombie));
    }
}

void Game::updateHealthBar() {
    const float normalizedHealth = (maxHealth > 0.f)
        ? std::clamp(currentHealth / maxHealth, 0.f, 1.f)
        : 0.f;

    const float maxFillWidth = 252.f;
    healthBarFill.setSize({ maxFillWidth * normalizedHealth, 16.f });

    if (isHudFontLoaded && healthBarText.has_value()) {
        const int hpNow = static_cast<int>(std::round(currentHealth));
        const int hpMax = static_cast<int>(std::round(maxHealth));
        healthBarText->setString("HP: " + std::to_string(hpNow) + "/" + std::to_string(hpMax));

        const sf::FloatRect textBounds = healthBarText->getLocalBounds();
        const sf::Vector2f barPos = healthBarBackground.getPosition();
        const sf::Vector2f barSize = healthBarBackground.getSize();

#if SFML_VERSION_MAJOR >= 3
        const float textWidth = textBounds.size.x;
        const float textHeight = textBounds.size.y;
        const float textOffsetX = textBounds.position.x;
        const float textOffsetY = textBounds.position.y;
#else
        const float textWidth = textBounds.width;
        const float textHeight = textBounds.height;
        const float textOffsetX = textBounds.left;
        const float textOffsetY = textBounds.top;
#endif

        healthBarText->setPosition({
            barPos.x + (barSize.x - textWidth) * 0.5f - textOffsetX,
            barPos.y + (barSize.y - textHeight) * 0.5f - textOffsetY
        });
    }
}

bool Game::isPlayerCollidingWithAnyZombie() const {
    const sf::FloatRect playerBounds = player.getBounds();
    for (const Zombie& zombie : zombies) {
        if (intersectsRect(playerBounds, zombie.getBounds())) {
            return true;
        }
    }

    return false;
}

bool Game::pushPlayerOutOfZombies() {
    bool collided = false;

    for (int iteration = 0; iteration < 3; ++iteration) {
        bool movedThisIteration = false;
        const sf::FloatRect playerBounds = player.getBounds();
        const sf::Vector2f playerCenter = rectCenter(playerBounds);

        for (const Zombie& zombie : zombies) {
            const sf::FloatRect zombieBounds = zombie.getBounds();
            if (!intersectsRect(playerBounds, zombieBounds)) {
                continue;
            }

            collided = true;
            movedThisIteration = true;

            const sf::Vector2f zombieCenter = rectCenter(zombieBounds);
            sf::Vector2f delta = playerCenter - zombieCenter;

            const float overlapX = (rectWidth(playerBounds) * 0.5f + rectWidth(zombieBounds) * 0.5f) - std::fabs(delta.x);
            const float overlapY = (rectHeight(playerBounds) * 0.5f + rectHeight(zombieBounds) * 0.5f) - std::fabs(delta.y);

            sf::Vector2f correction(0.f, 0.f);
            if (overlapX < overlapY) {
                correction.x = (delta.x >= 0.f ? overlapX : -overlapX);
            }
            else {
                correction.y = (delta.y >= 0.f ? overlapY : -overlapY);
            }

            player.setPosition(player.getPosition() + correction);
            break;
        }

        if (!movedThisIteration) {
            break;
        }
    }

    return collided;
}
void Game::resolveZombieCollisions() {
    for (size_t i = 0; i < zombies.size(); ++i) {
        for (size_t j = i + 1; j < zombies.size(); ++j) {

            sf::FloatRect aBounds = zombies[i].getBounds();
            sf::FloatRect bBounds = zombies[j].getBounds();

            if (!intersectsRect(aBounds, bBounds)) {
                continue;
            }

            sf::Vector2f aCenter = rectCenter(aBounds);
            sf::Vector2f bCenter = rectCenter(bBounds);

            sf::Vector2f delta = aCenter - bCenter;

            float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            if (distance < 0.001f) {
                delta = { 1.f, 0.f };
                distance = 1.f;
            }

            delta /= distance;

            float overlapX = (rectWidth(aBounds) * 0.5f + rectWidth(bBounds) * 0.5f) - std::fabs(aCenter.x - bCenter.x);
            float overlapY = (rectHeight(aBounds) * 0.5f + rectHeight(bBounds) * 0.5f) - std::fabs(aCenter.y - bCenter.y);

            float push = std::min(overlapX, overlapY) * 0.5f;

            sf::Vector2f correction = delta * push;

            zombies[i].setPosition(zombies[i].getPosition() + correction);
            zombies[j].setPosition(zombies[j].getPosition() - correction);
        }
    }
}

void Game::render() {
    window.clear();
    window.setView(view);
    map.draw(window);
    for (const Zombie& zombie : zombies) {
        zombie.draw(window);
    }
    player.draw(window);

    window.setView(window.getDefaultView());
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
    if (isHudFontLoaded && healthBarText.has_value()) {
        window.draw(*healthBarText);
    }
    if (isGameOver && isHudFontLoaded) {
        sf::Text gameOverText(hudFont);
        gameOverText.setString("  GAME OVER\n Press R to respawn");
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color::Red);

        sf::FloatRect bounds = gameOverText.getLocalBounds();

#if SFML_VERSION_MAJOR >= 3
        float width = bounds.size.x;
        float height = bounds.size.y;
#else
        float width = bounds.width;
        float height = bounds.height;
#endif

        gameOverText.setPosition({
            window.getSize().x / 2.f - width / 2.f,
            window.getSize().y / 2.f - height / 2.f
            });

        window.draw(gameOverText);
    }
    window.display();
}