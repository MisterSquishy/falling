#ifndef BLOCK_H
#define BLOCK_H

#include "GameObject.h"

using namespace std;
using namespace Zeni;

enum BlockState{BL_NORMAL, BL_FALLING};

class Block : public GameObject
{
public:
	Block(const Point2f &position_, const Vector2f &size_, const float &theta, const float speed);
	~Block(void);

	void perform_logic(float time_passed, float time_step);
	void render() const;

private:
	BlockState state;
    float m_speed;
};

#endif