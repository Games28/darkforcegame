#ifndef MAP_H
#define MAP_H
#include "defs.h"
#include <string>
#include "olcPixelGameEngine.h"


class Map
{
public:
	Map() = default;
	void initMap();
	void drawMap(olc::PixelGameEngine* PGEptr);
	bool hasWallAt(float x, float y);
public:
	std::string sMap;     // contains char's that define the type of block per map location
	int* nMap;     // contains int's  that represent the height per block
	int nMapX = 32;
	int nMapY = 32;

};



#endif // !MAP_H



