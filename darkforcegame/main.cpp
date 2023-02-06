#include <cfloat>     // I need FLT_MAX in the DDA function

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "defs.h"
#include "map.h"
#include "Player.h"
#include "sprite.h"
#include "raycast.h"

// Screen and pixel constants - keep the screen sizes constant and vary the resolution by adapting the pixel size
// to prevent accidentally defining too large a window
#define SCREEN_X    960
#define SCREEN_Y    600
#define PIXEL_X       1
#define PIXEL_Y       1

// colour constants
#define COL_CEIL    olc::DARK_BLUE
#define COL_FLOOR   olc::DARK_YELLOW
#define COL_WALL    olc::GREY
#define COL_TEXT    olc::MAGENTA


class MyRayCaster : public olc::PixelGameEngine {

public:
    MyRayCaster() {    // display the screen and pixel dimensions in the window caption
        sAppName = "MyRayCaster - Permadi tutorial - S:(" + std::to_string(SCREEN_X / PIXEL_X) + ", " + std::to_string(SCREEN_Y / PIXEL_Y) + ")" +
            ", P:(" + std::to_string(PIXEL_X) + ", " + std::to_string(PIXEL_Y) + ")";
    }

private:
    Player player;
    Map map;
    Sprite sprite;
    Raycast ray;
        // take diagonal distance into account

    

    float fDistToProjPlane;         // constant distance to projection plane - is calculated in OnUserCreate()

    


public:



    bool OnUserCreate() override {

       
        // work out distance to projection plane. This is a constant depending on the width of the projection plane and the field of view.
        fDistToProjPlane = ((ScreenWidth() / 2.0f) / sin((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f)) * cos((player.fPlayerFoV_deg / 2.0f) * PI / 180.0f);

        // load sprites for texturing walls, floor and ceiling
        map.initMap();
        sprite.initSprite();
       
      

        return true;
    }

    // adaptation of the DDA function to support a list of intersections (instead of just the first one)

        // Holds intersection point in float (world) coordinates and in int (tile) coordinates,
        // the distance to the intersection point and the height of the map at these tile coordinates
   

    // Implementation of the DDA algorithm. This function uses class variables for the description of the map.
    // The players position is the "from point", a "to point" is determined using fRayAngle and fMaxDistance.
    // A ray is cast from the "from point" to the "to point". If there is a collision (intersection with wall cell
    // in the map) then the point of intersection, the distance and the map tile of the wall cell is returned in the references.
   

    

  

 
 
    bool OnUserUpdate(float fElapsedTime) override {

        
        // step 1 - user input
        // ===================
        player.movePlayer(this, map, fElapsedTime);
        //Clear(olc::BLACK);
        ray.WallProjection(fDistToProjPlane, this, player, map, sprite);
        //map.drawMap(this);

      

        // step 2 - game logic
        // ===================

        // step 3 - render
        // ===============

        

       

        // output some player values for debugging
        //DrawString(10, 10, "fPlayerX = " + std::to_string(fPlayerX), COL_TEXT);
        //DrawString(10, 20, "fPlayerY = " + std::to_string(fPlayerY), COL_TEXT);
        //DrawString(10, 30, "fPlayerA = " + std::to_string(fPlayerA_deg), COL_TEXT);
        
        return true;
    }
};

int main()
{
    MyRayCaster demo;
    if (demo.Construct(SCREEN_X / PIXEL_X, SCREEN_Y / PIXEL_Y, PIXEL_X, PIXEL_Y))
        demo.Start();

    return 0;
}
