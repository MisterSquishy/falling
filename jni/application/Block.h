#ifndef BLOCK_H
#define BLOCK_H

#include "GameObject.h"

using namespace std;
using namespace Zeni;

enum BlockState{NORMAL, WAITING_TO_START_FALLING, FALLING};

class Block : public GameObject
{
public:
	Block(const Point2f &position_, const Vector2f &size_, const float &theta);
	~Block(void);

	void perform_logic(float time_passed, float time_step);
	void render() const;

	//void setColliding(bool colliding);

private:
	BlockState state;
	//bool collidingWithOther;
	//bool waitingToMoveSoICanLookCool;
	
	float coolTimer;
};

#endif