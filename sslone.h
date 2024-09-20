
class Ship;
class Player
{

	private:
		
	public:

		Ship *ship;
		void test();
		void setKeys(int in_up, int in_down, int in_left, int in_right, int in_attack);
		int up,down,left,right,attack;
};
