#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/Shape.h"

inline float dt;

int main() {
    int w = 200, h = 200;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "SFML window");
    sf::Clock clock;
    float lastTime = 0;
    std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>> main_layout_level =
            std::make_shared<std::vector<std::shared_ptr<pe::Shape>>>();
    std::vector<std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>>> rect_layouts{main_layout_level};
    std::shared_ptr<pe::Rectangle> rect = std::make_shared<pe::Rectangle>(sf::Vector2<float>(600.0, 600.0),
                                                                          100, 100,
                                                                          rect_layouts, true);
    main_layout_level->push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(100.0, 100.0),
                                                                 200, 200,
                                                                 rect_layouts));
    main_layout_level->push_back(rect);
    main_layout_level->push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(800.0, 800.0),
                                                                 50, 50,
                                                                 rect_layouts));
    main_layout_level->push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(900.0, 800.0),
                                                                 50, 100,
                                                                 rect_layouts));
    main_layout_level->push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(1000.0, 800.0),
                                                                 50, 200,
                                                                 rect_layouts));
    main_layout_level->push_back(std::make_shared<pe::Rectangle>(sf::Vector2<float>(1100.0, 800.0),
                                                                 50, 400,
                                                                 rect_layouts));
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

        //draw
        for (auto &i: (*main_layout_level.get())) {
            i->s_draw(window);
        }
        window.display();

        //fps
        sf::Time time = clock.getElapsedTime();
        dt = time.asSeconds() - lastTime;
        double fps = 1 / (time.asSeconds() - lastTime);
        lastTime = time.asSeconds();
        window.setTitle(std::to_string(fps));
    }
    return EXIT_SUCCESS;
}