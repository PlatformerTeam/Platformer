#ifndef UNTITLED2_SHAPE_H
#define UNTITLED2_SHAPE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

namespace pe {

    struct Shape {
        enum Type {
            sRectangle
        };

        Shape();

        virtual bool isCollide(const std::shared_ptr<Shape> &b1) = 0;

        virtual void s_draw(sf::RenderWindow &window);

        virtual void move(sf::Vector2<float> v);

        Type getType();

        virtual ~Shape();

    protected:
        Type t;
    };

    struct Rectangle : Shape {
        sf::Color c;

        Rectangle(sf::Vector2<float> p1, sf::Vector2<float> p2);

        bool isCollide(const std::shared_ptr<Shape> &b1) override;

        void s_draw(sf::RenderWindow &window) override;

        void move(sf::Vector2<float> v) override;

        float getWidth();

        float getHeight();

    private:
        // *----
        // |   |
        // ----*
        std::vector<sf::Vector2<float>> vertices;
    };


} // namespace pe


#endif//UNTITLED2_SHAPE_H
