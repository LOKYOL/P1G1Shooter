// P1G1Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include "Inputs.h"
#include "ConsoleDisplay.h"
#include "bitmap.h"
#include "DisplayZoneDrawing.h"

int main()
{

    DisplayZone* dz = (DisplayZone*)malloc(sizeof(DisplayZone));
    InitDisplayZone(dz, 0, 0, 100, 100, NULL);

    while (1) {

    }

    Inputs* myInputs;
    InitInputs(&myInputs);

    do
    {


        Update(myInputs);
    } while (1);
}
