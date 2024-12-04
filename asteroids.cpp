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

class Image {
public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
                if (fname[0] == '\0')
                        return;
                //printf("fname **%s**\n", fname);
                int ppmFlag = 0;
                char name[40];
                strcpy(name, fname);
                int slen = strlen(name);
                char ppmname[80];
                if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                        ppmFlag = 1;
                if (ppmFlag) {
                        strcpy(ppmname, name);
                } else {
                        name[slen-4] = '\0';
                        //printf("name **%s**\n", name);
                        sprintf(ppmname,"%s.ppm", name);
                        //printf("ppmname **%s**\n", ppmname);
                        char ts[100];
                        //system("convert eball.jpg eball.ppm");
                        sprintf(ts, "convert %s %s", fname, ppmname);
                        system(ts);
                }
                //sprintf(ts, "%s", name);
                //sprintf(ts, "%s", name);
                FILE *fpi = fopen(ppmname, "r");
                if (fpi) {
                        char line[200];
                        fgets(line, 200, fpi);
                        fgets(line, 200, fpi);
                        //skip comments and blank lines
                        while (line[0] == '#' || strlen(line) < 2)
                                fgets(line, 200, fpi);
                        sscanf(line, "%i %i", &width, &height);
                        fgets(line, 200, fpi);
                        //get pixel data
                        int n = width * height * 3;
                        data = new unsigned char[n];
                        for (int i=0; i<n; i++)
                                data[i] = fgetc(fpi);
                        fclose(fpi);
                } else {
                        printf("ERROR opening image: %s\n",ppmname);
                        exit(0);
                }
                if (!ppmFlag)
                        unlink(ppmname);
        }
};
Image img[5] = {
"./images/menu.png",
"./images/button1.png",
"./images/button2.png",
"./images/button3.png",
"./images/button4.png" };

