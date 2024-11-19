//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"

#include "sslone.h"
#include "jsalazar.h"
#include "rrivasnavarr.h"
#include<cstring>
using namespace std;
//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];
//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
//const float timeslice = 1.0f;
//const float gravity = -0.2f;
//#define PI 3.141592653589793
//#define ALPHA 1
//const int MAX_BULLETS = 11;
//const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
Global::Global() {
	xres = 640;
	yres = 480;
	memset(keys, 0, 65536);
	isPaused = false;
};
Ship::Ship(PhysWorld * in_member = NULL, Player * in_parent = NULL) : Object(in_member)
{
	parent = in_parent;
	//Object(in_member);
	Global gl;
	//added rand to have every new player spawn randomly within the region of the arena
	objectType = SHIP;
	h = 20;
	w = 20;
	pos[0] = (float)(rand() % gl.xres) ;
	pos[1] = (float)(rand() % gl.yres) ;
	drawPos[0] = pos[0] - (w/2);
	drawPos[1] = pos[1] - (h/2);
	
	//       pos[2] = 0.0f;
	member = in_member;
	VecZero(dir);
	VecZero(vel);
	VecZero(acc);
	angle = 0.0;
	//generates a random color on every ship initialization
	color[0] = (float)(rand() % 100) / 100;
	color[1] = (float)(rand() % 100) / 100;
	color[2] = (float)(rand() % 100) / 100;
}

Global gl;
PhysWorld * myPhysWorld = new PhysWorld();
class Game {
	public:
		Player *players[2];
		struct timespec mouseThrustTimer;
		bool mouseThrustOn;
        Map * mapp;
	public:
		Game() {

			srand(time(NULL));
			players[0] = new Player(1,3,6,myPhysWorld);
			players[1] = new Player(1,3,6,myPhysWorld);
			mouseThrustOn = false;

			//temporary, setting passive and weapons for both players for testing
			players[0]->currentPassive = new Speed(players[0]);
			players[1]->currentPassive = new Shield(players[1]);
			players[0]->currentWeapon = new Boomerang(10,players[0],myPhysWorld);		
			//players[1]->currentWeapon = new Sniper(10,players[1]);
            players[1]->currentWeapon = new Bomb(10,players[1],myPhysWorld);
			myPhysWorld->addObject(players[0]->ship);
			myPhysWorld->addObject(players[1]->ship);


			players[1]->ship->setColor(100,90,240);
			players[1]->ship->setColor(100,90,240);

            // creates level map
            mapp = new Map();

		}
		~Game() {

		}
} g;

