/**
 * @file RenderFrame.cpp
 *
 * @author gruppe3
 */

#include "RenderFrame.hpp"

#include <set>

#include <QKeyEvent>
#include <QMouseEvent>

#include "rendering/TexturedMesh.hpp"
#include "rendering/Fighter.hpp"
#include "math/glVector.hpp"
#include "io/Read3DS.hpp"
#include "rendering/Asteorid.hpp"
#include <stdio.h>
#include "io/SoundManager.hpp"
#include <string>
#include "view/Menu.hpp"

//statische Variablen
Camera RenderFrame::m_cam;
bool menu = false; 
bool warning_sound = false;

RenderFrame::RenderFrame(QWidget* parent) : QGLWidget(parent)
{
    // setze neuen Timer
    m_timer = new QTimer();
    // setze Intervall auf 25ms
    m_timer->setInterval(25);
    // verbinde mit updateGL()
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()),Qt::QueuedConnection);
    
    // setze 2.Timer
    m_timer2= new QTimer();
    // setze Intervall auf 100ms
    m_timer2->setInterval(100);
    // verbinde mit Gameupdate()
    connect(m_timer2,SIGNAL(timeout()), this, SLOT(Gameupdate()));
    
    // erzeugt den Joystick (string: Ort des Joysticks in Linux) gesetzt für Ubuntu
    joys = new JoystickControl("/dev/input/js0");
    
    // fragt ob verbunden wurde
    joystick = joys->connected();
    
	setAutoFillBackground(false);

    show();
    
    // Soll zuerst das Menu darstellen
    menu = true;
    // Nicht den Highscore anzeigen
    paintHighscore = false;
    // QLabel erzeugen
    textLabel =new QLabel(this);
    
    // GamerOver auf false setzen
    gameOver = false;
    // Abfrage des Usernames
    bool ok;
    userName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("User name:"), QLineEdit::Normal,
                                          QDir::home().dirName(), &ok);
    if (ok && !userName.isEmpty())
    {
        textLabel->setText(userName); 
    }
    string so;
    so = userName.toStdString();
    for( unsigned int i=0; i<so.length(); i++)
    {
        if(so[i] == ' ')
        { 
            so.erase(i,1);
        }
    }
    userName= QString::fromStdString(so);    
}

RenderFrame::~RenderFrame()
{
    // wenn Joystick vorhanden dann löschen
    if(joystick)
    {
        delete joys;
    }
    //Fighter niht löschen, erzeugt SegFaults
    //delete Game::getFighter();
    delete m_skybox;
    SoundManager::deleteManager();
}

void RenderFrame::start()
{
    // MenüScreen nicht mehr anzeigen
    menu = false;
    Menu::deleteSplash();
    
    //Game Initialisieren
    Game::Init();

    //Collision starten
    Game::getCollision()->start();
    
    //Startperspektive setzen
    m_cam.setCockpit();
    Game::setView(0);
    //Cockpit setzen
    if (Game::getHud())
    {
        Game::getHud()->loadCockpit();	
    }

    //Musik starten
    SoundManager::playBattleMusic();
    
    // starte Timer
    m_timer->start();
    m_timer2->start();

    // Setze Bewegung des Fighters auf Null    
    Game::getFighter()->setNULL();
}

void RenderFrame::initializeGL()
{

	// Create a sky box
	string names[6];
	names[0] = "box1.ppm";
	names[1] = "box2.ppm";
	names[2] = "box3.ppm";
	names[3] = "box4.ppm";
	names[4] = "box5.ppm";
	names[5] = "box6.ppm";

	m_skybox = new Skybox(2048, names, m_cam);
    
	glMatrixMode(GL_MODELVIEW);
	//glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	// Setup two light sources
	float light0_position[4];
	float light0_ambient[4];
	float light0_diffuse[4];

	float light1_position[4];
	float light1_ambient[4];
	float light1_diffuse[4];

	light0_position[0] =   1.0f; light0_ambient[0] = 0.8f; light0_diffuse[0] = 0.8f;
	light0_position[1] =   1.0f; light0_ambient[1] = 0.8f; light0_diffuse[1] = 0.8f;
	light0_position[2] =   0.0f; light0_ambient[2] = 0.8f; light0_diffuse[2] = 0.8f;
	light0_position[3] =   1.0f; light0_ambient[3] = 0.1f; light0_diffuse[3] = 1.0f;

	light1_position[0] =   0.0f; light1_ambient[0] = 0.1f; light1_diffuse[0] = 0.5f;
	light1_position[1] =  -1.0f; light1_ambient[1] = 0.1f; light1_diffuse[1] = 0.5f;
	light1_position[2] =   0.0f; light1_ambient[2] = 0.1f; light1_diffuse[2] = 0.5f;
	light1_position[3] =   1.0f; light1_ambient[3] = 1.0f; light1_diffuse[3] = 1.0f;

	// Light 1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);

	// Light 2
	glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Enable z buffer and gouroud shading
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	glShadeModel (GL_SMOOTH);
}
 
