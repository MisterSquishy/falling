#ifndef BLOCK_H
#define BLOCK_H

#include "GameObject.h"
#include "Powerup.h"

using namespace std;
using namespace Zeni;

enum BlockState{BL_STARTUP, BL_NORMAL, BL_DESTROYED};
enum BlockType{DIRT, TITANIUM};

class Block : public GameObject
{
public:
	Block(const Point2f &position_, const Vector2f &size_, const float &theta, const float &speed_y_ = 0.0f, const float &speed_x_ = 0.0f);
    ~Block(void);
    
	void perform_logic(float current_time, float time_step);
	void render() const;
    
    BlockState getState();
    void destroy(bool dudeIsStrong);
    
    PowerupType getPowerup();
    
private:
	BlockType type;
	BlockState state;
	float broughtToLife;
};

#endif