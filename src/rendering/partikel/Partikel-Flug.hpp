#ifndef _PARTIKEL_FLUG_H
#define _PARTIKEL_FLUG_H

#include "math/glVector.hpp"
#include <QtOpenGL>

// Partikel für Flug

class PartikelFlug
{
public:
    //Konstruktor
    PartikelFlug(float lifetime, glVector<float> pos, int size, glVector<float> color);
    //Desructor
    ~PartikelFlug();
    // weitersetzen
    void update();
    
    bool isAlive();

    void render();
private:
    //Gesundheitstatus
    bool m_alive;
    //Lebensdauer
    float m_lifetime;
    //Startzeit
    float m_birthday;
    //aktuelle Position
    glVector<float> m_position;
    //grösse
    float m_size;

    float m_alpha;
    //Farbe
    glVector<float> m_color;
       
    static GLuint TexID1;
};

#endif //_PARTIKEL_FLUG_H
