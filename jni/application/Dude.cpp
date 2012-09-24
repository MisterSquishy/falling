#include "Dude.h"


Dude::Dude(const Point2f &position_, const Vector2f &size_, const float &theta_) : GameObject(position_, size_, theta_)
{
	state = D_NORMAL;

	accel = 0.19875f;
}

Dude::~Dude(void)
{
}

void Dude::perform_logic(float time_passed, float time_step, ControlState cs)
{
	if((cs.left && !collisions[LEFT]) || (cs.right && !collisions[RIGHT])) m_position.x += ((cs.right-cs.left) * time_step * 170.0f);
	switch(state)
	{
	case D_NORMAL:
		if(!collisions[BOTTOM]) m_speed += accel;
		else m_speed = 0;
		m_position.y += m_speed * time_step * 17.0f;
		if(cs.jump && collisions[BOTTOM])
		{
			m_speed = 15.0f;
			state = D_JUMPING;
		}
		break;
	case D_JUMPING:
		m_speed -= accel;
		if(!collisions[TOP]) m_position.y -= m_speed * time_step * 17.0f;
		else m_speed = 0.0f;
		if(m_speed <= 0.0f) state = D_NORMAL;
		break;
	}
}

void Dude::render() const
{
	GameObject::render("dude");
}