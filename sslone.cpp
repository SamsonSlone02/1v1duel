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

Weapon::Weapon(int in_rate = 10, Player * in_parent = NULL)
{
	rate = in_rate;
	parent = in_parent;
	cout << "weap" <<parent << endl;
}
void Weapon::fireWeapon(){}
void Weapon::updatePosition(){}
void Weapon::draw(){}
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
	delete [] barr;
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

			b->color[0] = 28.0f/255.0f;
			b->color[1] = 200.0f/255.0f;
			b->color[2] = 90.0f/255.0f;
			nbullets++;
		}


	}



}

void Boomerang::updatePosition()
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

		float scaler = 13;
		scaler = scaler/ttl;

		b->vel[0] = scaler * (cos(theta)* cos(theta - b->initRot) - sin(theta) * sin(theta - b->initRot));
		b->vel[1] = scaler * (cos(theta)* sin(theta - b->initRot) + sin(theta) * cos(theta - b->initRot));

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

void Boomerang::draw()
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

Player::Player()
{

}
void Player::test()
{
	std::cout << "1" << endl;
}
void Player::setKeys(int in_up, int in_down, int in_left, int in_right, int in_attack)
{
	up = in_up;
	down = in_down;
	left = in_left;
	right = in_right;
	attack = in_attack;
}
void Player::getWeapon()
{

}
void Player::setWeapon()
{

}

