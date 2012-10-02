#include "Powerup.h"

Powerup::Powerup(PowerupType pt, const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_y_, const float &speed_x_) :
GameObject(position_, size_, theta_, speed_y_ /* speed_y */, speed_x_ /* speed_x */, Y_ACCELERATION /* accel_y */)//, UNDEFINED /* accel_x */)
{
    type = pt;
    state = P_JUMPING;
    activated = UNDEFINED;
}

void Powerup::perform_logic(float current_time, float time_step)
{
    switch(state)
    {
        case P_FALLING:
            moveDown(time_step);
            if(collisions[BOTTOM])
                state = P_NORMAL;
            break;
        case P_NORMAL:
            if(collisions[TOP]) state = P_DEAD;
            if(!collisions[BOTTOM]) state = P_FALLING;
            else m_speed_y = DORMANT;
            break;
        case P_JUMPING:
            m_speed_y -= m_accel_y;
            
            if(m_speed_y <= DORMANT) // If we've reached the top of our jump
            {
                m_speed_y = DORMANT;
                state = P_FALLING;
            }
            else
            {
				m_position.y -= m_speed_y * SCALE * time_step;
                if(m_position.y <= GRID_SIZE) // If we've gone too far
                {
                    m_position.y = GRID_SIZE;
                    m_speed_y = DORMANT;
                    state = P_FALLING;
                }
            }
            break;
        case P_ACQUIRED:
            if(activated == UNDEFINED) activated = current_time;
            
            switch(type)
        {
            case SUPER_JUMP:
            if(current_time-activated >= SUPER_JUMP_ACTIVE_TIME)
                state = P_DONE;
            break;
            case EXTRA_LIFE:
                state = P_DONE;
                break;
            case SUPER_STRENGTH:
                if(current_time-activated >= SUPER_STRENGTH_ACTIVE_TIME)
                    state = P_DONE;
                break;
        }
            break;
        case P_DEAD:
            state = P_DONE;
            break;
    }
}

PowerupState Powerup::getState()
{
    return state;
}

PowerupType Powerup::getType()
{
    return type;
}

bool Powerup::considerDude(GameObject* d)
{
    bool withG[NUM_COLLISSION_SIDES] = {false};
    collidingWith(d, withG);
    bool colliding = false;
    
    colliding |= withG[TOP]    && (withG[LEFT] || withG[RIGHT] );
    colliding |= withG[BOTTOM] && (withG[LEFT] || withG[RIGHT] );
    colliding |= withG[LEFT]   && (withG[TOP]  || withG[BOTTOM]);
    colliding |= withG[RIGHT]  && (withG[TOP]  || withG[BOTTOM]);
    colliding = colliding && (state == P_NORMAL || state == P_FALLING);
    if(colliding)
    {
        state = P_ACQUIRED;
        m_size *= 2;
        m_position = Point2f(get_Window().get_width()-m_size.x-10.0f, 10.0f + (type == SUPER_STRENGTH ? m_size.y+10.0f : 0.0f));
        activated = UNDEFINED;
        if(type > EXTRA_LIFE) play_sound("power_up_active");
    }
    return colliding;
}

void Powerup::render() const
{
    switch(state)
    {
        case P_FALLING:
        case P_JUMPING:
        case P_NORMAL:
        case P_ACQUIRED:
            switch(type)
            {
                case SUPER_JUMP:
                    GameObject::render("jump_powerup");
                    break;                    
                case EXTRA_LIFE:
                    GameObject::render("life_powerup");
                    break;
                case SUPER_STRENGTH:
                    GameObject::render("stength_powerup");
                    break;
            }
            break;
    }
}