//X Windows variables
class X11_wrapper {
	private:
		Display *dpy;
		Window win;
		GLXContext glc;
	public:
		X11_wrapper() { }
		X11_wrapper(int w, int h) {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setup_screen_res(gl.xres, gl.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				std::cout << "\n\tcannot connect to X server" << std::endl;
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XWindowAttributes getWinAttr;
			XGetWindowAttributes(dpy, root, &getWinAttr);
			int fullscreen = 0;
			gl.xres = w;
			gl.yres = h;
			if (!w && !h) {
				//Go to fullscreen.
				gl.xres = getWinAttr.width;
				gl.yres = getWinAttr.height;
				//When window is fullscreen, there is no client window
				//so keystrokes are linked to the root window.
				XGrabKeyboard(dpy, root, False,
						GrabModeAsync, GrabModeAsync, CurrentTime);
				fullscreen=1;
			}
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				std::cout << "\n\tno appropriate visual found\n" << std::endl;
				exit(EXIT_FAILURE);
			} 
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
				StructureNotifyMask | SubstructureNotifyMask;
			unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
			if (fullscreen) {
				winops |= CWOverrideRedirect;
				swa.override_redirect = True;
			}
			win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
					vi->depth, InputOutput, vi->visual, winops, &swa);
			//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
			set_title();
			glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			show_mouse_cursor(0);
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void set_title() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "1v1Duel");
		}
		void check_resize(XEvent *e) {
			//The ConfigureNotify is sent by the
			//server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != gl.xres || xce.height != gl.yres) {
				//Window size did change.
				reshape_window(xce.width, xce.height);
			}
		}
		void reshape_window(int width, int height) {
			//window has been resized.
			setup_screen_res(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
			set_title();
		}
		void setup_screen_res(const int w, const int h) {
			gl.xres = w;
			gl.yres = h;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void set_mouse_position(int x, int y) {
			XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
		}
		void show_mouse_cursor(const int onoff) {
			if (onoff) {
				//this removes our own blank cursor.
				XUndefineCursor(dpy, win);
				return;
			}
			//vars to make blank cursor
			Pixmap blank;
			XColor dummy;
			char data[1] = {0};
			Cursor cursor;
			//make a blank cursor
			blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
			if (blank == None)
				std::cout << "error: out of memory." << std::endl;
			cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
			XFreePixmap(dpy, blank);
			//this makes the cursor. then set it using this function
			XDefineCursor(dpy, win, cursor);
			//after you do not need the cursor anymore use this function.
			//it will undo the last change done by XDefineCursor
			//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
		}
} x11(gl.xres, gl.yres);
// ---> for fullscreen x11(0, 0);

//function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	srand(time(NULL));
	logOpen();
	init_opengl();
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int count,done=0;

	g.players[0]->setKeys(XK_Up,XK_Down,XK_Left,XK_Right,XK_Return);
	g.players[1]->setKeys(XK_w,XK_s,XK_a,XK_d,XK_space);

	while (!done) {




		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			//	check_mouse(&e);
			done = check_keys(&e);
		}

		count++;
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}

		render();

		x11.swapBuffers();
	}

	cleanup_fonts();
	logClose();
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}
int check_keys(XEvent *e)
{
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress) {
		//not a keyboard event
		return 0;
	}
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key] = 0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift = 0;
		return 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift = 1;
			return 0;
		}
	}
	(void)shift;
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_s:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_p:
			gl.isPaused = !gl.isPaused;
			break;
	}
	return 0;
}
void physics()
{
	//	Flt d0,d1,dist;
	if(!gl.isPaused)
	{
		for(int i = 0; i < 2; i++)
		{
			//Update ship position
			g.players[i]->ship->pos[0] += g.players[i]->ship->vel[0];
			g.players[i]->ship->pos[1] += g.players[i]->ship->vel[1];
			//Check for collision with window edges
			if (g.players[i]->ship->pos[0] < 0.0) {
				g.players[i]->ship->pos[0] = 0.0;
			}
			if (g.players[i]->ship->pos[0] > (float)gl.xres) {
				g.players[i]->ship->pos[0] = (float)gl.xres;
			}
			if (g.players[i]->ship->pos[1] < 0.0) {
				g.players[i]->ship->pos[1] = 0.0;
			}
			if (g.players[i]->ship->pos[1] > (float)gl.yres) {
				g.players[i]->ship->pos[1] = (float)gl.yres;
			}


			//cout << "Updating Position" << endl;
			//cout << "Before!" << endl;
            g.players[i]->currentWeapon->physics();
			//cout << "After!" << endl;
            g.players[i]->currentPassive->update();
			//g.players[i]->ship->Object::testCollision();
			//cout << "Position Updated" << endl;
		}
		//---------------------------------------------------
		//check keys pressed now

		for(int i =0; i < 2;i++)
		{
			double rSpeed = g.players[i]->getRSpeed();

			if (gl.keys[g.players[i]->left]) {
				g.players[i]->ship->angle += rSpeed;
				if (g.players[i]->ship->angle >= 360.0f)
					g.players[i]->ship->angle -= 360.0f;
			}
			if (gl.keys[g.players[i]->right]) {
				g.players[i]->ship->angle -= rSpeed;
				if (g.players[i]->ship->angle < 0.0f)
					g.players[i]->ship->angle += 360.0f;
			}
			if (gl.keys[g.players[i]->up]) {

				//apply thrust
				//convert->ship angle to radians
				Flt rad = ((g.players[i]->ship->angle+90.0) / 360.0f) * PI * 2.0;
				g.players[i]->isThrust = true;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				//g.ship.vel[0] += xdir*0.02f;
				//g.ship.vel[1] += ydir*0.02f;
				g.players[i]->ship->vel[0] += xdir * g.players[i]->getSpeed();
				g.players[i]->ship->vel[1] += ydir * g.players[i]->getSpeed();
				Flt speed = sqrt(g.players[i]->ship->vel[0]*g.players[i]->ship->vel[0]+
						g.players[i]->ship->vel[1]*g.players[i]->ship->vel[1]);
				if (speed > g.players[i]->getSpeed()) {
					speed = g.players[i]->getSpeed();
					normalize2d(g.players[i]->ship->vel);
					g.players[i]->ship->vel[0] *= speed;
					g.players[i]->ship->vel[1] *= speed;
				}
			} 
			// reverse motion for ship
			else if (gl.keys[g.players[i]->down]){
				// apply thrust
				// convert->ship angle to radius
				Flt rad = ((g.players[i]->ship->angle+90.0) / 360.0f) * PI * 2.0;
				//g.players[i]->isThrust = false;
				// convert angle to vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				// 
				g.players[i]->ship->vel[0] -= xdir * g.players[i]->getSpeed();
				g.players[i]->ship->vel[1] -= ydir * g.players[i]->getSpeed();
				Flt speed = sqrt(g.players[i]->ship->vel[0]*g.players[i]->ship->vel[0]+
						g.players[i]->ship->vel[1]*g.players[i]->ship->vel[1]);
				if (speed > g.players[i]->getSpeed()) {
					speed = g.players[i]->getSpeed();
					normalize2d(g.players[i]->ship->vel);
					g.players[i]->ship->vel[0] *= speed;
					g.players[i]->ship->vel[1] *= speed;
				}
			}
			else
			{

				g.players[i]->isThrust = false;
				g.players[i]->ship->vel[0] = 0;
				g.players[i]->ship->vel[1] = 0;

			}
			if (gl.keys[g.players[i]->attack]) {
				g.players[i]->currentWeapon->fireWeapon();
			}

		}
		if (g.mouseThrustOn) {
			//should thrust be turned off
			struct timespec mtt;
			clock_gettime(CLOCK_REALTIME, &mtt);
			double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
			if (tdif < -0.3)
				g.mouseThrustOn = false;
		}
	
		for(int i = 0; i < 2; i++){
			g.players[i]->ship->Object::testCollision();
		}
	}

}

