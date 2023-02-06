#ifndef SPRITE_H
#define SPRITE_H
#include "olcPixelGameEngine.h"

class Sprite
{
public:
	Sprite() = default;
	void initSprite();

public:
	olc::Sprite* sprites[3];


};

#endif // !SPRITE_H



