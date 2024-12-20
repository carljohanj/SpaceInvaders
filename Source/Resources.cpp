#include "Resources.h"
#include <iostream>;

void Resources::Load()		//ToDo: Move the resources to their corresponding classes and remove Resources.cpp
{
	std::cout << "Yay"; // yes
	alienTexture = LoadTexture("./Assets/Alien.png");
	barrierTexture = LoadTexture("./Assets/Barrier.png");
	/*shipTextures.push_back(LoadTexture("./Assets/Ship1.png"));
	shipTextures.push_back(LoadTexture("./Assets/Ship2.png"));
	shipTextures.push_back(LoadTexture("./Assets/Ship3.png"));*/
	laserTexture = LoadTexture("./Assets/Laser.png");
}