#include "Player.h"




void Player::movePlayer(olc::PixelGameEngine* PGEptr, Map& map, float deltatime)
{
    // Rotate - collision detection not necessary. Keep fPlayerA_deg between 0 and 360 degrees
    if (PGEptr->GetKey(olc::D).bHeld) { fPlayerA_deg += SPEED_ROTATE * deltatime; if (fPlayerA_deg >= 360.0f) fPlayerA_deg -= 360.0f; }
    if (PGEptr->GetKey(olc::A).bHeld) { fPlayerA_deg -= SPEED_ROTATE * deltatime; if (fPlayerA_deg < 0.0f) fPlayerA_deg += 360.0f; }

    // variables used for collision detection - work out the new location in a seperate coordinate pair, and only alter
    // the players coordinate if there's no collision
    float fNewX = fPlayerX;
    float fNewY = fPlayerY;

    // walking forward, backward and strafing left, right
    float fPlayerA_rad = fPlayerA_deg * PI / 180.0f;
    if (PGEptr->GetKey(olc::W).bHeld) { fNewX += cos(fPlayerA_rad) * SPEED_MOVE *   deltatime; fNewY += sin(fPlayerA_rad) * SPEED_MOVE *   deltatime; }   // walk forward
    if (PGEptr->GetKey(olc::S).bHeld) { fNewX -= cos(fPlayerA_rad) * SPEED_MOVE *   deltatime; fNewY -= sin(fPlayerA_rad) * SPEED_MOVE *   deltatime; }   // walk backwards
    if (PGEptr->GetKey(olc::Q).bHeld) { fNewX += sin(fPlayerA_rad) * SPEED_STRAFE * deltatime; fNewY -= cos(fPlayerA_rad) * SPEED_STRAFE * deltatime; }   // strafe left
    if (PGEptr->GetKey(olc::E).bHeld) { fNewX -= sin(fPlayerA_rad) * SPEED_STRAFE * deltatime; fNewY += cos(fPlayerA_rad) * SPEED_STRAFE * deltatime; }   // strafe right
    // collision detection - check if out of bounds or inside non-empty tile
    // only update position if no collision
    if (fNewX >= 0 && fNewX < map.nMapX &&
        fNewY >= 0 && fNewY < map.nMapY &&
        map.sMap[int(fNewY) * map.nMapX + int(fNewX)] == GRND_FLOOR) {
        fPlayerX = fNewX;
        fPlayerY = fNewY;
    }
}
