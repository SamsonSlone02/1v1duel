#include "sslone.h"
using namespace std;

Object::Object(PhysWorld * in_member = NULL){
	member = in_member;
}
Object::~Object(){}

bool Object::testCollision(){ 
    for(int i = 0; i < member->arrSize;i++)
    {
           
        if(this == member->objectArr[i] || member->objectArr[i] == NULL)
        {
            
            continue;
        }
        if(           this->pos[0] < member->objectArr[i]->pos[0] + member->objectArr[i]->w
                   && this->pos[0] + this->w > member->objectArr[i]->pos[0]
                   && this->pos[1] < member->objectArr[i]->pos[1] + member->objectArr[i]->w
                   && this->pos[1] + this->w > member->objectArr[i]->pos[1]
                )
        {
        cout << "collision" << endl;
        }
    
    }
    return true;
}

PhysWorld::PhysWorld(){
	arrSize = 500;
	for(int i = 0; i < arrSize;i++)
	{
		objectArr[i] = NULL;
	}
}

PhysWorld::~PhysWorld(){}
bool PhysWorld::addObject(Object * in_object){

	for(int i = 0; i < arrSize;i++)
	{

		if(objectArr[i] == NULL)
		{
			objectArr[i] = in_object;
			return true;

		}
	}
	return false;
}
bool PhysWorld::remObject(Object * in_object) {

	for(int i = 0; i < arrSize;i++)
	{
		if(objectArr[i] == in_object)
		{
			//delete objectArr[i];
			objectArr[i] = NULL;
			return true;
		}

	}
	return false;

}

void PhysWorld::printArr()
{
	std::cout << "**********************" << std::endl;
	for(int i = 0; i < 10;i++)
	{
		std::cout << "Slot: " << i << ", Address: " << objectArr[i] << std::endl;

	}
	std::cout << "**********************" << std::endl;

}


void Ship::setColor(int r, int g,int b)
{
	color[0] = r / 255.0f;
	color[1] = g / 255.0f;
	color[2] = b / 255.0f;
}
Bullet::Bullet(PhysWorld * in_member = NULL)
{
    h = 5;
    w = 5;

	this->yBounce = 1;
	this->xBounce = 1;
	angle = 0;
	member = in_member;
	if(member != NULL)
	{
		//initX = this->bulletParent->parent->ship->pos[0];	
		//initY = this->bulletParent->parent->ship->pos[1];	
	}
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	//cout << "TIME ADDRESS ";
	//cout << &time << endl;
}
//PASSIVE ABILITIES
Passive::Passive(Player * in_parent= NULL)
{
	parent = in_parent;
}
Passive::~Passive(){}
void Passive::update(){}
void Passive::render(){}

Shield::Shield(Player * in_parent= NULL){
	Passive();
	this->parent = in_parent;
	parent->setHealth(parent->getHealth() + 1);

	//float pos;

	//shieldAngle = 0;

	shield1[0] = parent->ship->pos[0];
	shield1[1] = parent->ship->pos[1];
	shield2[0] = parent->ship->pos[0];
	shield2[1] = parent->ship->pos[1];
	shield3[0] = parent->ship->pos[0];
	shield3[1] = parent->ship->pos[1];


	angle = parent->ship->angle;


}
Shield::~Shield(){

}

void Shield::update(){

	float parentPos[2];
	parentPos[0] = parent->ship->pos[0];
	parentPos[1] = parent->ship->pos[1];

	angle += 2.5;
	angle = fmod(angle,360.0f);	

	float rad = ((angle+90.0) / 360.0f) * PI * 2.0;

	float xdir = cos(rad +  (PI)/2);
	float ydir = sin(rad + (PI)/2);

	float r = 25;

	xdir = cos(rad + (11 * PI) /6);
	ydir = sin(rad + (11 * PI) /6);
	shield1Angle = (atan2(ydir,xdir) * (180/PI)) - 90;
	shield1[0] = parentPos[0] + r * xdir;
	shield1[1] = parentPos[1] + r * ydir;

	xdir = cos(rad + (7 * PI) /6);
	ydir = sin(rad + (7 * PI) /6);
	shield2Angle = (atan2(ydir,xdir) * (180/PI))-90;
	shield2[0] = parentPos[0] + r * xdir;
	shield2[1] = parentPos[1] + r * ydir;

	xdir = cos(rad + PI/2);
	ydir = sin(rad + PI/2);
	shield3Angle = (atan2(ydir,xdir) * (180/PI)) - 90;
	shield3[0] = parentPos[0] + r * xdir;
	shield3[1] = parentPos[1] + r * ydir;

}

