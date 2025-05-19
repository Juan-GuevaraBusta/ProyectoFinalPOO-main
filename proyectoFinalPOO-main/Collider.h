#ifndef COLLIDER_H
#define COLLIDER_H

#include <SFML/Graphics.hpp>
#include <math.h>

class Collider
{
public:
    Collider(sf::RectangleShape& l_body);

    void move(float x, float y);
    sf::Vector2f getPosition();
    sf::Vector2f getHalfSize();

    // pushForce means how hard is it to move the object (0 - easily, 1 - impossible)
    bool checkCollision(Collider otherCollider, sf::Vector2f& direction, float pushForce);

private:
    sf::RectangleShape& m_body;
};

#endif // COLLIDER_H