void render()
{
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;

	//--draws background
	glColor3f(255/255.0,255/255.0,255/255.0);
	glBegin(GL_POLYGON);
	glVertex2i(0,0);
	glVertex2i(gl.xres,0);
	glVertex2i(gl.xres,gl.yres);
	glVertex2i(0,gl.yres);
	glEnd();
	//--




	ggprint8b(&r, 16, 0x00ff0000, "Player 1 - w a s d space");
	ggprint8b(&r, 16, 0x00ff0000, "Player 2 - up down left right enter");
	ggprint8b(&r, 16, 0x00ff0000, "PAUSE : 'P'");

	glColor3ub(0,0,0);


	glPushMatrix();
	glTranslatef((gl.xres/2)-50, (gl.yres/2)-50, 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f, 100.0f);
	glVertex2f(  0.0f,  0.0f);
	glVertex2f(  100.0f,  0.0f);
	glVertex2f(100.0f, 100.0f);
	glEnd();
	glPopMatrix();

	
	g.players[0]->ship->drawHitbox();
	g.players[1]->ship->drawHitbox();
	

	//Draw the ship
	for(int i =0; i < 2; i++)
	{
		
		float tempX = g.players[i]->ship->pos[0];
		float tempY = g.players[i]->ship->pos[1];
		
		glColor3fv(g.players[i]->ship->color);
		glPushMatrix();
		glTranslatef(tempX, tempY, g.players[i]->ship->pos[2]);
		glRotatef(g.players[i]->ship->angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex2f(-12.0f, -10.0f);
		glVertex2f(  0.0f,  20.0f);
		glVertex2f(  0.0f,  -6.0f);
		glVertex2f(  0.0f,  -6.0f);
		glVertex2f(  0.0f,  20.0f);
		glVertex2f( 12.0f, -10.0f);
		glEnd();
		glPopMatrix();
	}


	// cout << "Drawing Weapons" << endl;
	for(int count =0;count < 2;count++)
	{ 
		g.players[count]->currentWeapon->render();
		g.players[count]->currentPassive->render();
	}

	// render Map
    g.mapp->render();

	if(gl.isPaused)
	{
		//glBlendFunc  (GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
		//glEnable     (GL_BLEND);
		//glClearColor(0.0,0.0,0.0,0.0);
		//glEnable     (GL_COLOR_MATERIAL);
		Rect p;
		p.bot = gl.yres / 2;
		p.left = gl.xres/2;
		ggprint8b(&p, 16, 0x00ff0000, "GAME PAUSED");
		/*
		   glColor4f(1.0f,0,0,0.5);
		   glBegin(GL_POLYGON);
		   glVertex2i(0,0);
		   glVertex2i(gl.xres,0);
		   glVertex2i(gl.xres,gl.yres);
		   glVertex2i(0,gl.yres);
		   */
		glEnd();

	}
}



