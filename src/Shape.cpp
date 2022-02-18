#include <iostream>
#include <cmath>
#include <cassert>
#include "../include/Shape.h"
#include "algorithm"

inline float dt;

namespace pe {

    //Нужно не забыть, что возможно float не хватит для возведения в квадрат.
    /// float and geometry
    float v_mod(sf::Vector2<float> v) {
        return v.x * v.x + v.y * v.y;
    }

    float distance_square(sf::Vector2<float> v1, sf::Vector2<float> v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }

    float FastInvSqrt(float x) {
        float xhalf = 0.5f * x;
        int i = *(int *) &x;  // представим биты float в виде целого числа
        i = 0x5f3759df - (i >> 1);  // какого черта здесь происходит ?
        x = *(float *) &i;
        x = x * (1.5f - (xhalf * x * x));
        return x;
    }

    float sign(float a) {
        if (a > 0) return 1;
        return -1;
    }

    float DotProduct(const sf::Vector2<float> &v1, const sf::Vector2<float> &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    // Векторное произведение двух векторов возвращает скаляр
    float CrossProduct(const sf::Vector2<float> &a, const sf::Vector2<float> &b) {
        return a.x * b.y - a.y * b.x;
    }

    // Более экзотичные (но необходимые) виды векторных произведений
    // с вектором a и скаляром s, оба возвращают вектор
    sf::Vector2<float> CrossProduct(const sf::Vector2<float> &a, float s) {
        return {s * a.y, -s * a.x};
    }

    sf::Vector2<float> CrossProduct(float s, const sf::Vector2<float> &a) {
        return {-s * a.y, s * a.x};
    }

    void normalize(sf::Vector2<float> &v, float length){
        if(v_mod(v) < 0.001) return;
        v *= length * FastInvSqrt(v_mod(v));
    }

    float Pythagore(float a, float b){
        return std::sqrt(a * a + b * b);
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

    Rectangle::Rectangle(sf::Vector2<float> mid_pos, int width, int height,
                         std::vector<std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>>> layouts,
                         bool controllerMovement, bool movable) : mid_pos(mid_pos), width(width), height(height) {
        t = sRectangle;
        c = sf::Color(0, 255, 0);
        this->layouts = layouts;
        this->controllerMovement = controllerMovement;
        this->movable = movable;
    }

    void Rectangle::collision_resolution(const std::shared_ptr<Shape> &b1) {
        if (b1->getType() == sRectangle) {
            std::shared_ptr<Rectangle> r1 = std::dynamic_pointer_cast<Rectangle>(b1);

            // Вектор из A в B
            sf::Vector2<float> v_B_A = r1->mid_pos - mid_pos;

            // Вычисление половины ширины вдоль оси x для каждого объекта
            float a_half_width = width / 2;
            float b_half_width = r1->width / 2;

            // Вычисление наложения по оси x
            float x_overlap = a_half_width + b_half_width - std::abs(v_B_A.x);
            if (x_overlap < 0) return;

            // Вычисление половины ширины вдоль оси y для каждого объекта
            float a_half_height = height / 2;
            float b_half_height = r1->height / 2;

            // Вычисление наложения по оси y
            float y_overlap = a_half_height + b_half_height - std::abs(v_B_A.y);
            if (y_overlap < 0) return;

            // Вектор нормали для текущей коллизии
            sf::Vector2<float> normal;

            // Определяем, по какой из осей проникновение наименьшее
            if (x_overlap < y_overlap) {
                // Указываем в направлении B, зная, что n указывает в направлении от A к B
                normal = sf::Vector2<float>(1, 0) * sign(v_B_A.x);
            } else {
                // Указываем в направлении B, зная, что n указывает в направлении от A к B
                normal = sf::Vector2<float>(0, 1) * sign(v_B_A.y);
            }

            // Вычисляем относительную скорость
            sf::Vector2<float> rv = r1->velocity - velocity;

            // Вычисляем относительную скорость относительно направления нормали
            float velAlongNormal = DotProduct(rv, normal);

            // Не выполняем вычислений, если скорости разделены
            if (velAlongNormal > 0)
                return;
            if(velAlongNormal > -1 && y_overlap < x_overlap) {
                if(movable){
                    this->mid_pos.y -= y_overlap;
                }
                return;
            }

            // Вычисляем упругость
            float e = std::min(restitution, r1->restitution);

            // Вычисляем скаляр импульса силы
            float j = -(1 + e) * velAlongNormal;
            j /= getInvMass() + r1->getInvMass();

            // Прикладываем импульс силы
            sf::Vector2<float> impulse = j * normal;
            velocity -= getInvMass() * impulse;
            r1->velocity += r1->getInvMass() * impulse;
            //////////////////////////////////////
            // Трение

            // Вычисляем касательный вектор
            sf::Vector2<float> tangent = rv - DotProduct( rv, normal ) * normal;
            if(v_mod(tangent) > 1)
                std::cout << "";
            normalize(tangent, 1.0f);

            // Вычисляем величину, прилагаемую вдоль вектора трения
            float jt = -DotProduct( rv, tangent);
            jt = jt / (getInvMass() + r1->getInvMass());

            // A^2 + B^2 = C^2, вычисляем C для заданных A и B
            // Используем для аппроксимации мю для заданных коэффициентов трения каждого тела
            float mu = Pythagore(staticFriction, r1->staticFriction);

            // Ограничиваем величину трения и создаём вектор импульса силы
            sf::Vector2<float> frictionImpulse;
            if(std::abs( jt ) < j * mu)
                frictionImpulse = jt * tangent;
            else {
                float d_mu = Pythagore( dynamicFriction, r1->dynamicFriction);
                frictionImpulse = -j * tangent * d_mu;
            }
            if(abs(frictionImpulse.x) > 10000000 && movable)
                std::cout << "";

            // Прикладываем
            velocity -= getInvMass() * frictionImpulse;
            r1->velocity += r1->getInvMass() * frictionImpulse;
        }
    }

    bool Rectangle::isCollide(const std::shared_ptr<Shape> &b1) {
        if (b1->getType() == sRectangle) {
            std::shared_ptr<Rectangle> r1 = std::dynamic_pointer_cast<Rectangle>(b1);

            // координаты двух крайних вершин прямоугольника
            std::vector<sf::Vector2<float>> vertices = {{mid_pos.x - width / 2, mid_pos.y - height / 2},
                                                        {mid_pos.x + width / 2, mid_pos.y + height / 2}};
            std::vector<sf::Vector2<float>> vertices1 = {{r1->mid_pos.x - r1->width / 2,
                                                                                         r1->mid_pos.y -
                                                                                         r1->height / 2},
                                                         {r1->mid_pos.x + r1->width / 2, r1->mid_pos.y +
                                                                                         r1->height / 2}};
            // если не пересекаются по обоим осям, ничего не делаем
            if (vertices[0].x > vertices1[1].x || vertices[1].x < vertices1[0].x) {
                return false;
            }
            if (vertices[0].y > vertices1[1].y || vertices[1].y < vertices1[0].y) {
                return false;
            }
            //c = sf::Color(255, 0, 0);
            collision_resolution(b1);
            return true;
        }
        return false;
    }

    void Rectangle::check_collision_existence() {
        // Перебираем все объекты с нашего слоя, проверяя пересечение с ними
        c = sf::Color(0, 255, 0);
        for (auto layout: layouts) {
            for (auto object: *layout) {
                if (this != object.get()) {
                    isCollide(object);
                }
            }
        }
    }

    void Rectangle::before_drawing_movement() {
        // применяем силы
        applyForce();
        // двигаемся
        move(velocity);
        if (controllerMovement) {
            keyboard_controller();
        }
        // проверяем наличие коллизий
        check_collision_existence();
    }

    void Rectangle::s_draw(sf::RenderWindow &window) {
        // делаем все, что нужно перед отрисовкой
        before_drawing_movement();

        // пока что рисуем квадрат, по размерам такой же, как и сам объект
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2<float>(getWidth(), getHeight()));
        rect.setFillColor(c);
        rect.setPosition(mid_pos.x - width / 2, mid_pos.y - height / 2);
        window.draw(rect);
    }


    void Rectangle::move(sf::Vector2<float> v) {
        sf::Vector2<float> test = v * dt;
        //if(abs(velocity.x) < 5) velocity.x = 0;
        //if(abs(velocity.y) < 3) velocity.y = 0;
        //assert(abs(velocity.x) < 10000);
        mid_pos += velocity * dt;
    }

    void Rectangle::applyForce() {
        resultForce = {0, 700.0f * getMass()};
        if(!controllerMovement) velocity += resultForce * getInvMass() * dt;
    }

    void Rectangle::keyboard_controller() {
        float velocity_k = 200;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = 1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -1;
        } else {
            velocity.x = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = 1;
        } else {
            velocity.y = 0;
        }
        velocity *= velocity_k;
    }


    float Rectangle::getWidth() {
        return width;
    }

    float Rectangle::getHeight() {
        return height;
    }

    sf::Vector2<float> Rectangle::gravity_center() {
        return mid_pos;
    }

    float Rectangle::getMass() {
        if(!movable) return 0;
        return getHeight() * getWidth();
    }

    float Rectangle::getInvMass() {
        if(getMass() == 0) return 0;
        return 1 / getMass();
    }



} // namespace pe