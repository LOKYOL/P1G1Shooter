
#include <stdio.h>
#include <stdlib.h>

#include "Vector.h"

typedef unsigned char KEYCODE;

typedef enum KeyState
{
	KeyPressed	= 0,
	KeyDown		= 1,
	KeyUp		= 2
} KeyState;

typedef struct Key
{
	KEYCODE keycode;
	KeyState keystate;
} Key;

typedef struct Inputs
{
	VectorD list;
} Inputs;

void InitInputs(Inputs** inputs);

void Update(Inputs* inputs);
void UpdateInput(Inputs* inputs, KEYCODE input);

void AddInput(Inputs* inputs, KEYCODE input);
void DeleteInput(Inputs* inputs, KEYCODE input);


void PrintAllInputs(Inputs* inputs, KeyState keystate);


char IsKeyDown(Inputs inputs, KEYCODE keycode);
char IsKeyUp(Inputs inputs, KEYCODE keycode);
char WasKeyPressed(Inputs inputs, KEYCODE keycode);
char KeyPressStart(Inputs inputs, KEYCODE keycode);
