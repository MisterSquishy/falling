#include "Dude.h"


Dude::Dude(const Point2f &position_, const Vector2f &size_, const float &theta_, const float &speed_y_, const float &speed_x_) :
    GameObject(position_, size_, theta_, speed_y_ /* speed_y */, speed_x_ /* speed_x */, Y_ACCELERATION /* accel_y */)//, UNDEFINED /* accel_x */)
{
	state = D_JUMPING;
    wallJumpCount = 0;
	lastWallJump = UNDEFINED;
	remainingLives = DUDE_INITIAL_LIFE_COUNT;
    render_right = true;
	safeToWallJump = true;
    dead = false;
}

Dude::~Dude(void)
{
}

bool Dude::perform_logic(float current_time, float time_step, ControlState cs)
{
    if(m_speed_x > 0)
        render_right = true;
    if(m_speed_x < 0)
        render_right = false;
    
	moveSide(time_step, cs.right-cs.left);

	switch(state)
	{
        case D_NORMAL:
            moveDown(time_step);
            
            if(cs.jump && collisions[BOTTOM])
            {
                wallJumpCount = 0;
				lastWallJump = current_time;
                m_speed_y = JUMP_VERTICAL_SPEED;
                state = D_JUMPING;
				safeToWallJump = false;
            }
            else if(cs.destroy && (collisions[LEFT] || collisions[RIGHT]) && collisions[BOTTOM])
            {
                if(collisions[LEFT])
                    for(int i = 0; i < wasLeft.size(); i++)
                        ((Block*) wasLeft[i])->destroy();
                else if(collisions[RIGHT])
                    for(int i = 0; i < wasRight.size(); i++)
                        ((Block*) wasRight[i])->destroy();
            }
            break;
        case D_JUMPING:
            m_speed_y -= m_accel_y;
            if(!collisions[TOP])
			{
				m_position.y -= m_speed_y * SCALE * time_step;
				if(m_position.y < GRID_SIZE)
				{
					m_position.y = GRID_SIZE;
					state = D_NORMAL;
					m_speed_y = DORMANT;
				}
			}
            else m_speed_y = DORMANT;
            if(m_speed_y <= DORMANT) state = D_NORMAL;
            break;
	}
    
	if(!safeToWallJump && !cs.jump) safeToWallJump = true;

	// Wall Jump
    if(safeToWallJump && !collisions[BOTTOM] && (collisions[RIGHT] || collisions[LEFT]) && cs.jump && wallJumpCount < 5 && current_time-lastWallJump >= 0.20f)
    {
        m_speed_y = WALL_JUMP_VERTICAL_SPEED;
		m_speed_x = m_speed_x + ((collisions[LEFT] - collisions[RIGHT]) * JUMP_HORIZONTAL_SPEED);
        lastWallJump = current_time;
		state = D_JUMPING;
        wallJumpCount++;
		safeToWallJump = false;
    }
    
    if(!collisions[BOTTOM] && !collisions[RIGHT] && !collisions[LEFT])
    {
        m_theta += SPIN_SPEED * SCALE * time_step * (render_right ? -1 : 1);
    }
    else
    {
        m_theta = DORMANT;
    }
    bool retval = false;
    if(collisions[TOP])
	{
		state = D_JUMPING;
		remainingLives--;
		play_sound("death");
		if(remainingLives != 0)
		{
			retval = true;
			state = D_DEAD;
		}
	}

	return retval;
}

bool Dude::isDead()
{
    return remainingLives == 0;
}

void Dude::spawn(Point2f loc)
{
	state = D_NORMAL;
	m_position = loc;
	m_speed_x = 0.0f;
	m_speed_y = 0.0f;
}

void Dude::revive()
{
    dead = false;
	remainingLives = DUDE_INITIAL_LIFE_COUNT;
}

void Dude::render() const
{
	switch(state)
	{
	case D_JUMPING:
	case D_NORMAL:
		if((collisions[RIGHT] || collisions[LEFT]) && !collisions[BOTTOM])
		    GameObject::render("dude_wall", !render_right);
		else if(!collisions[BOTTOM])
		{
		    GameObject::render("dude_air", !render_right);
		}
		else
		{
		    GameObject::render("dude", !render_right);
		}
	case D_DEAD:
		for(int i = 0; i < remainingLives; i++)
		{
			// Use a helper defined in Zeni/EZ2D.h
			render_image(
				"dude_life_indicator",
				Point2f(10.0f+(25*i), 30.0f),
				Point2f(10.0f+20.0f+(25*i), 30.0f+20.0f),
				0,
				1.0f,
				m_position + 0.5f * m_size,
				false,
				Color());
		}
		break;
	}
}

int Dude::left()
{
    return m_position.x + 17;
}

int Dude::right()
{
    return m_position.x + m_size.x - 17;
}