Global::Global() {
	//xres = 640;
	//yres = 480;
	//xres = 960;
	//yres = 540;
	debug = 0;
	xres = 1440;
	yres = 810;
	memset(keys, 0, 65536);
	isPaused = false;
    showMenu = true;
	wepNum = 0;
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
	/*
	int spawnPos[4][2] = {{50,50},{gl.xres - 50,gl.yres - 50},{gl.xres - 50, 50}, {50, gl.yres - 50}};
	pos[0] = (float)(rand() % gl.xres) ;
	pos[1] = (float)(rand() % gl.yres) ;
	pos[0] = spawnPos[rand() % 4][0];
	pos[1] = spawnPos[rand() % 4][1];
	*/
	drawPos[0] = pos[0] - (w/2);
	drawPos[1] = pos[1] - (h/2);
	clearFilter();

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
BoxWorld * myBoxWorld = new BoxWorld(myPhysWorld);
MapHandler * myMapHandler = new MapHandler();
class Game {
	public:
		Player *players[2];
		struct timespec mouseThrustTimer;
		bool mouseThrustOn;
		Hud * myHud;
	public:
		Game() {
			srand(time(NULL));
			players[0] = new Player(1,3,6,myPhysWorld);
			players[1] = new Player(1,3,6,myPhysWorld);
			mouseThrustOn = false;
			players[0]->opponent = players[1];
			players[1]->opponent = players[0];
			players[0]->ship->pos[0] = gl.xres - 100;
			players[0]->ship->pos[1] = 100;
			players[1]->ship->pos[0] = 100;
			players[1]->ship->pos[1] = gl.yres - 100;


			//temporary, setting passive and weapons for both players for testing
			//players[0]->currentPassive = new Speed(players[0]);
	//		players[1]->currentPassive = new Shield(players[1]);
			players[0]->currentWeapon = new Boomerang(10,players[0],myPhysWorld);		
			players[1]->currentWeapon = new Boomerang(10,players[1],myPhysWorld);		
			//players[1]->currentWeapon = new Boomerang(10,players[1],myPhysWorld);		
			//players[1]->currentWeapon = new Bomb(10,players[1],myPhysWorld);
			//players[1]->currentWeapon = new Shotgun(10,players[1],myPhysWorld);
            		//players[1]->currentWeapon = new Sniper(10,players[1],myPhysWorld);
			myPhysWorld->addObject(players[0]->ship);
			myPhysWorld->addObject(players[1]->ship);


			players[0]->ship->setColor(0,255,0);
			players[1]->ship->setColor(255,0,255);

			// creates level map
			myHud = new Hud(players[0],players[1]);
				
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
			show_mouse_cursor(1);
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
			
			//setup_screen_res(width, height);
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, gl.xres , 0, gl.yres , -1, 1);
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

char * args[]  = {(char*)"",(char*)"",NULL};
char * path;
Menu * menu = new Menu();

//==========================================================================
// M A I N
//==========================================================================
int main(int argc, char *argv[])
{
	path = argv[0];
	if(argc >1)
	{
	printf("%s \n",(char *)argv[1]);
	printf("%s \n",(char *)argv[0]);
	cout << atoi(args[1]) << endl;
	}
	
	
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
			check_mouse(&e);
			done = check_keys(&e);
		}

		count++;
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;

        if (!gl.showMenu) {
            while (physicsCountdown >= physicsRate) {
                physics();
                physicsCountdown -= physicsRate;
            }
            render();
        } else {
	    
           	 menu->render();
		Rect r;
		r.bot = (gl.yres/2) - 50;
		r.left = gl.xres/2;
		r.center = 1;
		if(argc > 1)
		{

			int a = atoi(argv[a]);
			printf("%d",a);
			fflush(stdout);
			if(atoi(argv[1]) ==1)
			{
				ggprint8b(&r, 16, 0x00ff0000, "Player 1 Wins!!");

			}
			if(atoi(argv[1]) == 0)
			{
				ggprint8b(&r, 16, 0x00ff0000, "Player 2 Wins!!");

			}


		}
	}


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

	// load image files
	glGenTextures(1, &gl.menu);
	glGenTextures(1, &gl.button1);
	glGenTextures(1, &gl.button2);
	glGenTextures(1, &gl.button3);
	glGenTextures(1, &gl.button4);

	//menu
	//
	glBindTexture(GL_TEXTURE_2D, gl.menu);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[0].width, img[0].height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

	//button1
	//
	glBindTexture(GL_TEXTURE_2D, gl.button1);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[1].width, img[1].height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

	//button2
	//
	glBindTexture(GL_TEXTURE_2D, gl.button2);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[2].width, img[2].height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

	//button3
	//
	glBindTexture(GL_TEXTURE_2D, gl.button3);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[3].width, img[3].height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

	//button4
	//
	glBindTexture(GL_TEXTURE_2D, gl.button4);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[4].width, img[4].height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

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

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//

	if (e->xbutton.x > gl.xres/3 
			&& e->xbutton.x < (2*gl.xres/3)
			&& e->xbutton.y > (4.6*gl.yres/8)
			&& e->xbutton.y < (5.6*gl.yres/8)) {
		gl.hoverButton = 1;
	} else {
		gl.hoverButton = 0;
	}

	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			if (e->xbutton.x > (gl.xres/3)
					&& e->xbutton.x < (2*gl.xres/3)
					&& e->xbutton.y > (4.6*gl.yres/8)
					&& e->xbutton.y < (5.6*gl.yres/8)) {
				gl.showMenu = 0;
			}
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
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
			cout << g.players[0]->ship->pos[0] << ", " << g.players[0]->ship->pos[1] << endl;
			cout << g.players[1]->ship->pos[0] << ", " << g.players[1]->ship->pos[1] << endl;
			break;
		case XK_r:
			myPhysWorld->printArr();
			break;
		case XK_e:
			g.players[1]->ship->pos[0] = (float)(rand() % gl.xres) ;
			g.players[1]->ship->pos[1] = (float)(rand() % gl.yres) ;
			g.players[0]->ship->pos[0] = (float)(rand() % gl.xres) ;
			g.players[0]->ship->pos[1] = (float)(rand() % gl.yres) ;
			g.players[1]->ship->color[0] = (float)(rand() % 100) / 100;
			g.players[1]->ship->color[1] = (float)(rand() % 100) / 100;
			g.players[1]->ship->color[2] = (float)(rand() % 100) / 100;
			g.players[0]->ship->color[0] = (float)(rand() % 100) / 100;
			g.players[0]->ship->color[1] = (float)(rand() % 100) / 100;
			g.players[0]->ship->color[2] = (float)(rand() % 100) / 100;
			break;
		case XK_Down:
			break;
		case XK_q:

			break;
		case XK_m:
			gl.showMenu ^= 1;
			break;
		case XK_p:
			gl.isPaused = !gl.isPaused;
			break;
		case XK_1:
			gl.wepNum +=1;
			gl.wepNum = gl.wepNum % 7;
			cout << gl.wepNum << endl;
			g.players[0]->setWeapon(gl.wepNum);
			break;
		case XK_2:
			gl.debug += 1;
			gl.debug = gl.debug % 2;
			break;
	}
	return 0;
}
void physics()
{
	//	Flt d0,d1,dist;
	if(!gl.isPaused)
	{

		myBoxWorld->update();
		for(int i = 0; i < 2; i++)
		{
			if(g.players[0]->lives == 0)
			{
				args[0] = (char *)"0";
				execvp(path,args);
				//exit(1);
			}
			if(g.players[1]->lives == 0)
			{
				args[0] = (char *)"1";;
				execvp(path,args);
				//exit(1);
			}
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

		/*
		   for(int i = 0; i < 2; i++){
		   g.players[i]->ship->Object::testCollision();
		   }
		   */
		for(int i = 0; i < myPhysWorld->arrSize;i++)
		{
			if(myPhysWorld->objectArr[i] != NULL)
				myPhysWorld->objectArr[i]->testCollision();
		}
	}


	//how we would implement gamemodes
	/*
	   arr gameMode[3] = {points, elims}

	   gameMode->update();

	   if(gameMode->winCondition)
	   {
	   done = false;

	   }
	   */

}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Rect r;
	r.bot = (gl.yres/2) - 50;
	r.left = gl.xres/2;
	r.center = 1;


	//--draws background
	glColor3f(128/255.0,128/255.0,128/255.0);
	glBegin(GL_POLYGON);
	glVertex2i(0,0);
	glVertex2i(gl.xres,0);
	glVertex2i(gl.xres,gl.yres);
	glVertex2i(0,gl.yres);
	glEnd();
	//--
	myMapHandler->currentMap->render();
	g.myHud->render();

	glColor3ub(0,0,0);
	/*
	   glPushMatrix();
	   glTranslatef((gl.xres/2)-50, (gl.yres/2)-50, 1);
	   glBegin(GL_POLYGON);
	   glVertex2f(0.0f, 100.0f);
	   glVertex2f(  0.0f,  0.0f);
	   glVertex2f(  100.0f,  0.0f);
	   glVertex2f(100.0f, 100.0f);
	   glEnd();
	   glPopMatrix();
	   */
	//g.players[0]->ship->drawHitbox();
	//g.players[1]->ship->drawHitbox();

	//Draw the ship
	for(int i =0; i < 2; i++)
	{

		float tempX = g.players[i]->ship->pos[0];
		float tempY = g.players[i]->ship->pos[1];
		//g.players[i]->ship->drawHitbox();
		glColor3fv(g.players[i]->ship->color);
		glPushMatrix();
		glTranslatef(tempX, tempY, 1);
		glRotatef(g.players[i]->ship->angle, 0.0f, 0.0f, 1.0f);

		/*
		   glBegin(GL_TRIANGLES);
		   glVertex2f(-12.0f, -10.0f);
		   glVertex2f(  0.0f,  20.0f);
		   glVertex2f(  0.0f,  -6.0f);
		   glVertex2f(  0.0f,  -6.0f);
		   glVertex2f(  0.0f,  20.0f);
		   glVertex2f( 12.0f, -10.0f);
		   */

		int w1 = 5;
		int h1 = 7;
		int w2 = 10;
		int h2 = 10;
		glColor3ub(0,0,0);
		glBegin(GL_POLYGON);
		glVertex2f(-w1 - 10,-h1+5);
		glVertex2f(-w1 - 10,h1+5);
		glVertex2f(w1 - 10,h1+5);
		glVertex2f(w1 - 10,-h1+5);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(-w1 + 10,-h1+5);
		glVertex2f(-w1 + 10,h1+5);
		glVertex2f(w1 + 10,h1+5);
		glVertex2f(w1 + 10,-h1+5);
		glEnd();
		glColor3fv(g.players[i]->ship->color);
		glBegin(GL_POLYGON);
		glVertex2f(-w2,-h2);
		glVertex2f(-w2,h2);
		glVertex2f(w2,h2);
		glVertex2f(w2,-h2);
		glEnd();
		glPopMatrix();
		/*
		   glVertex2f(-w1,-h1);
		   glVertex2f(-w1,h1);
		   glVertex2f(w1,h1);
		   glVertex2f(w1,-h1);
		   */
	}


	// cout << "Drawing Weapons" << endl;
	for(int count =0;count < 2;count++)
	{ 
		g.players[count]->currentWeapon->render();
		g.players[count]->currentPassive->render();
	}

	// render Map
	//render itemBox (testing, will remove)
	//g.myBox->render();


	for(int i = 0; i < BOXCAP;i++)
	{
		if(myBoxWorld->currentBoxes[i] != NULL)
		{
			myBoxWorld->currentBoxes[i]->render();
		}
	}




	if(gl.isPaused)
	{
		//glBlendFunc  (GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
		//glEnable     (GL_BLEND);
		//glClearColor(0.0,0.0,0.0,0.0);
		//glEnable     (GL_COLOR_MATERIAL);
		glPushMatrix();
		glColor3ub(0,0,0);
		glBegin(GL_POLYGON);
		int w = 150;
		int h = 100;
		glVertex2f((gl.xres/2) + w,(gl.yres/2) + h);
		glVertex2f((gl.xres/2) + w,(gl.yres/2) - h);
		glVertex2f((gl.xres/2) - w,(gl.yres/2) - h);
		glVertex2f((gl.xres/2) - w,(gl.yres/2) + h);
		glEnd();
		ggprint8b(&r, 16, 0x00ff0000, "Player 1 - w a s d space");
		ggprint8b(&r, 16, 0x00ff0000, "Player 2 - up down left right enter");
		ggprint8b(&r, 16, 0x00ff0000, "PAUSE : 'P'");
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



