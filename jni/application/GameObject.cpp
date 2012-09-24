#include "GameObject.h"

GameObject::GameObject(const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_)
  : m_position(position_), m_size(size_), m_theta(theta_), m_speed(speed_)
{
	for(int i = 0; i < NUM_COLLISSION_SIDES; i++)
	{
		collisions[i] = false;
	}
}
 
GameObject::~GameObject()
{
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
	return m_position.y + m_size.y >= get_Window().get_height();
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