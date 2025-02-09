/**
 * Fighter.cpp
 *
*/

#include "Fighter.hpp"
#include <stdio.h>
#include "rendering/TexturedMesh.hpp"
#include "rendering/Fighter.hpp"
#include "io/Read3DS.hpp"
#include "math/Global.hpp"
#include "control/control.hpp"
#include <cmath>

int damage = 0;
int score  = 0;

void Fighter::shoot()
{
    shot = true;
    // Create a new bullet with this fighter's position an x-Axis
    Bullet* b = new Bullet(m_position, m_xAxis);
    // Read 3ds for Rocket
    Read3DS reader("res/models/rocket.3ds");
    reader.getMesh(*(static_cast<TexturedMesh*>(b)));
    // Set the axis of the Rocket
    b->setAxis(m_xAxis,m_yAxis,m_zAxis);
    // Start a new thread, move the bullet
    b->start();
    // Add it to this fighter's vector of bullets
    m_bullets.push_back(b);
}

vector<Bullet*> Fighter::get_Bullets()
{
	vector<Bullet*> ret;
	ret = m_bullets;
  	return ret; 
}

void Fighter::render(bool printFighter)
{
    rotate(PITCH, m_anglepitch);
    rotate(YAW  , m_angleyaw);
    rotate(ROLL , m_angleroll); 
	m_position = m_position - m_xAxis * m_speed;
    
	if (printFighter)
	{
		// Render the fighter
    	TexturedMesh::render();
	}
	renderBullet();
}

void Fighter::renderBullet()
{
    vector<Bullet*>::iterator bulletIt;
    bulletIt = m_bullets.begin();
    // Iterate over the fighter's bullets and render them
    // if the bullet's lifetime is over, erase it from the vector.
    while(bulletIt != m_bullets.end()){
        (*bulletIt)->render();
        if(!(*bulletIt)->isAlive()){
            bulletIt = m_bullets.erase(bulletIt);
	    }else{
	        bulletIt++;	
	    }
    }
}

// Anmerkung eigentlich zuviel rausnehmen
void Fighter::killBullet(Bullet* b)
{
        //killen der kugel
        vector<Bullet*>::iterator bulletIt;
        bulletIt = m_bullets.begin();
        while(bulletIt != m_bullets.end())
        {
	        if((*bulletIt) == b)
	        {
	                bulletIt = m_bullets.erase(bulletIt);
                        b->kill();
                        break;
	        }
	        else
	        {
	                bulletIt++;	
	        }
      }
        
}

void Fighter::increaseDamage(int i)
{
    damage += i;
}

void Fighter::changeSpeed(float change)
{
    if(m_speed <= 100 && m_speed >= 0)   
    {
        m_speed += change;
        if(m_speed > 100) 
        {
            m_speed = 100;
        }
        if(m_speed < 0)
        {
            m_speed = 0;
        }
    }
}


int Fighter::getDamage()
{
    return damage;
}
float Fighter::getSpeed()
{
    return m_speed;
}
int Fighter::getScore()
{
    return score;
}

bool Fighter::wasShot()
{
	if (shot)
	{
		shot = false;
		return true;
	}
	return false;
}

void Fighter::resetDamage()
{
    damage = 0;
}

void Fighter::reset_position()
{
	glVector<float> temp (0.0,0.0,0.0);
	m_position = temp;
}

void Fighter::changeAngle(int axis, float change)
{
    switch(axis){
        case PITCH: // up-down
            m_anglepitch += change;
            if(m_anglepitch >= MAXANGLE)
            {
                m_anglepitch = MAXANGLE;
            }
            else if(m_anglepitch <= -MAXANGLE)
            {
                m_anglepitch = -MAXANGLE;
            } 
            //std::cout << "pitch: " << m_anglepitch << std::endl;               
            break;

        case YAW: // left-right
            m_angleyaw += change;
            if(m_angleyaw >= MAXANGLE)
            {
                m_angleyaw = MAXANGLE;
            }
            else if(m_angleyaw <= -MAXANGLE)
            {
                m_angleyaw = -MAXANGLE;
            } 
            //std::cout << "yaw: " << m_angleyaw << std::endl;               
            break;

        case ROLL: // rollen
            m_angleroll += change;
            if(m_angleroll >= MAXANGLE)
            {
                m_angleroll = MAXANGLE;
            }
            else if(m_angleroll <= -MAXANGLE)
            {
                m_angleroll = -MAXANGLE;
            } 
            //std::cout << "roll: " << m_angleroll << std::endl;            
            break;
    }
}

void Fighter::reduceAngle()
{
    if(m_speed != 0)
    {
        changeSpeed(BRAKE);
    }

    if(m_anglepitch != 0)
    {
        if(m_anglepitch > 0)
        {
            changeAngle(PITCH, -REDUCE);
        }
        else if(m_anglepitch < 0)
        {
            changeAngle(PITCH, REDUCE);
        }
        if(m_anglepitch > 0 && m_anglepitch < REDUCE)
        {
            m_anglepitch = 0;
        }
        else if(m_anglepitch < 0 &&m_anglepitch > -REDUCE)
        {
            m_anglepitch = 0;
        }
    } 

    if(m_angleyaw != 0)
    {
        if(m_angleyaw > 0)
        {
            changeAngle(YAW, -REDUCE);
        }
        else if(m_angleyaw < 0)
        {
            changeAngle(YAW, REDUCE);
        }
        if(m_angleyaw > 0 && m_angleyaw < REDUCE)
        {
            m_angleyaw = 0;
        }
        else if(m_angleyaw < 0 &&m_angleyaw > -REDUCE)
        {
            m_angleyaw = 0;
        }
    }
   
    if(m_angleroll != 0)
    {
        if(m_angleroll > 0)
        {
            changeAngle(ROLL, -REDUCE);
        }
        else if(m_angleroll < 0)
        {
            changeAngle(ROLL, REDUCE);
        }
        if(m_angleroll > 0 && m_angleroll < REDUCE)
        {
            m_angleroll = 0;
        }
        else if(m_angleroll < 0 &&m_angleroll > -REDUCE)
        {
            m_angleroll = 0;
        }
    }              
}
void Fighter::setNULL()
{
    m_angleroll = 0;
    m_anglepitch= 0;
    m_angleyaw  = 0;
    m_speed     = 0;
}

