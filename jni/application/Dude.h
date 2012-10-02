#ifndef DUDE_H
#define DUDE_H

#include "GameObject.h"
#include "Block.h"

using namespace std;
using namespace Zeni;

enum DudeState {D_NORMAL, D_JUMPING, D_DEAD};

class Dude : public GameObject
{
public:
	Dude(const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_y_ = 0.0f, const float &speed_x_ = 0.0f);
	~Dude(void);
    
	//Returns if the block handler should clear the first row for him
	bool perform_logic(float current_time, float time_step, ControlState cs);
	void render() const;
    
    int left();
	int right();
    
    bool isDead();
    void revive();
	void spawn(Point2f loc);
    void reset();
    void jumpHigher();
    void bulkUp();
    void newLife();
    
protected:
    
	template <class T>
	void colliding(vector<T*>& candidates, bool resetKnowledge)
	{
		if(resetKnowledge)
        {
            for(int i = 0; i < NUM_COLLISSION_SIDES; i++) collisions[i] = false;
            wasBelow.clear();
            wasLeft.clear();
            wasRight.clear();
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
			withG[TOP] |= top() <= g->bottom() && bottom() >= g->bottom() && tb_mid >= g->left() && tb_mid <= g->right();
            // Bottom
            if(bottom() >= g->top() && top() <= g->top() && tb_mid >= g->left() && tb_mid <= g->right())
            {
                withG[BOTTOM] = true;
                wasBelow.push_back(g);
            }
			// Left
			if(left() <= g->right() && right() >= g->right() && lr_mid <= g->bottom() && lr_mid >= g->top())
            {
                withG[LEFT] = true;
                wasLeft.push_back((Block*) g);
            }
			// Right
            if(right() >= g->left() && left() <= g->left() && lr_mid <= g->bottom() && lr_mid >= g->top())
            {
                withG[RIGHT] = true;
                wasRight.push_back((Block*) g);
            }
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
    bool render_right;
	bool safeToWallJump;
    float getJumpBoost();
//    bool dead;
	int remainingLives;
	bool okToSpawn;
    bool superJumpActive;
    float superJumpActivated;
    bool strong;
    float strongActivated;
    
    float lastFlip;
    bool walkFlip;
};

#endif

