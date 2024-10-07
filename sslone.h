#include <ctime>
#include<iostream>
#include<cstring>
#include <GL/glx.h>
#include<cmath>
class Player;
class Bullet;
class Global;

typedef float Vec[3];
typedef float Flt;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]


//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
//constants
class Global {
	public:
		int xres, yres;
		char keys[65536];
		Global();
};


class Ship {
	public:
		Vec pos;
		Vec dir;
		Vec vel;
		Vec acc;
		float angle;
		float color[3];
		Ship();
		void setColor(int r, int g, int b);
};
class Player;
class Passive
{
	private:
		std::string type = "None";

	public:
		Player * parent;
		Passive(Player * in_parent);
		~Passive();
		virtual void update();
		virtual void render();

};

class Shield: public Passive
{

	private:
		std::string type = "Shield";
	public:
		Shield(Player * in_parent);
		void update();
		void render();
		~Shield();
};
class Speed: public Passive
{

	private:
		std::string type = "Speed";
	public:
		Speed(Player * in_parent);
		void update();
		void render();
		~Speed();
};


class Weapon;
class Bullet {
	public:
		Vec pos;
		Vec vel;
		Flt angle;
		int yBounce;
		int xBounce;
		float color[3];
		float initX;
		float initY;
		float initRot;
		struct timespec time;
		Weapon * bulletParent;
	public:
		Bullet(Weapon * in_bulletParent);
};

class Player
{

	private:
		int health;
		double speed;	
		double rSpeed;
	public:
		int up,down,left,right,attack;
		Passive * currentPassive;
		Weapon * currentWeapon;		
		Ship * ship;
		void test();
		Player(int in_health,double in_speed, double in_rSpeed);
		void setKeys(int in_up, int in_down, int in_left, int in_right, int in_attack);
		std::string getWeapon();
		void setWeapon();

		double getRSpeed();
		void setRSpeed(double in_rSpeed);

		double getSpeed();
		void setSpeed(double in_speed);

		int getHealth();
		void setHealth(int in_health);
		//void setPassive();
};

class Weapon
{

	private:

		int rate;
		std::string type = "Weapon";
	public:
		Weapon(int in_rate, Player * in_parent);
		virtual ~Weapon();
		Player * parent;
		virtual std:: string getWeapon();
		virtual void fireWeapon();
		virtual void physics();
		virtual void render();

};


class Boomerang: public Weapon
{
	private:
		Vec pos;
		Vec vel;
		float color[3];
		Bullet * barr;
		struct timespec bulletTimer;
		int nbullets;
		std::string type = "boomerang";

	public:
		Boomerang(int in_rate, Player * in_parent);
		~Boomerang();
		std::string getWeapon();
		void fireWeapon();
		void physics();
		void render();

};

class Bomb: public Weapon
{
	private:
		std::string type = "Bomb";
	public:
		std::string getWeapon();
};

class Sniper: public Weapon
{

	private:
		float startPosL[2];
		float startPosR[2];
		float endPosL[2];
		float endPosR[2];

		std::string type = "Sniper";
	public:
		Sniper(int in_rate, Player * in_parent);
		std::string getWeapon();
		void fireWeapon();
		void physics();
		void render();

};
