#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game", sf::Style::None);
    sf::CircleShape player(40.f); // создаю персонажа 
    player.setFillColor(sf::Color::Red); // задаю цвет
    player.setOrigin({ 40.f, 40.f });   
    player.setPosition({ 960, 540 }); // спавн в центре

        while (window.isOpen())
    {

            while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

window.clear();
window.draw(player);
window.display();
    }
}