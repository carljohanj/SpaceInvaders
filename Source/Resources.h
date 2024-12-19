#pragma once
#include "raylib.h"
#include "vector"

struct Resources		//ToDo: Remove Resources.h
{
	void Load();
	//void Unload();

	std::vector<Texture2D> shipTextures;
	Texture2D alienTexture;
	Texture2D barrierTexture;
	Texture2D laserTexture;

};