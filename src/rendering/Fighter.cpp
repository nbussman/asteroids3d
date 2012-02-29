/**
 * Fighter.cpp
 *
*/

#include "Fighter.hpp"
#include <stdio.h>
#include "rendering/TexturedMesh.hpp"
#include "rendering/Fighter.hpp"
#include "io/Read3DS.hpp"

int damage = 0;
int score  = 0;

Fighter::~Fighter()
{
    delete m_normalBuffer;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    // TODO Auto-generated destructor stub
    delete &m_materials;
    delete &m_matFaceLists;
    delete m_textureCoords;
    delete &m_bullets;
}

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

