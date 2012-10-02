#ifndef POWERUP_H
#define POWERUP_H

#include <iostream>

#include "GameObject.h"

enum PowerupType {EXTRA_LIFE, SUPER_JUMP, SUPER_STRENGTH, NUM_POWERUPS /*keep as last*/, NO_POWERUP};
enum PowerupState {P_JUMPING, P_FALLING, P_NORMAL, P_ACQUIRED, P_DEAD, P_DONE};


class Powerup : public GameObject
{
public:
    Powerup(PowerupType pt, const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_y_ = 0.0f, const float &speed_x_ = 0.0f);
	Powerup();
    
    void activate();
    
	void perform_logic(float current_time, float time_step);
	void render() const;
    
    bool considerDude(GameObject* d);
    
    template <class T>
	void colliding_p(vector<T*>& candidates, bool resetKnowledge)
	{
        if(state == P_FALLING || state == P_NORMAL)
            GameObject::colliding(candidates, resetKnowledge);
	}
    
    PowerupState getState();
    PowerupType getType();
    
private:
	PowerupType type;
    PowerupState state;
	float activated;
};

#endif
