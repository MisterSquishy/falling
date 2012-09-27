#ifndef DUDE_H
#define DUDE_H

#include "GameObject.h"

using namespace std;
using namespace Zeni;

enum DudeState {D_NORMAL, D_JUMPING};

class Dude : public GameObject
{
public:
	Dude(const Point2f &position_, const Vector2f &size_, const float &theta_);
	~Dude(void);

	void perform_logic(float time, float time_passed, float time_step, ControlState cs);
	void render() const;
    
    int left();
	int right();

protected:

	template <class T>
	void colliding(vector<T*>& candidates, bool resetKnowledge)
	{
		if(resetKnowledge)
        {
            for(int i = 0; i < NUM_COLLISSION_SIDES; i++) collisions[i] = false;
            wasBelow.clear();
        }
        if(atBottom()) collisions[BOTTOM] |= true;
        if(atRight()) collisions[RIGHT] |= true;
        if(atLeft()) collisions[LEFT] |= true;
        
		for(unsigned int i = 0; i < candidates.size(); i++)
		{
			GameObject* g = candidates[i];
			if(g == NULL) continue;
				
			bool withG[NUM_COLLISSION_SIDES] = {false};
			
			int tb_mid = (left() + right())/2;
			int lr_mid = (top() + bottom())/2;
			// Top
			withG[TOP] = top() <= g->bottom() && bottom() >= g->bottom() && tb_mid >= g->left() && tb_mid <= g->right();
			// Bottom
            if(bottom() >= g->top() && top() <= g->top() && tb_mid >= g->left() && tb_mid <= g->right())
            {
                withG[BOTTOM] = true;
                wasBelow.push_back(g);
            }
			// Left
			withG[LEFT] = left() <= g->right() && right() >= g->right() && lr_mid <= g->bottom() && lr_mid >= g->top();
			// Right
			withG[RIGHT] = right() >= g->left() && left() <= g->left() && lr_mid <= g->bottom() && lr_mid >= g->top();
				
			collisions[TOP]    |= withG[TOP];
			collisions[BOTTOM] |= withG[BOTTOM];
			collisions[LEFT]   |= withG[LEFT];
			collisions[RIGHT]  |= withG[RIGHT];
		}
	}

private:
	DudeState state;
	friend class BlockHandler;
    int wallJumpCount;
    float lastWallJump;
    int walledRight;
};

#endif

