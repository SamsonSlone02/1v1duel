#include "sslone.h"
#include "rrivasnavarr.h"
using namespace std;

Object::Object(PhysWorld * in_member = NULL)
{
	setColor(0,0,0);
	member = in_member;
	filterSize = 20;
	//addFilter();
}

void Object::drawHitbox()
{
	glPushMatrix();
	glColor3ub(0,0,0);
        glTranslatef(pos[0], pos[1] , 1);
        glBegin(GL_POLYGON);
        glVertex2f(w/2,h/2);
        glVertex2f(w/2, -h/2);
        glVertex2f(  -w/2,  -h/2);
        glVertex2f(-w/2,h/2 );
        glEnd();
        glPopMatrix();
}

Object::~Object(){}

bool Object::testCollision()
{ 
	bool skipFlag;
	bool didCollide = false;

	for (int i = 0; i < member->arrSize;i++) {

		skipFlag = false;
		//if colliding with itself or comparing against an empty object
		if (this == member->objectArr[i] || member->objectArr[i] == NULL) {
			continue;
		}

		int objectW,objectH;
		objectW = member->objectArr[i]->w;
		objectH = member->objectArr[i]->h;
		//AABB collision detection
		if ( 
			this->pos[0] - w/2 < member->objectArr[i]->pos[0] + objectW/2 &&
			this->pos[0] + w/2 > member->objectArr[i]->pos[0] - objectW/2 &&
			this->pos[1] - h/2 < member->objectArr[i]->pos[1] + objectH/2 &&
			this->pos[1] + h/2 > member->objectArr[i]->pos[1] - objectH/2

		) {


			for (int j = 0; j < filterSize;j++) {
				if (member->objectArr[i] == filter[j] || member->objectArr[i]->objectType == filterType[j]) {
					//cout << "ignoring current collision" << endl;
					//item detected in filter, skipping collision check.
					skipFlag = true;
					break;
				}
			}


			if (!skipFlag) {
				//if you got here, that means a valid collision was detected.
				//run the objects collision handling function here.
				//ex. this->handleCollision(object->getType)
				handleCollision(member->objectArr[i]);
				if (member->objectArr[i] != NULL)
					member->objectArr[i]->handleCollision(this);
				didCollide = true;	
			}
		}

	}
	return didCollide;
}
void Object::addFilter(Object * in_object)
{
	for (int i = 0; i < filterSize;i++) {
		if (filter[i] == NULL) {
			filter[i] = in_object;
			//cout << "succesfully added" << endl;
			return;
		}
	}	
}

void Object::remFilter(Object * in_object)
{
	for (int i = 0; i < filterSize; i++) {
		if (filter[i] == in_object) {
			filter[i] = NULL;
		}
	}

}
void Object::clearFilter()
{
	for (int i = 0; i < filterSize; i++) {
		filter[i] = NULL;
	}
		

}

void Object::addFilterType(Type in_object)
{
	for (int i = 0; i < filterSize;i++) {
		if (filterType[i] == NON) {
			filterType[i] = in_object;
			//cout << "succesfully added" << endl;
			return;
		}
	}	
}

void Object::remFilterType(Type in_object)
{
	for (int i = 0; i < filterSize; i++) {
		if (filterType[i] == in_object) {
			filterType[i] = NON;
		}
	}

}

void Object::clearFilterType()
{
	for (int i = 0; i < filterSize; i++) {
		filterType[i] = NON;
	}
		

}

void Object::handleCollision(Object * in_object)
{
	//cout << in_object << endl;
	in_object = in_object;
}

void Object::setColor(float r,float g,float b)
{

	color[0] = r;
	color[1] = g;
	color[2] = b;
}


PhysWorld::PhysWorld()
{
	
	arrSize = 50;
	for (int i = 0; i < arrSize;i++) {
		objectArr[i] = NULL;
	}
}

PhysWorld::~PhysWorld(){}
bool PhysWorld::addObject(Object * in_object)
{

	for (int i = 0; i < arrSize;i++) {

		if (objectArr[i] == NULL) {
			objectArr[i] = in_object;
			return true;
		}
	}
	return false;
}
bool PhysWorld::remObject(Object * in_object) 
{

	for (int i = 0; i < arrSize;i++) {
		if (objectArr[i] == in_object) {
			//delete objectArr[i];
			objectArr[i] = NULL;
			return true;
		}

	}
	//cout << "failed to rem" << endl;
	return false;

}

