#include "sslone.h"
using namespace std;



Global::Global() {
	xres = 640;
	yres = 480;
	memset(keys, 0, 65536);
};


Ship::Ship()
{
	Global gl;
	//added rand to have every new player spawn randomly within the region of the arena
	pos[0] = (float)(rand() % gl.xres);
	pos[1] = (float)(rand() % gl.yres);
	pos[2] = 0.0f;
	VecZero(dir);
	VecZero(vel);
	VecZero(acc);
	angle = 0.0;
	//generates a random color on every ship initialization
	color[0] = (float)(rand() % 100) / 100;
	color[1] = (float)(rand() % 100) / 100;
	color[2] = (float)(rand() % 100) / 100;
}
void Ship::setColor(int r, int g,int b)
{
	color[0] = r / 255.0f;
	color[1] = g / 255.0f;
	color[2] = b / 255.0f;
}
Bullet::Bullet(Weapon * in_bulletParent = NULL)
{
	this->yBounce = 1;
	this->xBounce = 1;
	angle = 0;
	bulletParent = in_bulletParent;
	if(bulletParent != NULL)
	{
		initX = this->bulletParent->parent->ship->pos[0];	
		initY = this->bulletParent->parent->ship->pos[1];	
	}
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	cout << "TIME ADDRESS ";
	cout << &time << endl;
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
}
Shield::~Shield(){}
void Shield::update(){}
void Shield::render(){}

Speed::Speed(Player * in_parent = NULL){
	Passive();
	this->parent = in_parent;
	parent->setSpeed(3.8);
}
Speed::~Speed(){}
void Speed::update(){}
void Speed::render(){}


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
Boomerang::Boomerang(int in_rate = 10, Player * in_parent = NULL)
{ 
	Weapon(in_rate, in_parent);
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
			Bullet *b = &barr[nbullets];
			//shoot a bullet...
			cout << &b << endl;		
			timeCopy(&b->time, &bt);
			b->bulletParent = this;
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


		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		double ttl = 3.6;
		if (ts > ttl) {
			//time to delete the bullet.
			b->xBounce = 1;
			b->yBounce = 1;
			memcpy(&barr[i], &barr[nbullets-1],
					sizeof(Bullet));
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

	cout << "fire" << endl;

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
	glVertex2f(startPosL[0],startPosL[1]);
	glVertex2f(endPosL[0],endPosL[1]);
	glVertex2f(startPosR[0],startPosR[1]);
	glVertex2f(endPosR[0],endPosR[1]);
	glEnd();

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
	this->up = in_up;
	this->attack = in_attack;
	this->down = in_down;
	this->left = in_left;
	this->right = in_right;
}

string Player::getWeapon()
{
	return this->currentWeapon->getWeapon();
}
void Player::setWeapon()
{
	//remove current weapon
	delete this->currentWeapon;
	//set empty weapon
	//unfinised, will eventually figure out how i want weapons to be assigned via arguments
	this->currentWeapon = new Weapon(10,this);

}

void Player::setRSpeed(double in_rSpeed)
{
	this->rSpeed = in_rSpeed;

}
void Player::setSpeed(double in_speed)
{

	this->speed = in_speed;
}
void Player::setHealth(int in_health)
{
	this->health = in_health;
}

Player::Player(int in_health, double in_speed, double in_rSpeed)
{
	this->setSpeed(in_speed);
	this->setRSpeed(in_rSpeed);

	this->health = in_health;
	this->currentWeapon = new Weapon(10,this);
	this->currentPassive = new Passive(this);
	this->ship = new Ship();
}
