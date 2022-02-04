#include "../include/Shape.h"
#include "algorithm"

namespace pe {

    //Нужно не забыть, что возможно float не хватит для возведения в квадрат.

    float v_mod(sf::Vector2<float> v) {
        return v.x * v.x + v.y * v.y;
    }

    float distance_square(sf::Vector2<float> v1, sf::Vector2<float> v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }

    ///Shape

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

    ///Rectangle

    Rectangle::Rectangle(sf::Vector2<float> p1, sf::Vector2<float> p2,
                         std::vector<std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>>> layouts,
                         bool movable) {
        vertices = std::vector{p1, p2};
        t = sRectangle;
        c = sf::Color(0, 255, 0);
        this->layouts = layouts;
        this->movable = movable;
    }

    void Rectangle::collision_resolution(const std::shared_ptr<Shape> &b1) {
        if (b1->getType() == sRectangle) {
            std::shared_ptr<Rectangle> r1 = std::dynamic_pointer_cast<Rectangle>(b1);

            /// x
            std::vector<float> elems_x{vertices[0].x, vertices[1].x, r1->vertices[0].x, r1->vertices[1].x};
            float union_x = (*std::max_element(elems_x.begin(), elems_x.end())) -
                            (*std::min_element(elems_x.begin(), elems_x.end()));
            sf::Vector2<float> intersection_x = {std::max(0.0f, r1->getWidth() + getWidth() - union_x), 0};
            if (gravity_center().x < r1->gravity_center().x) intersection_x *= -1.0f;

            /// y
            std::vector<float> elems_y{vertices[0].y, vertices[1].y, r1->vertices[0].y, r1->vertices[1].y};
            float union_y = (*std::max_element(elems_y.begin(), elems_y.end())) -
                            (*std::min_element(elems_y.begin(), elems_y.end()));
            sf::Vector2<float> intersection_y = {0, std::max(0.0f, r1->getHeight() + getHeight() - union_y)};
            if (gravity_center().y < r1->gravity_center().y) intersection_y *= -1.0f;

            /// костыли
            if(v_mod(intersection_y) < v_mod(intersection_x)){
                move(intersection_y);
            }
            else{
                move(intersection_x);
            }


            /*// Вычисляем относительную скорость
            sf::Vector2<float> rv = r1->velocity - velocity;

            // Вычисляем относительную скорость относительно направления нормали
            float velAlongNormal = DotProduct( rv, normal )

            // Не выполняем вычислений, если скорости разделены
            if(velAlongNormal > 0)
                return;

            // Вычисляем упругость
            float e = min( A.restitution, B.restitution)

            // Вычисляем скаляр импульса силы
            float j = -(1 + e) * velAlongNormal
            j /= 1 / A.mass + 1 / B.mass

            // Прикладываем импульс силы
            Vec2 impulse = j * normal
            A.velocity -= 1 / A.mass * impulse
            B.velocity += 1 / B.mass * impulse*/

        }


    }

    bool Rectangle::isCollide(const std::shared_ptr<Shape> &b1) {
        if (b1->getType() == sRectangle) {
            c = sf::Color(0, 255, 0);
            std::shared_ptr<Rectangle> r1 = std::dynamic_pointer_cast<Rectangle>(b1);

            //useless
            /*///too far
            if (distance_square(gravity_center(), b1->gravity_center()) >
                std::max(getWidth(), r1->getWidth()) * std::max(getWidth(), r1->getWidth()) +
                std::max(getHeight(), r1->getHeight()) + std::max(getHeight(), r1->getHeight())) {
                return false;
            }*/

            if (vertices[0].x > r1->vertices[1].x || vertices[1].x < r1->vertices[0].x) {
                return false;
            }

            if (vertices[0].y > r1->vertices[1].y || vertices[1].y < r1->vertices[0].y) {
                return false;
            }

            c = sf::Color(255, 0, 0);
            if(movable){
                collision_resolution(b1);
            }
            return true;
        }
        return false;
    }

    void Rectangle::check_collision_existence() {
        for (auto layout: layouts) {
            for (auto object: *layout) {
                if(this != object.get()){
                    isCollide(object);
                }
            }
        }
    }

    void Rectangle::before_drawing_movement() {
        move(velocity);
        if (movable) {
            keyboard_controller();
        }
        check_collision_existence();
    }

    void Rectangle::s_draw(sf::RenderWindow &window) {
        before_drawing_movement();
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

    void Rectangle::keyboard_controller() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -1;
        }
        else{
            velocity.x = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = 1;
        }
        else{
            velocity.y = 0;
        }
    }


    float Rectangle::getWidth() {
        return vertices[1].x - vertices[0].x;
    }

    float Rectangle::getHeight() {
        return vertices[1].y - vertices[0].y;
    }

    sf::Vector2<float> Rectangle::gravity_center() {
        return {(vertices[0].x + vertices[1].x) / 2, (vertices[0].y + vertices[1].y) / 2};
    }

    float Rectangle::getMass() {
        return  getHeight() * getWidth();
    }

} // namespace pe