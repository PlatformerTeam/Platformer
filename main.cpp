#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/Shape.h"

int main() {
    int w = 200, h = 200;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "SFML window");
    sf::Clock clock;
    float lastTime = 0;
    std::vector<std::shared_ptr<pe::Shape>> drawable;
    std::shared_ptr<pe::Rectangle> rect = std::make_shared<pe::Rectangle>(sf::Vector2<float>(500.0, 500.0),
                                                                          sf::Vector2<float>(700.0, 800.0));
    drawable.push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(0.0, 0.0),
                                                       sf::Vector2<float>(100.0, 100.0)));
    drawable.push_back(rect);
    drawable.push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(800.0, 800.0),
                                                       sf::Vector2<float>(950.0, 805.0)));
    while (window.isOpen()) {
        if(rect->isCollide(drawable[0])){
            std::cout << "!!!\n";
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            rect->move(sf::Vector2<float>(1, 0));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            rect->move(sf::Vector2<float>(-1, 0));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            rect->move(sf::Vector2<float>(0, -1));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            rect->move(sf::Vector2<float>(0, 1));
        }
        // Process events
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

        //draw
        for(auto &i : drawable){
            i->s_draw(window);
        }
        window.display();

        //fps
        sf::Time time = clock.getElapsedTime();
        double fps = 1 / (time.asSeconds() - lastTime);
        lastTime = time.asSeconds();
        window.setTitle(std::to_string(fps));
    }
    return EXIT_SUCCESS;
}