void Shield::render(){

	
	glPushMatrix();
	glTranslatef(shield1[0], shield1[1], 1);
	glRotatef(shield1Angle, 0.0f, 0.0f, 1.0f);

	glScalef(2.4, 1.4, 1.4); 
	glBegin(GL_TRIANGLES);
	glColor3ub(166, 218, 247);
	glVertex2f(-5,0);
	glVertex2f(-3,3);
	glVertex2f(-3,0);

	glVertex2f(5,0);
	glVertex2f(3,3);
	glVertex2f(3,0);

	glVertex2f(-3,3);
	glVertex2f(3,0);
	glVertex2f(-3,0);

	glVertex2f(-3,3);
	glVertex2f(3,3);
	glVertex2f(3,0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(shield2[0], shield2[1], 1);
	glRotatef(shield2Angle, 0.0f, 0.0f, 1.0f);

	glScalef(2.4, 1.4, 1.4); 
	glBegin(GL_TRIANGLES);

	glColor3ub(166, 218, 247);
	glVertex2f(-5,0);
	glVertex2f(-3,3);
	glVertex2f(-3,0);

	glVertex2f(5,0);
	glVertex2f(3,3);
	glVertex2f(3,0);

	glVertex2f(-3,3);
	glVertex2f(3,0);
	glVertex2f(-3,0);

	glVertex2f(-3,3);
	glVertex2f(3,3);
	glVertex2f(3,0);

	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(shield3[0], shield3[1], 1);
	glRotatef(shield3Angle, 0.0f, 0.0f, 1.0f);

	glScalef(2.4, 1.4, 1.4); 
	glBegin(GL_TRIANGLES);
	glColor3ub(166, 218, 247);
	glVertex2f(-5,0);
	glVertex2f(-3,3);
	glVertex2f(-3,0);

	glVertex2f(5,0);
	glVertex2f(3,3);
	glVertex2f(3,0);

	glVertex2f(-3,3);
	glVertex2f(3,0);
	glVertex2f(-3,0);

	glVertex2f(-3,3);
	glVertex2f(3,3);
	glVertex2f(3,0);

	glEnd();
	glPopMatrix();
	
	Rect r;
	r.bot = parent->ship->pos[1] - 25;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

}

Speed::Speed(Player * in_parent = NULL){
	Passive();
	this->parent = in_parent;
	parent->setSpeed(3.8);
}
Speed::~Speed(){}
void Speed::update(){}
void Speed::render(){


	if (parent->isThrust) {
		//draw thrust
		Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (int j=0; j<16; j++) {
			xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
			ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
			r = rnd()*40.0+40.0;
			xe = -xdir * r + rnd() * 18.0 - 9.0;
			ye = -ydir * r + rnd() * 18.0 - 9.0;
			glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
			glVertex2f(parent->ship->pos[0]+xs,parent->ship->pos[1]+ys);
			glVertex2f(parent->ship->pos[0]+xe,parent->ship->pos[1]+ye);
		}

		glEnd();
	}
	Rect r;
	r.bot = parent->ship->pos[1] - 25;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

}


Weapon::Weapon(int in_rate = 10, Player * in_parent = NULL)
{
	rate = in_rate;
	parent = in_parent;
	cout << "weap" <<parent << endl;

}
Weapon::~Weapon(){}
string Weapon::getWeapon()
{
	return this->type;
}
void Weapon::fireWeapon(){}
void Weapon::physics(){}
void Weapon::render(){}
Boomerang::Boomerang(int in_rate = 10, Player * in_parent = NULL, PhysWorld * in_member = NULL)
{ 
	Weapon(in_rate, in_parent);
	member = in_member;
	this->parent = in_parent;
	nbullets = 0;

	this->barr = new Bullet[MAX_BULLETS];


	struct timespec bulletTimer;
	clock_gettime(CLOCK_REALTIME, &bulletTimer);

}
Boomerang::~Boomerang()
{
	cout << "barr del!"<< endl;
	delete [] barr;
}
string Boomerang::getWeapon()
{
	return this->type;
}

void Boomerang::fireWeapon()
{
	//a little time between each bullet
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&bulletTimer, &bt);

	if (ts > 1.5) {
		timeCopy(&bulletTimer, &bt);
		if (nbullets < MAX_BULLETS) {
			Bullet * b= new Bullet(member);
			b = &barr[nbullets];
            b->member = member;
			member->addObject(b);
			std::cout << "after adding" << std::endl;
			member->printArr();
			//shoot a bullet...
			//cout << &b << endl;		
			timeCopy(&b->time, &bt);
			//b->bulletParent = this;
			b-> initX = parent->ship->pos[0];
			b-> initY = parent->ship->pos[1];
			b->pos[0] = parent->ship->pos[0];
			b->pos[1] = parent->ship->pos[1];
			b->vel[0] = parent->ship->vel[0];
			b->vel[1] = parent->ship->vel[1];

			//convert ship angle to radians
			Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
			b->initRot = rad;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			b->pos[0] += xdir*20.0f;
			b->pos[1] += ydir*20.0f;

			b->color[0] = 0.0f/255.0f;
			b->color[1] = 0.0f/255.0f;
			b->color[2] = 0.0f/255.0f;

			b->xBounce = 1;
			b->yBounce = 1;
			nbullets++;
		}


	}



}

