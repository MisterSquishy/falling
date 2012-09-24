#include "Block.h"


Block::Block(const Point2f &position_, const Vector2f &size_, const float &theta_) : GameObject(position_, size_, theta_)
{
	state = WAITING_TO_START_FALLING;
	coolTimer = 0.0f;
}


Block::~Block(void)
{
}

void Block::perform_logic(float time_passed, float time_step)
{
	switch(state)
	{
	case NORMAL:
		if(!collisions[BOTTOM])
			state = WAITING_TO_START_FALLING;
		break;
	case WAITING_TO_START_FALLING:
		coolTimer += time_step;
		if(coolTimer >= 0.089f)
		{
			// We're done waiting!
			state = FALLING;
			coolTimer = 0.0f;
		}
		break;
	case FALLING:
		if(collisions[BOTTOM])
			state = NORMAL;
		else m_position.y += 1;
		break;
	}
}

void Block::render() const
{
    GameObject::render("dirt_block");
}