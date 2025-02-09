/**
 * @file Asteorid.hpp
 *
 */

#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

#include <GL/glu.h>
#include <GL/glut.h>
#include <QThread>
#include <QObject>
#include <QMetaType>
#include <stdio.h>

#include "io/SoundManager.hpp"
#include "TexturedMesh.hpp"

// We use common cpp threads for this class: 
// http://www.gnutelephony.org/index.php/GNU_Common_C%2B%2B 

/**
 * @brief Renders a Asteorid
 */
class Asteorid: public QThread, public TexturedMesh
{
Q_OBJECT

public:

    /**
     * @brief Contructor. Build a Asteorid on the given Fighter's
     *                      position. The Asteorid will move on the
     *                      given axis.
     * @param   start_position   Position of the fighter that shoots this Asteorid
     * @param   flight_axis   Axis the Asteorid will move on
     */
    Asteorid(glVector<float> start_position, glVector<float> flight_axis);

    /**
     * @brief Moves the Asteorid until it's lifetime is over.
     */
	void run();
    
    /**
      * @brief returns position
      */
      glVector<float> getPosition();

    /**
     * @brief Returns the status of this Asteorid.
     * @return false, if the Asteorid's lifetime is over and true otherwise
     */
	bool isAlive();
	
    /**
     * @brief destroys the asteorid
     */
	void destroy();

    /**
     * @brief changes the flight-direction of the Astroid
     */
     void changeDirection(glVector<float> dir);

    /**
     * @brief returns the flight-axis
     */
     glVector<float> getFlightAxis();
     
     void set_flight_axis(glVector<float> new_axis);
     float get_radius();
     bool is_mini();
     bool is_hitable();
     void set_hitable(bool hit);
     void set_speed(float s);

protected:
    // true, if the Asteorid's lifetime isn't over yet
	bool alive;
    // axis, the Asteorid is moving on (one of the shooting fighter's axes)
	glVector<float> flight_axis;
	float speed;
	float radius;
	bool mini;
	bool hitable;
	int wait;
	
signals:
	void destroyed(float x, float y, float z);
	void hit();
};

#endif /* ASTEORID_HPP_ */
