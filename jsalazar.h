 class Bomb: public Weapon
   {
       private:
       Vec pos;
       Vec vel;
       float color[3];
       
        Bullet * mybullet;
      struct timespec bulletTimer;
      int nbullets;
      bool isBullet;
      //const char * type = "Bomb";
     //std::string type = "Bomb";
      public:
      PhysWorld * member;
      Bomb(int in_rate, Player * in_parent, PhysWorld * in_member);
      ~Bomb();
     // std::string getWeapon();
      void fireWeapon();
      void physics();
      void render();
  
  };      
