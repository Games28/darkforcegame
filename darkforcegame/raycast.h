#ifndef RAYCAST_H
#define RAYCAST_H
#include "olcPixelGameEngine.h"
#include "sprite.h"
#include "Player.h"
#include "map.h"

struct IntersectInfo {
    float fHitX,
        fHitY;
    float fDistance;
    int   nMapCoordX,
        nMapCoordY;
    int nHeight;
};

class Raycast
{
public:
	Raycast() = default;
    bool GetDistancesToWalls(float fRayAngle, std::vector<IntersectInfo>& vHitList, Player& player, Map& map);
    void CalculateWallBottomAndTop(float disttoplane,olc::PixelGameEngine* PGEptr, float fCorrectedDistToWall, int nWallHeight, int& nWallTop, int& nWallBottom);
     void WallProjection(float disttoplane,olc::PixelGameEngine* PGEptr,Player& player, Map& map,Sprite& sprite);
public:
    float fMaxDistance = 40.0f;
};


#endif // !RAYCAST_H



