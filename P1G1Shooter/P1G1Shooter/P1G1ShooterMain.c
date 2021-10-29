// P1G1Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>

#include "Inputs.h"

int main()
{
    Inputs* myInputs;
    InitInputs(&myInputs);


    do
    {


        Update(myInputs);
    } while (1);
}
