#include<iostream>
#include "sslone.h"
using namespace std;
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


