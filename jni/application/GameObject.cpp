#include "GameObject.h"

GameObject::GameObject(const Point2f &position_, const Vector2f &size_, const float &theta_, const float &y_speed_, const float &x_speed_, const float &y_accel_, const float &x_accel_)
: m_position(position_), m_size(size_), m_theta(theta_), m_speed_y(y_speed_), m_speed_x(x_speed_), m_accel_y(y_accel_), m_accel_x(x_accel_)
{
	for(int i = 0; i < NUM_COLLISSION_SIDES; i++)
	{
		collisions[i] = false;
	}

	endOfWorldOnRight = get_Window().get_width();
}
 
GameObject::~GameObject()
{
}

void GameObject::moveDown(float time_step)
{
    if(collisions[BOTTOM])
    {
        m_speed_y = DORMANT;
        if(atBottom()) m_position.y = get_Window().get_height() - m_size.y;
        else
        {
            for(unsigned int i = 0; i < wasBelow.size(); i++)
            {
                GameObject* obj = wasBelow[i];
                if(obj != NULL && bottom() > obj->top())
                    m_position.y = obj->top() - m_size.y + 1;
            }
        }
    }
    else
    {
        m_speed_y += m_accel_y;
        m_position.y += (time_step * m_speed_y * SCALE);
    }
}

void GameObject::moveSide(float time_step, int dir)
{
	m_accel_x = collisions[BOTTOM] ? COF : IN_AIR_COF;

	switch(dir)
	{
	case 0: // No desired motion
		if(m_speed_x > DORMANT)
		{
			m_speed_x -= m_accel_x;
			if(m_speed_x < DORMANT) m_speed_x = DORMANT;
		}
		else if(m_speed_x < DORMANT)
		{
			m_speed_x += m_accel_x;
			if(m_speed_x > DORMANT) m_speed_x = DORMANT;
		}
		break;
	case 1:
	case -1:
		m_accel_x += MOVE_ACC_INC;

		m_speed_x *= dir;

		CollissionSide side = dir == 1 ? RIGHT : LEFT;

		if(collisions[side] && m_speed_x > DORMANT)
			m_speed_x = DORMANT;
		else
			m_speed_x = dir * min(m_speed_x+m_accel_x, MAX_X_SPEED);

		break;
	}
	
	if((m_speed_x < DORMANT && !collisions[LEFT]) || (m_speed_x > DORMANT && !collisions[RIGHT]))
	{
		m_position.x += m_speed_x * time_step * SCALE;
	}
	else if((m_speed_x < DORMANT && collisions[LEFT]) || (m_speed_x > DORMANT && collisions[RIGHT]))
	{
		m_speed_x = DORMANT;
	}
}

void GameObject::render(const String &texture, const Color &filter) const {
    // Use a helper defined in Zeni/EZ2D.h
    render_image(
      texture, // which texture to use
      m_position, // upper-left corner
      m_position + m_size, // lower-right corner
      m_theta, // rotation in radians
      1.0f, // scaling factor
      m_position + 0.5f * m_size, // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      filter); // what Color to "paint" the texture
}

// Returns, as part of sides, what portion of this g is colliding with.
/*template <class T>
void GameObject::colliding(vector<T*>& candidates, bool resetKnowledge)
{	
	
}*/

bool GameObject::atBottom()
{
	return bottom() >= get_Window().get_height();
}

bool GameObject::atRight()
{
	return right() >= endOfWorldOnRight || right() >= get_Window().get_width();
}

bool GameObject::atLeft()
{
	return left() <= 0;
}

int GameObject::top()
{
	return m_position.y;
}

int GameObject::bottom()
{
	return m_position.y + m_size.y;
}

int GameObject::left()
{
	return m_position.x;
}

int GameObject::right()
{
	return m_position.x + m_size.x;
}

int GameObject::height()
{
	return m_size.y;
}

int GameObject::width()
{
	return m_size.x;
}