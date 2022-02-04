#include "../include/Shape.h"

namespace pe {

    Shape::Shape() = default;

    bool Shape::isCollide(const std::shared_ptr<Shape> &b1) {
        return false;
    }

    void Shape::s_draw(sf::RenderWindow &window) {
    }

    void Shape::move(sf::Vector2<float> v) {
    }

    Shape::Type Shape::getType() {
        return t;
    }

    Shape::~Shape() = default;

    Rectangle::Rectangle(sf::Vector2<float> p1, sf::Vector2<float> p2) {
        vertices = std::vector{p1, p2};
        t = sRectangle;
        c = sf::Color(0, 255, 0);
    }

    bool Rectangle::isCollide(const std::shared_ptr<Shape> &b1) {
        if (b1->getType() == sRectangle) {
            c = sf::Color(0, 255, 0);
            std::shared_ptr<Rectangle> r1 = std::dynamic_pointer_cast<Rectangle>(b1);

            if (vertices[0].x > r1->vertices[1].x || vertices[1].x<r1->vertices[0].x) {
                return false;
            }

            if (vertices[0].y > r1->vertices[1].y || vertices[1].y<r1->vertices[0].y) {
                return false;
            }

            c = sf::Color(255, 0, 0);
            return true;
        }
        return false;
    }

    void Rectangle::s_draw(sf::RenderWindow &window) {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2<float>(getWidth(), getHeight()));
        rect.setFillColor(c);
        rect.setPosition(vertices[0].x, vertices[0].y);
        window.draw(rect);
    }

    void Rectangle::move(sf::Vector2<float> v) {
        vertices[0] += v;
        vertices[1] += v;
    }

    float Rectangle::getWidth() {
        return vertices[1].x - vertices[0].x;
    }

    float Rectangle::getHeight() {
        return vertices[1].y - vertices[0].y;
    }

} // namespace pe


