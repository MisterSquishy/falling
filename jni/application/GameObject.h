#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <zenilib.h>

using namespace std;
using namespace Zeni;

enum CollissionSide { TOP, BOTTOM, LEFT, RIGHT, NUM_COLLISSION_SIDES };

struct ControlState
{
	bool left;
	bool right;
	bool jump;
	bool destroy;

	ControlState()
	{
		left = false;
		right = false;
		jump = false;
		destroy = false;
	}
};

class GameObject
{
public:

	GameObject(const Point2f &position_, const Vector2f &size_, const float &theta_ = 0.0f, const float &speed_ = 0.0f);
	virtual ~GameObject(void);

	virtual void render() const = 0; // pure virtual function call

	bool atBottom();
	int top();
	int bottom();
	int left();
	int right();
	int height();
	int width();

protected:
	template <class T>
	void colliding(vector<T*>& candidates, bool resetKnowledge)
	{
		if(resetKnowledge) for(int i = 0; i < NUM_COLLISSION_SIDES; i++) collisions[i] = false;

		for(unsigned int i = 0; i < candidates.size(); i++)
		{
			GameObject* g = candidates[i];
			if(g == NULL) continue;
				
			bool withG[NUM_COLLISSION_SIDES] = {false};
		
			// Top
			if(0 < g->bottom() - top() && g->bottom() - top() <= g->m_size.y)
				withG[TOP] |= true;
			// Bottom
			if(0 < bottom() - g->top() && bottom() - g->top() <= g->m_size.y)
				withG[BOTTOM] |= true;
			// Left
			if(0 < g->right() - left() && g->right() - left() <= g->m_size.x)
				withG[LEFT] |= true;
			// Right
			if(0 < right() - g->left() && right() - g->left() <= g->m_size.x)
				withG[RIGHT] |= true;
				
			collisions[TOP]    |= withG[TOP]    && (withG[LEFT] || withG[RIGHT] );
			collisions[BOTTOM] |= withG[BOTTOM] && (withG[LEFT] || withG[RIGHT] );
			collisions[LEFT]   |= withG[LEFT]   && (withG[TOP]  || withG[BOTTOM]);
			collisions[RIGHT]  |= withG[RIGHT]  && (withG[TOP]  || withG[BOTTOM]);
			if(atBottom()) collisions[BOTTOM] = true;
		}
	}

	void render(const String &texture, const Color &filter = Color()) const;
	Point2f m_position; // Upper left corner
	Vector2f m_size; // (width, height)
	bool collisions[NUM_COLLISSION_SIDES];
	float m_speed;
	float m_theta;

private:
	friend class BlockHandler;
};

#endif

