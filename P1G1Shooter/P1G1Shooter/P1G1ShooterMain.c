// P1G1Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Inputs.h"
#include "ConsoleDisplay.h"
#include "bitmap.h"
#include "DisplayZoneDrawing.h"
#include "projectile.h"

int main()
{
    int deltatime = 0, tempdeltatime = 0;
    DisplayZone* dz = (DisplayZone*)malloc(sizeof(DisplayZone));
    InitDisplayZone(dz, 0, 0, 100, 100, 1);

    //projectile* proj = NULL;
    //InitProj(&proj, 1, 90, 0, 0, 54324, 3543543, 1);

    Inputs* myInputs;
    InitInputs(&myInputs);

    do
    {
        tempdeltatime = clock();


        //code

        deltatime = clock() - tempdeltatime;
        EntityUpdate(myInputs);
    } while (1);
}