void PhysWorld::printArr()
{
	const char * names[] = {"NON","WALL","SHIP","BULLET","ITEMBOX","BOMB"};
	std::cout << "**********************" << std::endl;
	for (int i = 0; i < 50;i++) {

		std::cout << "Slot: " << i << ", Address: " << objectArr[i];
		if (objectArr[i] != NULL) {

			int x = objectArr[i]->pos[0];
			int y = objectArr[i]->pos[1];
			std::cout << ", Type: " << names[objectArr[i]->objectType] << ", X: " << x <<  ", Y: " << y << " ";
		}
		else {
			cout << ", Type: NOT ASSIGNED ";
		}
		int debug  = 1;
		if (debug == 1)
		{	cout << "\nSlot: " << i << " ITEMS BEING FILTERED: ";

			if(objectArr[i] != NULL)
			{
				for (int j = 0; j < objectArr[i]->filterSize;j++) {
					cout << objectArr[i]->filter[j] << ", ";

				}
			}
			cout << endl;

		}
		cout << endl;
	}
	std::cout << "**********************" << std::endl;

}
void Ship::setColor(int r, int g,int b)
{
	color[0] = r / 255.0f;
	color[1] = g / 255.0f;
	color[2] = b / 255.0f;
}
void Ship::handleCollision(Object * in_object)
{
	switch(in_object->objectType)
	{

		case NON:
			break;
		case SHIP:
			//cout << "ship detected" << endl;
			break;
		case BULLET:
			srand(time(NULL));
			//cout << "bullet detected" << endl;
			cout << parent->getHealth() << endl;
			parent->setHealth(parent->getHealth() - 1);
			if (parent->getHealth() <1 ) {
				/*
				   color[0] = 255;
				   color[1] = 0;
				   color[2] = 0;

				   delete parent->currentWeapon;
				   delete parent->currentPassive;

				   parent->currentPassive = new Passive(parent);
				   parent->currentWeapon = new Weapon(3,parent);	


				   pos[0] = (float)(rand() % gl.xres);
				   pos[1] = (float)(rand() % gl.yres);
				   */
				parent->respawn();
			}

			break;
		case WALL:
			if (this->pos[0]  >= (in_object->pos[0] + (in_object->w / 2)))
				pos[0] = (in_object->pos[0] + (in_object->w/2) + w/2);
			if (this->pos[0] <= (in_object->pos[0] - (in_object->w / 2)))
				pos[0] = (in_object->pos[0] - (in_object->w / 2) - w/2);
			if (this->pos[1] >= (in_object->pos[1] + (in_object->h / 2)))
				pos[1] = (in_object->pos[1] + (in_object->h/2) + h/2);
			if (this->pos[1] < (in_object->pos[1] - (in_object->h / 2)))
				pos[1] = (in_object->pos[1] - (in_object->h / 2) - h/2);
			break;

		case ITEMBOX:
			srand(time(NULL));
			parent->setWeapon(rand() % 6);

			//	member->remObject(in_object);
			cout << "touched itembox" << endl;
			break;
		case BOMB:
			break;
	}



}

ItemBox::ItemBox(PhysWorld * in_member = NULL)
{	
	member = in_member;
	srand(time(NULL));
	boxContent = rand() % 4;
	clearFilter();
	h = (upperBound + lowerBound) /2;
	w = (upperBound + lowerBound) / 2;

	upperBound = 20;
	lowerBound = 15;
	isGrowing = true;
	objectType = ITEMBOX;
	count = 0;
	if (member != NULL)
		member->addObject(this);


}

void ItemBox::handleCollision(Object * in_object)
{

	float x1,x2,y1,y2;
	extern Global gl;


	switch (in_object->objectType) {

		case NON:
			break;
		case SHIP:
			//cout << "ship detected from item box" << endl;
			//parent->boxCount--;
			//member->remObject(this);
			parent->remObject(this);

			break;
		case BULLET:
			//cout << "bull coll" << endl;
			break;
		case WALL:

			x1 = gl.xres/2;
			y1 = gl.yres/2;
			x2 = pos[0];
			y2 = pos[1];

			if((x2 >= (gl.xres/2) - (gl.xres / 4) && x2 <= (gl.xres/2) + (gl.xres/4)) &&
					(y2 >= (gl.yres/2) - (gl.yres / 4) && (y2 <= (gl.yres/2) + (gl.yres/4)))
			  )
			{
				pos[0] += .3 * atan2((y2-y1),(x2-x1));
				pos[1] += .3 * atan2((y2-y1),(x2-x1));
			}
			else{

				pos[0] -= .3 * atan2((y2-y1),(x2-x1));
				pos[1] -= .3 * atan2((y2-y1),(x2-x1));

			}


			break;
		case ITEMBOX:
			break;
		case BOMB:
			break;
	}


}

