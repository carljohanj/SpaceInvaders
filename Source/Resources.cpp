#include "Resources.h"
#include <iostream>;

void Resources::Load()		//ToDo: Move the resources to their corresponding classes and remove Resources.cpp
{
	std::cout << "Yay"; // yes
	laserTexture = LoadTexture("./Assets/Laser.png");
}