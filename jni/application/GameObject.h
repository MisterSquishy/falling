#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <zenilib.h>
#include "Constants.h"
#include "zeni_audio/Zeni/Sound_Source_Pool.h"

using namespace std;
using namespace Zeni;

enum CollissionSide { TOP, BOTTOM, LEFT, RIGHT, NUM_COLLISSION_SIDES };

struct ControlState
{
	bool left;
	bool right;
	bool jump;
	bool destroy;
	bool retry;
	bool main_menu;
    
	ControlState()
	{
		left = false;
		right = false;
		jump = false;
		destroy = false;
		retry = false;
		main_menu = false;
	}
};

class GameObject
{
public:
	GameObject(const Point2f &position_, const Vector2f &size_, const float &theta_ = 0.0f, const float &y_speed_ = 0.0f, const float &x_speed_ = 0.0f, const float &y_accel_ = 0.0f, const float &x_accel_ = 0.0f);
	virtual ~GameObject(void);
    
	virtual void render() const = 0; // pure virtual function call
    
	virtual bool atBottom();
    virtual bool atRight();
    virtual bool atLeft();
	virtual int top();
	virtual int bottom();
    virtual int left();
	virtual int right();
	virtual int height();
	virtual int width();
    
protected:
	template <class T>
	void colliding(vector<T*>& candidates, bool resetKnowledge)
	{
		if(resetKnowledge)
        {
            wasBelow.clear();
            for(int i = 0; i < NUM_COLLISSION_SIDES; i++) collisions[i] = false;
        }
        
        if(atBottom()) collisions[BOTTOM] |= true;
        if(atRight()) collisions[RIGHT] |= true;
        if(atLeft()) collisions[LEFT] = true;
        
		for(unsigned int i = 0; i < candidates.size(); i++)
		{
			GameObject* g = candidates[i];
			if(g == NULL || g == this) continue;
            
			bool withG[NUM_COLLISSION_SIDES] = {false};
            collidingWith(g, withG);
            
			collisions[TOP]    |= withG[TOP]    && (withG[LEFT] || withG[RIGHT] );
			collisions[BOTTOM] |= withG[BOTTOM] && (withG[LEFT] || withG[RIGHT] );
			collisions[LEFT]   |= withG[LEFT]   && (withG[TOP]  || withG[BOTTOM]);
			collisions[RIGHT]  |= withG[RIGHT]  && (withG[TOP]  || withG[BOTTOM]);
		}
	}
        
    template <class T>
    void collidingWith(T* g, bool* collisions)
	{
        if(g != NULL)
        {
            if(right() != g->left() && left() != g->right())
            {
                // Top
                if(0 < g->bottom() - top() && g->bottom() - top() <= g->m_size.y)
                    collisions[TOP] |= true;
                // Bottom
                if(0 < bottom() - g->top() && bottom() - g->top() <= g->m_size.y)
                {
                    wasBelow.push_back(g);
                    collisions[BOTTOM] |= true;
                }
            }
            // Left
            if(0 < g->right() - left() && g->right() - left() <= g->m_size.x)
            {
                collisions[LEFT] |= true;
            }
            // Right
            if(0 < right() - g->left() && right() - g->left() <= g->m_size.x)
            {
                collisions[RIGHT] |= true;
            }
        }
	}
    
    void moveDown(float time_step);
	void moveSide(float time_step, int dir);
    
	void render(const String &texture, const bool flip = false, const Color &filter = Color()) const;
	Point2f m_position; // Upper left corner
	Vector2f m_size; // (width, height)
	bool collisions[NUM_COLLISSION_SIDES];
	float m_speed_x;
	float m_speed_y;
	float m_theta;
    float m_accel_y;
	float m_accel_x;
    vector<GameObject*> wasBelow;
    vector<GameObject*> wasLeft; // currently not used by this implementation in collide, only used by the dude
    vector<GameObject*> wasRight; // currently not used by this implementation in collide, only used by the dude
private:
	friend class BlockHandler;
	int endOfWorldOnRight;
};

#endif

