#include "Dude.h"


Dude::Dude(const Point2f &position_, const Vector2f &size_, const float &theta_) :
    GameObject(position_, size_, theta_, 0.0f /* speed */, 0.19875f /* accel */)
{
	state = D_NORMAL;
    wallJumpCount = 0;
    walledRight = -1;
}

Dude::~Dude(void)
{
}

void Dude::perform_logic(float time, float time_passed, float time_step, ControlState cs)
{
    float move_scale = 35.0f;
    if((cs.left && !collisions[LEFT]) || (cs.right && !collisions[RIGHT])) m_position.x += ((cs.right-cs.left) * time_step * 145.0f);
    switch(state)
	{
        case D_NORMAL:
            moveDown(time_step);
            
            if(cs.jump && collisions[BOTTOM])
            {
                wallJumpCount = 0;
                lastWallJump = time;
                walledRight = -1;
                m_speed = 8.0f;
                state = D_JUMPING;
            }
            break;
        case D_JUMPING:
            m_speed -= m_accel;
            if(!collisions[TOP]) m_position.y -= m_speed * move_scale * time_step;
            else m_speed = 0.0f;
            if(m_speed <= 0.0f) state = D_NORMAL;
            break;
	}
    
    if(((collisions[RIGHT] && walledRight == 0) || (collisions[LEFT] && walledRight == 1) || ((collisions[RIGHT] || collisions[LEFT]) && walledRight == -1)) && cs.jump && wallJumpCount < 5 && time - lastWallJump >= 0.5f)
    {
        walledRight = collisions[RIGHT] ? 1 : 0;
        lastWallJump = time;
        state = D_JUMPING;
        m_speed = 8.0f;
        wallJumpCount++;
    }
}

void Dude::render() const
{
    if(collisions[RIGHT] && !collisions[BOTTOM])
        GameObject::render("dude_wall_r");
    else if (collisions[LEFT] && !collisions[BOTTOM])
        GameObject::render("dude_wall_l");
    else
        GameObject::render("dude");
}

int Dude::left()
{
    return m_position.x + 17;
}

int Dude::right()
{
    return m_position.x + m_size.x - 17;
}