void RenderFrame::resizeGL(int w, int h)
{    
	if(h == 0)
	{
		h = 1;
	}

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,100000);
	glMatrixMode(GL_MODELVIEW);

}
void RenderFrame::setCam() 
{
    //wenn Fighter vorhanden
    if(Game::getFighter()) 
    {
        // hole die benötigen Vektoren vom Fighter
        glVector<float> pos = (*(static_cast<Transformable*>(Game::getFighter()))).getPosition();
        glVector<float> front=(*(static_cast<Transformable*>(Game::getFighter()))).getFront();
        glVector<float> up = (*(static_cast<Transformable*>(Game::getFighter()))).getUp();
        glVector<float> side =(*(static_cast<Transformable*>(Game::getFighter()))).getSide();
        // setze die Kamera
        m_cam.setLocation(pos, front, up, side);
    }
}
void RenderFrame::paintGL()
{       
    //Steuerung updaten
    if(joystick) {
        joys->update();
    }
    Keyboard::update();

    setCam();
    setFocus();
	// Set black background color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Clear bg color and enable depth test (z-Buffer)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Skybox
	if(m_skybox)
	{
	    m_skybox->render();
	}
    // Render Fighter
	if(Game::getFighter())
	{
		Game::getFighter()->render(!Game::getHud()->getView());
	}
    // Render Asteroiden
	if(Game::getGalaxis()) 
	{
		Game::getGalaxis()->render();
	}

    // FlugPartikel erzeugen
    Game::getEmitterFlug()->createPartikel();

    // Explosionspartikel updaten und rendern
    Game::getEEmit()->update();

    // Flugpartikel updaten und rendern
    Game::getEmitterFlug()->update();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLoadIdentity();

    // Painter erzeugen
    QPainter painter(this);
	Game::getHud()->setPainter( &painter );

    // HUD updaten
    if(Game::getFighter()) {
       	Game::getHud()->setLevel(Game::getGalaxis()->getLevelnumber());
        Game::getHud()->setIncLevel(Game::getGalaxis()->shouldIncLevel());
   	    Game::getHud()->setFighterData(Game::getFighter()->getDamage(), Game::getScore(), Game::getFighter()->getSpeed(), Game::getFighter()->wasShot());
   	    Game::getHud()->setAstroidsVector(Game::getCollision()->getCollisionVector());
        Game::getHud()->draw(width(),height(),font());
        
        // Warning bei zu Nahem Asteoriden
        if(Game::getCollision()->getWarning())
        {
            Game::getHud()->drawWarning();
        }
    }
    
    // bei zuviel Schaden Gameover
    if(Game::getFighter()->getDamage()>=100)
    {
    	gameOver = true;
        ReadTXT *reader = new ReadTXT();
        reader->write(userName.toStdString(), Game::getScore());
        menu = true;	
    }

    // Menü anzeigen
    if(menu)
    {
    	if(gameOver)
    	{
    		Menu::drawGameover(width(),height(), Game::getHud());
    	} else
        {
      	    Menu::drawSplash(width(),height(), Game::getHud());
        }        
        if(paintHighscore)
        {
            Game::getHud()->drawHighscore();
        }
    }
    // Keyboard updaten
    Keyboard::update();
    
    painter.end();
    glPopMatrix();
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);   

    glFinish();
	// Call back buffer
	swapBuffers();
}

void RenderFrame::keyPressEvent (QKeyEvent  *event)
{
    // Key an Keyboard übergeben
    Keyboard::keypressed(event);
    
    if(menu) {
    	// bei H Highscore anzeigen
    	if (event->key() == Qt::Key_H)
    	{
            paintHighscore = !paintHighscore;
       	    paintGL();
   		}
        // bei Enter Spiel Starten
        if (event->key() == Qt::Key_Return)
        {
        	if(gameOver){
        		Game::game_over();
        	}
        	else 
        	{
        		gameOver = false;
        	}
            start();
        }     
    }
}

void RenderFrame::keyReleaseEvent (QKeyEvent  *event)
{  
    // Keyand Keyboard übergeben
    Keyboard::keyrelease(event);
}

void RenderFrame::setupViewport(int width, int height)
{
     int side = qMin(width, height);
     glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void RenderFrame::Gameupdate()
{
    Game::update();
}
