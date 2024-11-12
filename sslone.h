#ifndef SSLONE_H
#define SSLONE_H

#include <ctime>
#include<iostream>
#include <unistd.h>
#include<cstring>
#include <GL/glx.h>
#include<cmath>
#include "fonts.h"
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
const int MAX_BULLETS = 10;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
//constants
class Global {
    public:
	int xres, yres;
	char keys[65536];
	bool isPaused;
	Global();
};
class PhysWorld;
class Object
{
    public:
    float h,w;
    PhysWorld * member;
    float pos[2];
    float vel[2];
    
    enum Type {NON,WALL,SHIP,BULLET};
    Type objectType;
    //things that collision should ignore
    Object * filter[10];
    int filterSize;

    Object(PhysWorld * in_member);
    ~Object();
    bool testCollision();
    void addFilter(Object *in_object);
    void remFilter(Object *in_object);
    virtual void handleCollision(Object * in_object);
    void drawHitbox();

};
class PhysWorld
{
    private:
    public:
        Object * objectArr[50];
        int arrSize;
        PhysWorld();
        ~PhysWorld();
        bool addObject(Object * in_object);
	bool remObject(Object * in_object);
	void printArr();

};



class Ship: public Object{
    public:
    //Vec pos;
	Vec dir;
	Vec vel;
	Vec acc;
	float drawPos[2];
	Player * parent;
	float angle;
	float color[3];
	Ship(PhysWorld * in_member,Player * in_parent);
	void setColor(int r, int g, int b);
	void handleCollision(Object * in_object);
};
class Player;
class Passive
{
    private:
	const char * type = "None";

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
	const char * type = "Shield";
    public:
	float pos;
	float angle;

	float shield1Angle;
	float shield2Angle;
	float shield3Angle;

	float shield1[2];
	float shield2[2];
	float shield3[2];

	Shield(Player * in_parent);
	void update();
	void render();
	~Shield();
};
class Speed: public Passive
{

    private:
	const char * type = "Speed";
    public:
	Speed(Player * in_parent);
	void update();
	void render();
	~Speed();
};


class Weapon;
class Bullet : public Object{
    public:
	//Vec pos;
	Vec vel;
	Flt angle;
	int yBounce;
	int xBounce;
	float color[3];
	float initX;
	float initY;
	float initRot;
	struct timespec time;
	//PhysWorld * member;
	Bullet(PhysWorld * in_member);
	void handleCollision(Object * in_object);
};

class Player
{

    private:
	int health;
	double speed;	
	double rSpeed;
    public:
	int up,down,left,right,attack;
	bool isThrust;
	Passive * currentPassive;
	Weapon * currentWeapon;		
	Ship * ship;
	void test();
	Player(int in_health,double in_speed, double in_rSpeed,PhysWorld * in_member);
	~Player();
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
	const char * type = "Weapon";
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
	const char * type = "boomerang";

    public:
    PhysWorld * member;
    Boomerang(int in_rate, Player * in_parent, PhysWorld * in_member);
	~Boomerang();
	std::string getWeapon();
	void fireWeapon();
	void physics();
	void render();

};
class Sniper: public Weapon
{

    private:
	float startPosL[2];
	float startPosR[2];
	float endPosL[2];
	float endPosR[2];

	const char * type = "Sniper";
    public:
	Sniper(int in_rate, Player * in_parent);
	std::string getWeapon();
	void fireWeapon();
	void physics();
	void render();

};

#endif //SSLONE_H
