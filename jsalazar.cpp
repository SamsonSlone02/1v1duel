#include <iostream>
#include "sslone.h"
#include "jsalazar.h"
using namespace std;
/*
   string Bomb::getWeapon()
   {
   return this->type;
   }
   */ 

/*
 Bullet::Bullet(PhysWorld * in_member = NULL) : Object(in_member)
 {
     //Object(in_member);
     h = 5;
     w = 5;
 
     this->yBounce = 1;
     this->xBounce = 1;
     angle = 0;
     member = in_member;
 
     objectType = BULLET;
 
     if (member != NULL) {
         //initX = this->bulletParent->parent->ship->pos[0]; 
         //initY = this->bulletParent->parent->ship->pos[1]; 
     }
     struct timespec time;
     clock_gettime(CLOCK_REALTIME, &time);
 }


 void Bullet::handleCollision(Object * in_object)
 {
     switch(in_object->objectType)
     {
         case NON:
             break;
         case SHIP:
             cout << "ship detected" << endl;
             break;
         case BULLET:
             break;
         case WALL:
             break;
 
     }
 
 }
*/

Bomb::Bomb(int in_rate = 10, Player * in_parent = NULL , PhysWorld * in_member = NULL) : Weapon(in_rate, in_parent)
{
    Weapon(in_rate, in_parent);
  //  Bullet Bomb(in_member);
    member = in_member;
    this->parent = in_parent;
    nbullets = 0;
 //   this->barr = new Bullet[MAX_BULLETS];

}

Bomb::~Bomb()
{
    delete [] barr;  

}

//void Bomb::fireWeapon{}
//void Bomb::physics(){}




void Bomb::render()
{


}

