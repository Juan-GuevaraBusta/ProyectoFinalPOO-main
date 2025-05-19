#include "Collider.h"
#include <cmath>

Collider::Collider(sf::RectangleShape& l_body) : m_body(l_body)
{
}

void Collider::move(float x, float y)
{
    m_body.move(x, y);
}

sf::Vector2f Collider::getPosition()
{
    return m_body.getPosition();
}

sf::Vector2f Collider::getHalfSize()
{
    return m_body.getSize() / 2.0f;
}

bool Collider::checkCollision(Collider otherCollider, sf::Vector2f& direction, float pushForce)
{
    sf::Vector2f other_position = otherCollider.getPosition();
    sf::Vector2f other_halfsize = otherCollider.getHalfSize();
    sf::Vector2f this_position = getPosition();
    sf::Vector2f this_halfsize = getHalfSize();

    float delta_x = other_position.x - this_position.x;
    float delta_y = other_position.y - this_position.y;
    float intersect_x = fabs(delta_x) - (other_halfsize.x + this_halfsize.x);
    float intersect_y = fabs(delta_y) - (other_halfsize.y + this_halfsize.y);

    if (intersect_x < 0.0f && intersect_y < 0.0f)
    {
        pushForce = std::min(std::max(pushForce, 0.0f), 1.0f);

        if (intersect_x > intersect_y)
        {
            if (delta_x > 0.0f)
            {
                move(intersect_x * (1.0f - pushForce), 0.0f);
                otherCollider.move(-intersect_x * pushForce, 0.0f);
                direction.x = 1.0f;
                direction.y = 0.0f;
            }
            else
            {
                move(-intersect_x * (1.0f - pushForce), 0.0f);
                otherCollider.move(intersect_x * pushForce, 0.0f);
                direction.x = -1.0f;
                direction.y = 0.0f;
            }
        }
        else
        {
            if (delta_y > 0.0f)
            {
                move(0.0f, intersect_y * (1.0f - pushForce));
                otherCollider.move(0.0f, -intersect_y * pushForce);
                direction.x = 0.0f;
                direction.y = 1.0f;
            }
            else
            {
                move(0.0f, -intersect_y * (1.0f - pushForce));
                otherCollider.move(0.0f, intersect_y * pushForce);
                direction.x = 0.0f;
                direction.y = -1.0f;
            }
        }
        return true;
    }
    return false;
}