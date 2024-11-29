
class BombObject;

class Bomb: public Weapon
   {
       private:
       Bullet * myBullet;
       struct timespec bulletTimer;
       float space = 10;
       public:
       PhysWorld * member;
       Bomb(int in_rate, Player * in_parent, PhysWorld * in_member);
       ~Bomb();
       //std::string getWeapon();
       void fireWeapon();
       void physics();
       void render();
       BombObject * myBomb;
       Bullet * barr[8];
  };  


class BombObject: public Object
{
  public:
      BombObject(PhysWorld * in_member);
      virtual ~BombObject();
      void handleCollision(Object * in_object);
      bool boom;
      float angle;
};


