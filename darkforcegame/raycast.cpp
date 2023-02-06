#include "raycast.h"


bool Raycast::GetDistancesToWalls(float fRayAngle, std::vector<IntersectInfo>& vHitList, Player& player, Map& map)
{
    // The player's position is the "from point"
    float fFromX = player.fPlayerX;
    float fFromY = player.fPlayerY;
    // Calculate the "to point" using it's angle and fMaxDistance
    float fToX = player.fPlayerX + fMaxDistance * cos(fRayAngle * PI / 180.0f);
    float fToY = player.fPlayerY + fMaxDistance * sin(fRayAngle * PI / 180.0f);
    // work out the direction vector (fDX, fDY) and normalize it
    float fDX = fToX - fFromX;
    float fDY = fToY - fFromY;
    float fRayLen = sqrt(fDX * fDX + fDY * fDY);
    fDX /= fRayLen;
    fDY /= fRayLen;

    // calculate the scaling factors for the ray increments per unit in x resp y direction
    // this calculation takes division by 0.0f into account
    float fSX = (fDX == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDY / fDX) * (fDY / fDX));
    float fSY = (fDY == 0.0f) ? FLT_MAX : sqrt(1.0f + (fDX / fDY) * (fDX / fDY));

    float fLengthPartialRayX = 0.0f;
    float fLengthPartialRayY = 0.0f;
    // work out if line is going right or left resp. down or up
    int nGridStepX = (fDX > 0.0f) ? +1 : -1;
    int nGridStepY = (fDY > 0.0f) ? +1 : -1;
    int nCurX = int(fFromX);
    int nCurY = int(fFromY);

    // work out the first intersections with the grid
    if (nGridStepX < 0) { // ray is going left - get scaled difference between start point and left cell border
        fLengthPartialRayX = (fFromX - float(nCurX)) * fSX;
    }
    else {              // ray is going right - get scaled difference between right cell border and start point
        fLengthPartialRayX = (float(nCurX + 1.0f) - fFromX) * fSX;
    }
    if (nGridStepY < 0) { // ray is going up - get scaled difference between start point and top cell border
        fLengthPartialRayY = (fFromY - float(nCurY)) * fSY;
    }
    else {              // ray is going down - get scaled difference between bottom cell border and start point
        fLengthPartialRayY = (float(nCurY + 1.0f) - fFromY) * fSY;
    }

    // did analysis get out of map boundaries?
    bool bOutOfBounds = (nCurX < 0 || nCurX >= map.nMapX ||
        nCurY < 0 || nCurY >= map.nMapY);
    // was a hit with a wall cell found?
    bool bHitFound = bOutOfBounds ? false : map.sMap[nCurY * map.nMapX + nCurX] != '.';
    // did analysis reach the destination cell?
    bool bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

    float fDistIfFound = 0.0f;
    while (!bOutOfBounds && !bDestCellFound && fDistIfFound < fMaxDistance) {

        // advance to next map cell, depending on length of partial ray's
        if (fLengthPartialRayX < fLengthPartialRayY) {
            // move in x direction
            nCurX += nGridStepX;
            fDistIfFound = fLengthPartialRayX;
            fLengthPartialRayX += fSX;
        }
        else {
            // move in y direction
            nCurY += nGridStepY;
            fDistIfFound = fLengthPartialRayY;
            fLengthPartialRayY += fSY;
        }

        bOutOfBounds = (nCurX < 0 || nCurX >= map.nMapX ||
            nCurY < 0 || nCurY >= map.nMapY);
        if (bOutOfBounds) {
            bHitFound = false;
            bDestCellFound = false;
        }
        else {
            bHitFound = bOutOfBounds ? false : map.sMap[nCurY * map.nMapX + nCurX] != '.';
            bDestCellFound = (nCurX == int(fToX) && nCurY == int(fToY));

            if (bHitFound) {
                // put the collision values in a new IntersectInfo node and push it up the hit list
                IntersectInfo sInfo;

                sInfo.fDistance = fDistIfFound;
                sInfo.fHitX = fFromX + fDistIfFound * fDX;
                sInfo.fHitY = fFromY + fDistIfFound * fDY;
                sInfo.nMapCoordX = nCurX;
                sInfo.nMapCoordY = nCurY;
                sInfo.nHeight = map.nMap[nCurY * map.nMapX + nCurX];

                vHitList.push_back(sInfo);
            }
        }
    }

    return (vHitList.size() > 0);
}