void ItemBox::render()
{

	if(h > upperBound)
	{
		isGrowing = false;
		h = upperBound;
		w = upperBound;
	}
	if(h < lowerBound)
	{
		isGrowing = true;
		h = lowerBound;
		w = lowerBound;
	}
	float rate = .1;
	if(isGrowing)
	{
		h+= rate;
		w+= rate;
	}
	else
	{
		h-= rate;
		w-= rate;
	}

	count+=.005;
	//cout <<count << endl;
	glPushMatrix();
	//srand(time(NULL));
	//glColor3ub(fmod(rnd() * 10000,255),fmod(rnd() * 10000,255),fmod(rnd() * 100000,255));
	//
	//
	glColor3f(sin(count + PI/2),sin(count+ PI),sin(count + (3*PI)/2));
	glTranslatef(pos[0], pos[1] , 1);
	glBegin(GL_POLYGON);
	glVertex2f(w/2,h/2);
	glVertex2f(w/2, -h/2);
	glVertex2f(  -w/2,  -h/2);
	glVertex2f(-w/2,h/2 );
	glEnd();
	//drawHitbox();
	/*
	   Rect r;
	   r.bot = pos[1] - 25;
	   r.left = pos[0] - 15;
	   r.center = 0;
	   ggprint8b(&r, 16, 0x00000000, (const char *)"COLLIDE FOR RANDOM ITEM, CHOICES ARE . . . ");
	   Rect r2;
	   r2.bot = pos[1] - 45;
	   r2.left = pos[0] - 15;
	   r2.center = 0;
	   ggprint8b(&r2, 16, 0x00000000, (const char *)"NOTHING,SNIPER,BOOMERANG,SPEED,SHIELD");
	   */
	glPopMatrix();




}
ItemBox::~ItemBox()
{
}

BoxWorld::BoxWorld(PhysWorld * in_member = NULL)
{
	member= in_member;

	for (int i = 0; i < BOXCAP;i++) {
		currentBoxes[i] = NULL;
	}

	boxCount = 0;
	timeTillSpawn = 0;
	startTime = time(NULL);
	currentTime = time(NULL) + timeTillSpawn;
}

bool BoxWorld::remObject(ItemBox * in_box)
{
	for (int i = 0; i < BOXCAP; i++) {
		if (currentBoxes[i] == in_box) {
			boxCount--;
			member->remObject(in_box);
			currentBoxes[i] = NULL;
			cout << "removerd succssesfgult" << endl;
			return true;
		}
	}
	return false;

}

BoxWorld::~BoxWorld()
{

}

void BoxWorld::update()
{

	extern Global gl;
	if (currentTime - startTime >= timeTillSpawn) {
		if (boxCount < BOXCAP) {
			//ItemBox * a = new ItemBox(member);
			for (int i = 0; i <BOXCAP;i++) {	
				if (currentBoxes[i] == NULL) {	
					currentBoxIndex = i;
					currentBoxes[i] = new ItemBox(member);
					ItemBox * a = currentBoxes[currentBoxIndex];
					a->pos[0] = rand() % gl.xres;				
					a->pos[1] = rand() % gl.yres;				
					a->parent = this;
					//currentBoxes[0] = new ItemBox(member);
					std::cout << "box spawning . . ." << endl;
					boxCount++;
					currentBoxIndex++;
					currentBoxIndex %= BOXCAP;
					startTime = time(NULL);
					break;
				}	

			}
		}
		else {
			startTime = time(NULL);

		}
		timeTillSpawn = 10 + (rand() % 10);
		cout << "time till next box spawn: " << timeTillSpawn << endl;
	}
	currentTime = time(NULL);

}

