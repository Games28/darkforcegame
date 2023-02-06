#ifndef PLAYER_H
#define PLAYER_H

#include "olcPixelGameEngine.h"
#include "map.h"
class Player
{
public:
	Player() = default;
   
    void movePlayer(olc::PixelGameEngine* PGEptr, Map& map, float deltatime);

public:
    // player: position and looking angle
    float fPlayerX = 2.0f;
    float fPlayerY = 2.0f;
    float fPlayerA_deg = 0.0f;      // looking angle is in degrees

    // player: height of eye point and field of view
    float fPlayerH = 0.5f;
    float fPlayerFoV_deg = 60.0f;   // in degrees !!
};


#endif // !PLAYER_H



