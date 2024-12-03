
class BombObject;

class Bomb: public Weapon
   {
       private:
       Bullet * myBullet;
       struct timespec bulletTimer;
       int bulletCount;
       const char * type = "Bomb";
       public:
       PhysWorld * member;
       Bomb(int in_rate, Player * in_parent, PhysWorld * in_member);
       ~Bomb();
       std::string getWeapon();
       
       void fireWeapon();
       void physics();
       void render();
       BombObject * myBomb;
       int count;
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

class Shotgun: public Weapon
{
       private:
       Bullet * myBullet;
       struct timespec bulletTimer;
       const char * type = "Shotgun";
       public:
       bool isBullet;
       int h, w;
       PhysWorld * member;
       Shotgun(int in_rate, Player * in_parent, PhysWorld * in_member);
       ~Shotgun();
       std::string getWeapon();
       void fireWeapon();
       void physics();
       void render();
       Bullet * sarr[3];
  };

class Map3
{

    public:
        Map3();
        ~Map3();
        //int h, w; 
        void render();
        Object * level[50];
        int levelCount;
};