Bullet::Bullet(PhysWorld * in_member = NULL)
{
	h = 5;
	w = 5;
	filterSize = 20;
	this->yBounce = 1;
	this->xBounce = 1;
	angle = 0;
	member = in_member;
	rotation = 1;
	objectType = BULLET;
	clearFilter();
	if (member != NULL) {
		//initX = this->bulletParent->parent->ship->pos[0];	
		//initY = this->bulletParent->parent->ship->pos[1];	
	}
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
}
void Bullet::handleCollision(Object * in_object)
{
	switch (in_object->objectType) {
		case NON:
			break;
		case SHIP:
			//addFilter(in_object);
			//cout << "tooche" << endl;
			member->remObject(this);
			break;
		case BULLET:
			break;

		case WALL:
			if (this->pos[0]  >= (in_object->pos[0] + (in_object->w / 2))) {	
				pos[0] = (in_object->pos[0] + (in_object->w/2) + w/2);
				this->xBounce *=-1;
			}
			else if (this->pos[0] <= (in_object->pos[0] - (in_object->w / 2))) {
				pos[0] = (in_object->pos[0] - (in_object->w / 2) - w/2);
				this->xBounce *=-1;
			}
			else if (this->pos[1] >= (in_object->pos[1] + (in_object->h / 2))) {
				pos[1] = (in_object->pos[1] + (in_object->h/2) + h/2);
				this->yBounce *=-1;
			}
			else if (this->pos[1] < (in_object->pos[1] - (in_object->h / 2))) {
				pos[1] = (in_object->pos[1] - (in_object->h / 2) - h/2);
				this->yBounce *=-1;
			}
			//this->xBounce *= -1;
			//this->yBounce *= -1;
			break;
		case ITEMBOX:
			break;
		case BOMB:
			break;
	}

}
Bullet::~Bullet()
{
	//cout << "del\n";
}
//PASSIVE ABILITIES
Passive::Passive(Player * in_parent= NULL) 
{
	parent = in_parent;
}
Passive::~Passive(){}
void Passive::update(){}
void Passive::render(){}

Shield::Shield(Player * in_parent= NULL)
{
	Passive();
	this->parent = in_parent;
	parent->setHealth(4);

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

Shield::~Shield()
{
	parent->setHealth(1);
}

void Shield::update()
{

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

void Shield::render()
{



	if (parent->getHealth() > 1) {

		Rect r;
		r.bot = parent->ship->pos[1] - 25;
		r.left = parent->ship->pos[0] - 15;
		r.center = 0;
		ggprint8b(&r, 16, 0x00ff0000, type);

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

		if (parent->getHealth() > 2) {
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

			if (parent->getHealth() > 3) {
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

			}
		}
	}
}

Speed::Speed(Player * in_parent = NULL)
{
	Passive();
	this->parent = in_parent;
	parent->setSpeed(3.7);
}
Speed::~Speed()
{

	parent->setSpeed(2);
}
void Speed::update(){}
void Speed::render()
{
	//float tempX = parent->ship->pos[0] + parent->ship->w/2;
	//float tempY = parent->ship->pos[1] + parent->ship->h/2;			
	float tempX = parent->ship->pos[0];
	float tempY = parent->ship->pos[1];			

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
			glVertex2f(tempX+xs,tempY+ys);
			glVertex2f(tempX+xe,tempY+ye);
		}

		glEnd();
	}
	Rect r;
	r.bot = tempY - 25;
	r.left = tempX - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

}


