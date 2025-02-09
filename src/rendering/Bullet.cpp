 /**
 * Bullet.cpp
 *
 */

#include "Bullet.hpp"
#include <stdio.h>

float Bullet::b_speed = 3;

Bullet::Bullet(glVector<float> fighter_position, glVector<float> fighter_axis, glVector<float> asteroid)
{
   	target = asteroid;
	this->fighter_axis = (fighter_position - target)* - (b_speed);
	alive = true;
    // same position as the fighter
 	m_position = fighter_position;
    SoundManager::playFireSound();
    seeking = true;
	
}

Bullet::Bullet(glVector<float> fighter_position, glVector<float> fighter_axis)
{
    // the Bullet will move on this axis, it has to be reversed for the direction to be right
    fighter_axis.normalize();  
    this->fighter_axis = fighter_axis * - b_speed;
    alive = true;
    // same position as the fighter
    m_position = fighter_position;
    SoundManager::playFireSound();
    seeking=false;
}

bool Bullet::isAlive()
{
	return alive;
}

void Bullet::run()
{
	int i = 0;
    // Modify the Asteorid's position until the lifetime is over

	while(i < lifetime){
		m_position = fighter_axis  + m_position;
		i++;
		usleep(1000);
	}
	alive = false;
}
void Bullet::kill()
{
        alive = false;
}

glVector<float> Bullet::getPosition()
{
        return m_position;
}
void Bullet::setAxis(glVector<float> xA, glVector<float> yA, glVector<float> zA)
{
        m_xAxis = xA;
        m_yAxis = yA;
        m_zAxis = zA;
}





