#include "map.h"


void Map::initMap()
{
    // tile layout of the map - must be of size nMapX x nMapY

        //            0         1         2         3
        //            01234567890123456789012345678901
    sMap.append("*##############################*");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("#...........................#..#");
    sMap.append("#..............................@");
    sMap.append("#...#..........................#");
    sMap.append("#.......*#.#*..................@");
    sMap.append("#...@...#...#..................#");
    sMap.append("#.......#...#..................@");
    sMap.append("#...*....@@@...................#");
    sMap.append("#..............................@");
    sMap.append("#...-..........................#");
    sMap.append("#..............................@");
    sMap.append("#...+..........................#");
    sMap.append("#..............................@");
    sMap.append("#...=..........................#");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("***---+++===###..###===+++---***");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("#..............................#");
    sMap.append("#..............................@");
    sMap.append("***---+++===###..###===+++---***");

    // init nMap as a 2d array of ints, having the same size as sMap, and containing the height per cell
    nMap = new int[nMapX * nMapY];
    for (int y = 0; y < nMapY; y++) {
        for (int x = 0; x < nMapX; x++) {
            switch (sMap[y * nMapX + x]) {
            case GRND_FLOOR: nMap[y * nMapX + x] = 0; break;
            case FRST_FLOOR: nMap[y * nMapX + x] = 1; break;
            case SCND_FLOOR: nMap[y * nMapX + x] = 2; break;
            case THRD_FLOOR: nMap[y * nMapX + x] = 3; break;
            case FRTH_FLOOR: nMap[y * nMapX + x] = 4; break;
            case FFTH_FLOOR: nMap[y * nMapX + x] = 5; break;
            case SXTH_FLOOR: nMap[y * nMapX + x] = 6; break;

            }
        }
    }
}

void Map::drawMap(olc::PixelGameEngine* PGEptr)
{
    for (int y = 0; y < nMapY; y++)
    {
        for (int x = 0; x < nMapX; x++)
        {
            olc::Pixel p;
            if (sMap[(y * nMapX) + x] != '.')
            {
                p = olc::BLUE;
            }
            else
            {
                p = olc::GREY;
            }
            PGEptr->FillRect(x * MAP_SIZE, y * MAP_SIZE, MAP_SIZE, MAP_SIZE, p);
        }
    }

    //player in map
    //PGEptr->FillCircle(player.fPlayerX * MAP_SIZE, player.fPlayerY * MAP_SIZE, 2);
    //DrawLine(fPlayerX * MAP_SIZE, fPlayerY * MAP_SIZE, hitx * MAP_SIZE, hity * MAP_SIZE, olc::BLUE);
}

bool Map::hasWallAt(float x, float y)
{
    if (x >= 0 && x < nMapX &&
        y >= 0 && y < nMapY &&
        sMap[int(y) * nMapX + int(x)] == GRND_FLOOR) 
    {
        return true;
    }
    else
    {
        return false;
    }
    
}
