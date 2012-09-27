#include "Block.h"


Block::Block(const Point2f &position_, const Vector2f &size_, const float &theta_, const float speed) :
GameObject(position_, size_, theta_, 0.0f /* speed */, 0.19875f /* accel */)
{
	state = BL_FALLING;
    m_speed = speed;
}


Block::~Block(void)
{
}

void Block::perform_logic(float time_passed, float time_step)
{
	switch(state)
	{
        case BL_NORMAL:
            if(!collisions[BOTTOM]) state = BL_FALLING;
            break;
        case BL_FALLING:
            moveDown(time_step);

            if(collisions[BOTTOM])
                state = BL_NORMAL;
            break;
	}
}

void Block::render() const
{
    GameObject::render("dirt_block");
}