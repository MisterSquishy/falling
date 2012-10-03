#include "Block.h"


Block::Block(const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_y_, const float &speed_x_) :
GameObject(position_, size_, theta_, speed_y_ /* speed_y */, speed_x_ /*speed_x*/, Y_ACCELERATION /* accel_y */)//, UNDEFINED /* accel_x*/ )
{
    int tn = rand() % 10;
    type = (tn <= 3) ? DIRT : TITANIUM;
	
    state = BL_STARTUP;
	broughtToLife = UNDEFINED;
}


Block::~Block(void)
{
}

BlockState Block::getState()
{
    return state;
}

PowerupType Block::getPowerup()
{
    bool powerup = false;
    
    if(type == TITANIUM)
        powerup = ((rand()%100) < 10);
    else if(type == DIRT)
        powerup = ((rand()%100) < 25);
        
    return powerup ? (PowerupType) (rand() % NUM_POWERUPS) : NO_POWERUP;
}

void Block::destroy(bool dudeIsStrong)
{
    if(dudeIsStrong || type != TITANIUM)
        state = BL_DESTROYED;
}

void Block::perform_logic(float current_time, float time_step)
{aw
    switch(state)
    {
        case BL_STARTUP:
            if(broughtToLife == UNDEFINED)
                broughtToLife = current_time;
            else if(current_time-broughtToLife >= BLOCK_PAUSE_BEFORE_FALL)
                state = BL_NORMAL;
            
            if(m_position.y <= -(GRID_SIZE/2)){
                m_position.y += 4;
            }
            break;
        case BL_NORMAL:
            bool playSound = m_speed_y != 0;
            moveDown(time_step);
            if(playSound && m_speed_y == 0)
                play_sound("pop");
            break;
    }
}

void Block::render() const
{
    switch(type)
    {
        case DIRT:
            GameObject::render("dirt_block");
            break;
        case TITANIUM:
            GameObject::render("titanium_block");
            break;
    }
}