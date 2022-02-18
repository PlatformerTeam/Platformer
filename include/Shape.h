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

        virtual sf::Vector2<float> gravity_center() = 0;

        virtual void collision_resolution(const std::shared_ptr<Shape> &b1) = 0;

        virtual bool isCollide(const std::shared_ptr<Shape> &b1) = 0;

        virtual void check_collision_existence() = 0;

        virtual void before_drawing_movement() = 0;

        virtual void s_draw(sf::RenderWindow &window) = 0;

        virtual void move(sf::Vector2<float> v) = 0;


        Type getType();

        virtual ~Shape();

    protected:
        Type t;
        std::vector<std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>>> layouts;
        bool movable = true;
    };

    struct Rectangle : Shape {
        sf::Color c;

        Rectangle(sf::Vector2<float> mid_pos, int width, int height, std::vector<std::shared_ptr<std::vector<std::shared_ptr<pe::Shape>>>> layouts,
                  bool movable = false);

        sf::Vector2<float> gravity_center() override;

        void collision_resolution(const std::shared_ptr<Shape> &b1) override;

        bool isCollide(const std::shared_ptr<Shape> &b1) override;

        void check_collision_existence() override;

        void s_draw(sf::RenderWindow &window) override;

        void before_drawing_movement() override;

        void move(sf::Vector2<float> v) override;

        void keyboard_controller();

        float getWidth();

        float getHeight();

        float getMass();

    private:
        sf::Clock clock;
        float lastTime;
        // *----
        // |   |
        // ----*
        sf::Vector2<float> mid_pos;
        float width;
        float height;
        float restitution = 1;
        sf::Vector2<float> velocity;

        // Угловые компоненты
        float orientation; // радианы
        float angularVelocity;
        float torque; // крутящий момент
    };


} // namespace pe


#endif//UNTITLED2_SHAPE_H