void Raycast::CalculateWallBottomAndTop(float disttoplane,olc::PixelGameEngine* PGEptr,float fCorrectedDistToWall, int nWallHeight, int& nWallTop, int& nWallBottom)
{
    // calculate slice height for a *unit height* wall
    int nSliceHeight = int((1.0f / fCorrectedDistToWall) * disttoplane);
    // offset wall slice height from halfway screen height (horizon) - take wall height into account
    nWallTop = (PGEptr->ScreenHeight() / 2) - (nSliceHeight / 2.0f) - (nWallHeight - 1) * nSliceHeight;
    nWallBottom = (PGEptr->ScreenHeight() / 2) + (nSliceHeight / 2.0f);
}

void Raycast::WallProjection(float disttoplane,olc::PixelGameEngine* PGEptr, Player& player, Map& map, Sprite& sprite)
{
    
    int nHalfScreenWidth = PGEptr->ScreenWidth() / 2;
    int nHalfScreenHeight = PGEptr->ScreenHeight() / 2;
    float fAngleStep = disttoplane / float(PGEptr->ScreenWidth());
    float fX_hit, fY_hit;
    // iterate over all screen slices, processing the screen in columns
    for (int x = 0; x < PGEptr->ScreenWidth(); x++) {
        float fViewAngle = float(x - nHalfScreenWidth) * fAngleStep;
        float fCurAngle = player.fPlayerA_deg + fViewAngle;

        float fRawDistToWall, fCorrectDistToWall;
        // to hold exact (float) hit location
        int   nX_hit, nY_hit;   // to hold coords of tile that was hit

        int nWallCeil, nWallFloor;   // to store the top and bottom y coord of the wall per column

        // this lambda returns a sample of the ceiling through the pixel at screen coord (px, py)
        auto get_ceil_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the ceiling you are looking at through this pixel
            // (the pixel is given since you know the x and y screen coordinate to draw to)
            float fCeilProjDistance = ((player.fPlayerH / float(nHalfScreenHeight - py)) * disttoplane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world ceiling coordinate from the player's position, the distance and the view angle + player angle
            float fCeilProjX = player.fPlayerX + fCeilProjDistance * cos(fCurAngle * PI / 180.0f);
            float fCeilProjY = player.fPlayerY + fCeilProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world ceiling coordinate, by subtracting the
            // integer part and only keeping the fractional part
            float fSampleX = fCeilProjX - int(fCeilProjX);
            float fSampleY = fCeilProjY - int(fCeilProjY);
            // having both sample coordinates, get the sample and draw the pixel
            return sprite.sprites[2]->Sample(fSampleX, fSampleY);
        };

        // this lambda returns a sample of the floor through the pixel at screen coord (px, py)
        auto get_floor_sample = [=](int px, int py) -> olc::Pixel {
            // work out the distance to the location on the floor you are looking at through this pixel
            // (the pixel is given since you know the x and y to draw to)
            float fFloorProjDistance = ((player.fPlayerH / float(py - nHalfScreenHeight)) * disttoplane) / cos(fViewAngle * PI / 180.0f);
            // calculate the world floor coordinate from the distance and the view angle + player angle
            float fFloorProjX = player.fPlayerX + fFloorProjDistance * cos(fCurAngle * PI / 180.0f);
            float fFloorProjY = player.fPlayerY + fFloorProjDistance * sin(fCurAngle * PI / 180.0f);
            // calculate the sample coordinates for that world floor coordinate, by subtracting the
            // integer part and only keeping the fractional part
            float fSampleX = fFloorProjX - int(fFloorProjX);
            float fSampleY = fFloorProjY - int(fFloorProjY);
            // having both sample coordinates, get the sample and draw the pixel
            return  sprite.sprites[1]->Sample(fSampleX, fSampleY);
        };

        // prepare the rendering for this screen slice by calculating the list of intersections in this direction
        std::vector<IntersectInfo> vColHitlist;
        if (GetDistancesToWalls(fCurAngle, vColHitlist,player,map)) {
            // a wall was hit - set bottom and top value depending on the distance found
            if (x == 478)
            {
                int u = 0;
            }
            // get the info from first hit point
            fX_hit = vColHitlist[0].fHitX;
            fY_hit = vColHitlist[0].fHitY;
            nX_hit = vColHitlist[0].nMapCoordX;
            nY_hit = vColHitlist[0].nMapCoordY;
            fRawDistToWall = vColHitlist[0].fDistance;
            int nColHeight = vColHitlist[0].nHeight;

            // make correction for the fish eye effect
            fCorrectDistToWall = fRawDistToWall * cos(fViewAngle * PI / 180.0f);
            CalculateWallBottomAndTop(disttoplane, PGEptr,fCorrectDistToWall, nColHeight, nWallCeil, nWallFloor);
        }
        else {
            // no wall was hit - set bottom and top value for wall at the horizon
            nWallCeil = nHalfScreenHeight;
            nWallFloor = nHalfScreenHeight;
        }

        // now render this slice using the info of the hit list
        int nHitListIndex = 0;
        // note that we are working upwards
        for (int y = PGEptr->ScreenHeight() - 1; y >= 0; y--) {

            // constants for different types of rendering


            int nDrawMode = UNKNOWN_DRAWING;
            if (y >= nWallFloor) {
                nDrawMode = FLOOR_DRAWING;
            }
            else if (nWallFloor > y && y > nWallCeil) {
                nDrawMode = WALL_DRAWING;
            }
            else if (y <= nWallCeil) {
                while (nDrawMode == UNKNOWN_DRAWING) {
                    if (nHitListIndex < (int)vColHitlist.size() - 1) {
                        // there are still hit points to process
                        nHitListIndex += 1;

                        // get the info from next hit point
                        fX_hit = vColHitlist[nHitListIndex].fHitX;
                        fY_hit = vColHitlist[nHitListIndex].fHitY;
                        nX_hit = vColHitlist[nHitListIndex].nMapCoordX;
                        nY_hit = vColHitlist[nHitListIndex].nMapCoordY;
                        fRawDistToWall = vColHitlist[nHitListIndex].fDistance;
                        int nColHeight = vColHitlist[nHitListIndex].nHeight;

                        // make correction for the fish eye effect
                        fCorrectDistToWall = fRawDistToWall * cos(fViewAngle * PI / 180.0f);
                        // get new values for nWallFloor and nWallCeil
                        int nCacheWallCeil = nWallCeil;
                        CalculateWallBottomAndTop(disttoplane, PGEptr, fCorrectDistToWall, nColHeight, nWallCeil, nWallFloor);

                        // use this intersection point only if the ceiling of its wall is higher than the previous wall segment.
                        if (nWallCeil < nCacheWallCeil)
                            nDrawMode = WALL_DRAWING;
                    }
                    else {
                        nDrawMode = CEIL_DRAWING;
                    }
                }
            }
            else {
                std::cout << "ERROR: OnUserUpdate() --> can't position y value as floor, wall or ceiling..." << std::endl;
            }

            // now we know what type of segments we're working on, render it
            switch (nDrawMode) {
            case CEIL_DRAWING: {                         // ========== render ceiling ====================
                // ceiling texturing doesn't work correctly with variable heights walls
//                            olc::Pixel auxSample = get_ceil_sample( x, y );
//                            Draw( x, y, auxSample );
            }
                             break;
            case FLOOR_DRAWING: {                        // ========== render floor   ====================
                olc::Pixel auxSample = get_floor_sample(x, y);
                PGEptr->Draw(x, y, auxSample);
            }
                              break;
            case WALL_DRAWING: {                         // ========== render wall    ====================
                // the y sample coordinate depends only on the pixel y coord on the screen
                // in relation to the vertical space the wall is taking up
                float fSampleY = float(y - nWallCeil) / float(nWallFloor - nWallCeil);

                // the x sample coordinate takes more work to figure out. You need to check what side of the
                // block was hit
                float fSampleX;
                float fBlockMidX = (float)nX_hit + 0.5f;   // location of middle of the cell that was hit
                float fBlockMidY = (float)nY_hit + 0.5f;
                // determine in what quadrant the hit location is wrt the block mid point
                float fTestAngle = atan2f((fY_hit - fBlockMidY), (fX_hit - fBlockMidX));
                if (-0.75f * PI <= fTestAngle && fTestAngle < -0.25f * PI) fSampleX = fX_hit - (float)nX_hit;  // south side
                if (-0.25f * PI <= fTestAngle && fTestAngle < 0.25f * PI) fSampleX = fY_hit - (float)nY_hit;  // east  side
                if (0.25f * PI <= fTestAngle && fTestAngle < 0.75f * PI) fSampleX = fX_hit - (float)nX_hit;  // north side
                if (-0.75f * PI > fTestAngle || fTestAngle >= 0.75f * PI) fSampleX = fY_hit - (float)nY_hit;  // west  side

                // having both sample coordinates, get the sample and draw the pixel
                olc::Pixel auxSample = sprite.sprites[0]->Sample(fSampleX, fSampleY);
                PGEptr->Draw(x, y, auxSample);
            }
                             break;
            default:
                std::cout << "ERROR: OnUserUpdate() --> unknown draw mode encountered: " << nDrawMode << std::endl;
                // draw a yellow pixel for debugging
                olc::Pixel auxSample = olc::YELLOW;
                PGEptr->Draw(x, y, auxSample);
                break;
            }
        }

    }
}