void Boomerang::physics()
{

	Global gl;
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i = 0;
	while (i < nbullets) {
		Bullet *b = &barr[i];

        b->testCollision();
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		double ttl = 3.6;
		if (ts > ttl) {
			//time to delete the bullet.
			b->xBounce = 1;
			b->yBounce = 1;
			member->remObject(b);
			member->addObject(&barr[i]);    
			member->remObject(&barr[nbullets-1]);
			barr[i] = barr[nbullets-1];

			std::cout << "after removal" << std::endl;
			member->printArr();

			nbullets--;
			//do not increment i.
			continue;
		}

		b->angle = fmod(ts/ttl,(PI));
		b->angle = (b->angle * (PI));

		Flt theta = (b->angle + b->initRot - PI/4);

		float scalar = 13;
		scalar = scalar/ttl;

		b->vel[0] = scalar * (cos(theta)* cos(theta - b->initRot) - sin(theta) * sin(theta - b->initRot));
		b->vel[1] = scalar * (cos(theta)* sin(theta - b->initRot) + sin(theta) * cos(theta - b->initRot));

		//wall collision
		if(b->pos[0] < 0.0)
		{
			b->xBounce *= -1;
			b->pos[0] = 5;
		}
		if(b->pos[0] > (float)gl.xres)
		{
			b->xBounce *= -1;

			b->pos[0] =(float)gl.xres - 5;
		}

		if(b->pos[1] < 0.0)
		{
			b->yBounce *= -1;
			b->pos[1] = 5;
		}
		if(b->pos[1] > (float)gl.yres)
		{
			b->yBounce *= -1;
			b->pos[1] =(float)gl.yres - 5;

		}

		//updates bullet position
		b->pos[1] += b->vel[1] * b->yBounce;
		b->pos[0] += b->vel[0] * b->xBounce;
		++i;
	}


}

void Boomerang::render()
{

	for (int i=0; i< nbullets; i++) {
		Bullet *b = &barr[i];
		glColor3f(b->color[0],b->color[1], b->color[2]);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(b->color[0], b->color[1], b->color[2]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	}
	Rect r;
	r.bot = parent->ship->pos[1] - 35;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

}
Sniper::Sniper(int in_rate = 10, Player * in_parent = NULL)
{
	Weapon(in_rate, in_parent);
	this->parent = in_parent;

}
string Sniper::getWeapon()
{
	return this->type;
}
void Sniper::fireWeapon()
{
	Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
	float xdir = cos(rad - PI/2);
	float ydir = sin(rad - PI/2);
	float r = 10;

	//hitscan should be a single frame operation, all logic and collision detection should be contained within this single function call
	startPosL[0] = parent->ship->pos[0] + r *(xdir);
	startPosL[1] = parent->ship->pos[1] + r *(ydir);

	startPosR[0] = parent->ship->pos[0] - r *(xdir);
	startPosR[1] = parent->ship->pos[1] - r *(ydir);

	xdir = cos(rad);
	ydir = sin(rad);

	endPosL[0] = startPosL[0] + (1000 * xdir);
	endPosL[1] = startPosL[1] + (1000 * ydir);

	endPosR[0] = startPosR[0] + (1000 * xdir);
	endPosR[1] = startPosR[1] + (1000 * ydir);

};
void Sniper::physics(){};
void Sniper::render()
{
	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2f(startPosL[0],startPosL[1]);
	glVertex2f(endPosL[0],endPosL[1]);
	glVertex2f(startPosR[0],startPosR[1]);
	glVertex2f(endPosR[0],endPosR[1]);
	glEnd();
	
	Rect r;
	r.bot = parent->ship->pos[1] - 35;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

};


string Bomb::getWeapon()
{
	return this->type;
}

double Player::getRSpeed()
{
	return rSpeed;
}
double Player::getSpeed()
{
	return speed;
}

int Player::getHealth()
{
	return health;
}
void Player::test()
{
	std::cout << "1" << endl;
}
void Player::setKeys(int in_up, int in_down, int in_left, int in_right, int in_attack)
{
	up = in_up;
	attack = in_attack;
	down = in_down;
	left = in_left;
	right = in_right;
}

string Player::getWeapon()
{
	return currentWeapon->getWeapon();
}
void Player::setWeapon()
{
	//remove current weapon
	delete this->currentWeapon;
	//set empty weapon
	//unfinised, will eventually figure out how i want weapons to be assigned via arguments
	currentWeapon = new Weapon(10,this);

}

void Player::setRSpeed(double in_rSpeed)
{
	rSpeed = in_rSpeed;

}
void Player::setSpeed(double in_speed)
{

	speed = in_speed;
}
void Player::setHealth(int in_health)
{
	health = in_health;
}

Player::Player(int in_health, double in_speed, double in_rSpeed, PhysWorld * in_member)
{
	setSpeed(in_speed);
	setRSpeed(in_rSpeed);
    PhysWorld * member = in_member;
	isThrust = false;
	health = in_health;
	currentWeapon = new Weapon(10,this);
	currentPassive = new Passive(this);
	ship = new Ship(member);
}