Weapon::Weapon(int in_rate = 10, Player * in_parent = NULL)
{
	rate = in_rate;
	parent = in_parent;

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

	//this->barr = new Bullet[MAX_BULLETS];


	struct timespec bulletTimer;
	clock_gettime(CLOCK_REALTIME, &bulletTimer);

}
Boomerang::~Boomerang()
{
	cout << "barr del!"<< endl;
	for (int i = 0; i < nbullets;i++) {
		Bullet * b = barr[i];
		parent->ship->remFilter(b);
		member->remObject(b);
		delete barr[i];
	}
	//delete [] barr;
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
	if (ts > 1.5 || ts < 0) {
		timeCopy(&bulletTimer, &bt);
		if (nbullets < MAX_BULLETS) {
			barr[nbullets]= new Bullet(member);
			Bullet * b = barr[nbullets];
			b->member = member;

			b->addFilter(parent->ship);
			parent->ship->addFilter(b);
			member->addObject(b);

			b->w = 15;
			b->h = 7;
			b->rotation = 9;
			//print gameobj arr
			std::cout << "after adding" << std::endl;
			member->printArr();

			//shoot a bullet...
			///cout << &b << endl;		
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
			/*
			   b->color[0] = 161.0f;
			   b->color[1] = 102.0f;
			   b->color[2] = 47.0f;
			   */
			b->color[0] = parent->ship->color[0] * 255.0f;
			b->color[1] = parent->ship->color[1] * 255.0f;
			b->color[2] = parent->ship->color[2] * 255.0f;

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

		Bullet *b = barr[i];

		//b->testCollision();
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		double ttl = 3.6;
		if (ts > ttl) {
			b->xBounce = 1;
			b->yBounce = 1;

			b->clearFilter();
			parent->ship->remFilter(b);
			member->remObject(b);
			//		
			//
			//
			//		member->addObject(barr[i]);    
			delete b;
			//member->remObject(barr[nbullets-1]);

			cout << barr[nbullets-1] << endl;
			barr[i] = barr[nbullets-1];
			cout << barr[i] << endl;
			//barr[i] = NULL;
			//barr[nbullets-1] = NULL;
			/*
			   barr[nbullets-1]->clearFilter();
			   parent->ship->remFilter(barr[nbullets-1]);
			   member->remObject(barr[nbullets-1]);
			   delete barr[nbullets-1];
			   */

			//barr[i]->addFilter(parent->ship);
			//parent->ship->addFilter(barr[i]);

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
		if (b->pos[0] < 0.0) {
			b->xBounce *= -1;
			b->pos[0] = 5;
		}
		if (b->pos[0] > (float)gl.xres) {
			b->xBounce *= -1;
			b->pos[0] =(float)gl.xres - 5;
		}

		if (b->pos[1] < 0.0) {
			b->yBounce *= -1;
			b->pos[1] = 5;
		}
		if (b->pos[1] > (float)gl.yres) {
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
		Bullet *b = barr[i];

		glPushMatrix();
		glColor3ub(b->color[0],b->color[1],b->color[2]);
		glTranslatef(b->pos[0], b->pos[1] , 1);
		glRotatef((b->angle * b->rotation * 180)/3.14159,0.0f,0.0f,1.0f);

		glBegin(GL_POLYGON);
		glVertex2f(b->w/2,b->h/2);
		glVertex2f(b->w/2, -b->h/2);
		glVertex2f(  -b->w/2,  -b->h/2);
		glVertex2f(-b->w/2,b->h/2 );
		glEnd();
		glPopMatrix();

	}
	Rect r;
	r.bot = parent->ship->pos[1] - 35;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

}
Sniper::Sniper(int in_rate = 10, Player * in_parent = NULL, PhysWorld * in_member = NULL)
{
	Weapon(in_rate, in_parent);
	this->parent = in_parent;
	member = in_member;
	canFire = true;
	tts = 3;
	startTime = time(NULL);
	currentTime = time(NULL);
	iterations = 200;
	step = 10;

	//iterations = 10;
	//step = 50;
}
string Sniper::getWeapon()
{
	return this->type;
}
void Sniper::fireWeapon()
{

	if(canFire){
		Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
		float xdir = cos(rad - PI/2);
		float ydir = sin(rad - PI/2);
		float r = 10;

		//hitscan should be a single frame operation, all logic and collision detection should be contained within this single function call
		//float tempX = parent->ship->pos[0] + parent->ship->w/2;
		//float tempY = parent->ship->pos[1] + parent->ship->h/2;
		float tempX = parent->ship->pos[0];
		float tempY = parent->ship->pos[1];			

		startPosL[0] = tempX + r *(xdir);
		startPosL[1] = tempY + r *(ydir);

		startPosR[0] = tempX - r *(xdir);
		startPosR[1] = tempY - r *(ydir);

		startPosC[0] = tempX ;
		startPosC[1] = tempY ;

		xdir = cos(rad);
		ydir = sin(rad);


		for (float i = 0; i < iterations; i++) {
			Bullet * b = new Bullet(member);

			b->pos[0] = startPosC[0] + (i * step * xdir) + (40 * xdir);
			b->pos[1] = startPosC[1] + (i * step * ydir) + (40 * ydir);

			b->w = 20;
			b->h = 20;

			member->addObject(b);
			parent->ship->addFilter(b);
			b->addFilter(parent->ship);   

			//filtering out BULLETS and ITEMBOXES as to not have those block the sniper shots. 
			//not filtering walls so we we dont have wallbang	
			b->addFilterType(Object::BULLET);
			b->addFilterType(Object::ITEMBOX);
			b->addFilter(parent->ship);

			float x2 = b->pos[0];
			float y2 = b->pos[1];

			if (b->testCollision()) {
				b->clearFilter();   
				b->clearFilterType();
				parent->ship->remFilter(b);
				member->remObject(b);
				float x1 = parent->ship->pos[0];
				float y1 = parent->ship->pos[1];

				shotDistance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
				cout << "shot distance: " << shotDistance << endl;
				delete b;

				break;	
			}
			b->clearFilterType();
			b->clearFilter();   
			parent->ship->remFilter(b);
			member->remObject(b);
			delete b;
			shotDistance = 3000;
		}

		endPosL[0] = startPosL[0] + (shotDistance * xdir);
		endPosL[1] = startPosL[1] + (shotDistance * ydir);

		endPosR[0] = startPosR[0] + (shotDistance * xdir);
		endPosR[1] = startPosR[1] + (shotDistance * ydir);

		endPosC[0] = startPosC[0]  + (shotDistance * xdir);
		endPosC[1] = startPosC[1]  + (shotDistance * ydir);

		canFire = false;
	}
};
void Sniper::physics(){

	currentTime = time(NULL);
	if (currentTime - startTime >= tts) {
		canFire = true;
		startTime = time(NULL);
	}
};
void Sniper::render()
{
	int debug = 0;

	if(debug) {
		for (int i = 0; i < iterations; i++) {
			Flt rad = ((parent->ship->angle+90.0) / 360.0f) * PI * 2.0;
			Object objectArr[200];
			float xdir = cos(rad);
			float ydir = sin(rad);
			int x = startPosC[0] + (i * step * xdir) + (40 * xdir);
			int y = startPosC[1] +  (i * step * ydir) + (40 * ydir);
			glPushMatrix();
			objectArr[i].pos[0] = x;
			objectArr[i].pos[1] = y;
			objectArr[i].h = 20;
			objectArr[i].w = 20;
			objectArr[i].drawHitbox();
			glPopMatrix();
		}
	}

	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2f(startPosL[0],startPosL[1]);
	glVertex2f(endPosL[0],endPosL[1]);
	glVertex2f(startPosR[0],startPosR[1]);
	glVertex2f(endPosR[0],endPosR[1]);
	glVertex2f(startPosC[0],startPosC[1]);
	glVertex2f(endPosC[0],endPosC[1]);
	glEnd();

	Rect r;
	r.bot = parent->ship->pos[1] - 35;
	r.left = parent->ship->pos[0] - 15;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, type);

};
Sniper::~Sniper()
{

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
	std::cout << "ptest" << endl;
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
void Player::setWeapon(int input)
{
	//remove current weapon
	//set empty weapon
	//unfinised, will eventually figure out how i want weapons to be assigned via arguments

	const char * names[] = {"NON","SNIPER","BOOMERANG","SHIELD","SPEED","BOMB"};
	std::cout << names[input] << endl;

	switch (input) {
		case 0:
			//nothing
			cout << "removing your current item!" << endl;
			delete this->currentWeapon;
			currentWeapon = new Weapon(10,this);
			delete this->currentPassive;
			currentPassive = new Passive(this);
			cout << "post" << endl;
			break;
		case 1:
			cout << "sniper" << endl;
			delete this->currentWeapon;
			currentWeapon = new Sniper(10,this,ship->member);
			cout << "post" << endl;
			break;
		case 2:
			cout << "boomerang" << endl;
			delete this->currentWeapon;
			currentWeapon = new Boomerang(10,this,ship->member);
			cout << "post" << endl;
			break;
		case 3:
			cout << "shield" << endl;
			delete this->currentPassive;
			currentPassive = new Shield(this);
			cout << "post" << endl;
			break;
		case 4:
			cout << "spped" << endl;
			delete this->currentPassive;
			currentPassive = new Speed(this);
			break;

	}


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
	setSpeed(2);
	setRSpeed(in_rSpeed);
	PhysWorld * member = in_member;
	isThrust = false;
	health = in_health;
	currentWeapon = new Weapon(10,this);
	currentPassive = new Passive(this);
	ship = new Ship(member,this);
}

void Player::respawn()
{
	extern Global gl;
	setWeapon(0);
	ship->pos[0] = 0;
	ship->pos[1] = 0;
	ship->setColor(0,0,0);
	ship->pos[0] = (float)(rand() % gl.xres);
	ship->pos[1] = (float)(rand() % gl.yres);


}


Player::~Player()
{
	ship->member->remObject(ship);	
}

Hud::Hud(Player * in_p1 = NULL, Player * in_p2 = NULL)
{
	p1 = in_p1;	
	p2 = in_p2;

}
Hud::~Hud()
{

}




void Hud::update()
{

}
void Hud::render()
{
	extern Global gl;
	float h = 70.0f;
	//float w = 200.0f;
	float midW = gl.xres * .23;

	float p1C[3] = {p1->ship->color[0],p1->ship->color[1],p1->ship->color[2]};
	float p2C[3] = {p2->ship->color[0],p2->ship->color[1],p2->ship->color[2]};

	float w = gl.xres / 2;
	//left box
	glPushMatrix();
	glColor3f(p1C[0],p1C[1],p1C[2]);
	glTranslatef(0, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(w,0.0f);
	glVertex2f(w,h);
	glEnd();
	glPopMatrix();


	//right box
	glPushMatrix();
	glColor3f(p2C[0],p2C[1],p2C[2]);
	glTranslatef(gl.xres - w, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(w,0.0f);
	glVertex2f(w,h);
	glEnd();
	glPopMatrix();

	//middle box
	glPushMatrix();
	glColor3f((p1C[0] + p2C[0])/2,(p1C[1] + p2C[1])/2,(p1C[2] + p2C[2])/2);
	glTranslatef(gl.xres/2 - (midW/2), (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(midW,0.0f);
	glVertex2f(midW,h);
	glEnd();
	glPopMatrix();

	//bottom bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,5);
	glVertex2f(0.0f,0.0f);
	glVertex2f(gl.xres,0.0f);
	glVertex2f(gl.xres,5);
	glEnd();
	glPopMatrix();


	//top bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0, (gl.yres-5), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,5);
	glVertex2f(0.0f,0.0f);
	glVertex2f(gl.xres,0.0f);
	glVertex2f(gl.xres,5);
	glEnd();
	glPopMatrix();


	//left mid vert bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef((gl.xres / 2) - midW/2, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(5,0.0f);
	glVertex2f(5,h);
	glEnd();
	glPopMatrix();

	//right mid vert bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef((gl.xres / 2) + midW/2, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(5,0.0f);
	glVertex2f(5,h);
	glEnd();
	glPopMatrix();

	// left vert bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(0, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(5,0.0f);
	glVertex2f(5,h);
	glEnd();
	glPopMatrix();


	//right vert bar
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(gl.xres - 5, (gl.yres-h), 1);
	glBegin(GL_POLYGON);
	glVertex2f(0.0f,h);
	glVertex2f(0.0f,0.0f);
	glVertex2f(5,0.0f);
	glVertex2f(5,h);
	glEnd();
	glPopMatrix();


}


Map2::Map2()
{
	float h,w;
	//woodColor
	float wC[3] = {161,102,47};

	extern PhysWorld * myPhysWorld;
	extern Global gl;
	level[0] = new Wall(myPhysWorld, 30.0f, gl.xres);
	level[0]->pos[1] = 15;
	myPhysWorld->addObject(level[0]);

	level[1] = new Wall(myPhysWorld, 30.0f, gl.xres);
	level[1]->pos[1] = gl.yres-15;
	myPhysWorld->addObject(level[1]);

	level[2] = new Wall(myPhysWorld, gl.xres, 30.f);
	level[2]->pos[0] = gl.xres-15;
	myPhysWorld->addObject(level[2]);

	level[3] = new Wall(myPhysWorld, gl.xres, 30.f);
	level[3]->pos[0] = 15;
	myPhysWorld->addObject(level[3]);

	level[4] = new Wall(myPhysWorld, 100.0f, 100.0f);
	level[4]->setColor(wC[0],wC[1],wC[2]);
	level[4]->pos[0] = gl.xres/2;
	level[4]->pos[1] = 200;
	myPhysWorld->addObject(level[4]);

	level[5] = new Wall(myPhysWorld, 100.0f, 100.0f);
	level[5]->setColor(wC[0],wC[1],wC[2]);
	level[5]->pos[0] = gl.xres/2;
	level[5]->pos[1] = gl.yres - 200;
	myPhysWorld->addObject(level[5]);

	level[6] = new Wall(myPhysWorld, (gl.yres - 520)/2, 20);
	level[6]->setColor(wC[0],wC[1],wC[2]);
	level[6]->pos[0] = gl.xres/2;
	level[6]->pos[1] = gl.yres - (gl.yres - 520);
	myPhysWorld->addObject(level[6]);

	level[7] = new Wall(myPhysWorld, (gl.yres - 520)/2, 20);
	level[7]->setColor(wC[0],wC[1],wC[2]);
	level[7]->pos[0] = gl.xres/2;
	level[7]->pos[1] = (gl.yres - 520);
	myPhysWorld->addObject(level[7]);

	level[8] = new Wall(myPhysWorld, 20.0f, 300.0f);
	level[8]->setColor(wC[0],wC[1],wC[2]);
	level[8]->pos[0] = gl.xres/2 + 100;
	level[8]->pos[1] = gl.yres - 200;
	myPhysWorld->addObject(level[8]);

	level[9] = new Wall(myPhysWorld, 20.0f, 300.0f);
	level[9]->setColor(wC[0],wC[1],wC[2]);
	level[9]->pos[0] = gl.xres/2 - 100;
	level[9]->pos[1] = 200;
	myPhysWorld->addObject(level[9]);

	//top left L shape
	w = 75.0f;
	h = 75.0f;
	level[10] = new Wall(myPhysWorld, h, w);
	level[10]->setColor(wC[0],wC[1],wC[2]);
	level[10]->pos[0] =(int)((float)gl.xres * .2f);
	level[10]->pos[1] = (int)((float)gl.yres * .75f);
	myPhysWorld->addObject(level[10]);

	level[11] = new Wall(myPhysWorld, h, w);
	level[11]->setColor(wC[0],wC[1],wC[2]);
	level[11]->pos[0] =(int)((float)gl.xres * .2f);
	level[11]->pos[1] = (int)((float)gl.yres * .75f) - h;
	myPhysWorld->addObject(level[11]);

	level[12] = new Wall(myPhysWorld, h, w);
	level[12]->setColor(wC[0],wC[1],wC[2]);
	level[12]->pos[0] =(int)((float)gl.xres * .2f) + w;
	level[12]->pos[1] = (int)((float)gl.yres * .75f);
	myPhysWorld->addObject(level[12]);


	//bottom right L shape
	w = 75.0f;
	h = 75.0f;
	level[13] = new Wall(myPhysWorld, h, w);
	level[13]->setColor(wC[0],wC[1],wC[2]);
	level[13]->pos[0] =(int)((float)gl.xres * .75f) + w;
	level[13]->pos[1] = (int)((float)gl.yres * .2f);
	myPhysWorld->addObject(level[13]);

	level[14] = new Wall(myPhysWorld, h, w);
	level[14]->setColor(wC[0],wC[1],wC[2]);
	level[14]->pos[0] =(int)((float)gl.xres * .75f);
	level[14]->pos[1] = (int)((float)gl.yres * .2f);
	myPhysWorld->addObject(level[14]);

	level[15] = new Wall(myPhysWorld, h, w);
	level[15]->setColor(wC[0],wC[1],wC[2]);
	level[15]->pos[0] =(int)((float)gl.xres * .75f) + w;
	level[15]->pos[1] = (int)((float)gl.yres * .2f) + h;
	myPhysWorld->addObject(level[15]);
	/*


	   level[0] = new Wall(myPhysWorld, 20.0f, gl.xres);
	   level[0]->pos[1] = 50;
	   myPhysWorld->addObject(level[0]);


	   level[0] = new Wall(myPhysWorld, 20.0f, gl.xres);
	   level[0]->pos[1] = 50;
	   myPhysWorld->addObject(level[0]);


	   level[0] = new Wall(myPhysWorld, 20.0f, gl.xres);
	   level[0]->pos[1] = 50;
	   myPhysWorld->addObject(level[0]);



	   level[0] = new Wall(myPhysWorld, 20.0f, gl.xres);
	   level[0]->pos[1] = 50;
	   myPhysWorld->addObject(level[0]);
	   */


}

void Map2::render()
{
	for(int i = 0; i < 50; i++)
	{
		if(level[i] != NULL)
		{
			Object * l = level[i];
			glPushMatrix();
			glColor3ub(l->color[0],l->color[1],l->color[2]);
			glTranslatef(l->pos[0], l->pos[1] , 1);
			glBegin(GL_POLYGON);
			glVertex2f(l->w/2,l->h/2);
			glVertex2f(l->w/2, -l->h/2);
			glVertex2f(  -l->w/2,  -l->h/2);
			glVertex2f(-l->w/2,l->h/2 );
			glEnd();
			glPopMatrix();
		}	
	}


}

Map2::~Map2()
{}

