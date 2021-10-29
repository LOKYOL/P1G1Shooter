#include "Inputs.h"

#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include <Windows.h>

void InitInputs(Inputs** inputs)
{
	*inputs = (Inputs*)malloc(sizeof(Inputs));
		
	(*inputs)->list = Vector_Create_Vector(Key);
}

void Update(Inputs* inputs)
{
	for (int i = 0; i < 256; i++)
	{
		UpdateInput(inputs, i);
	}
}

void UpdateInput(Inputs* inputs, KEYCODE input)
{
	if (GetKeyState(input) & 0x8000)
	{
		AddInput(inputs, input);
	}
	else
	{
		DeleteInput(inputs, input);
	}
}

void AddInput(Inputs* inputs, KEYCODE input)
{
	Key* inp = NULL;
	for (int i = 0; i < inputs->list._length; i++)
	{
		inp = &Vector_Data(inputs->list, i, Key);

		if (inp->keycode == input)
		{
			if (inp->keystate == KeyPressed)
			{
				inp->keystate = KeyDown;
			}

			return;
		}
	}

	Key newInput;
	newInput.keycode = input;
	newInput.keystate = KeyPressed;

	Vector_Push_back(inputs->list, newInput);
}

void DeleteInput(Inputs* inputs, KEYCODE input)
{
	Key* inp = NULL;
	for (int i = 0; i < inputs->list._length; i++)
	{
		inp = &Vector_Data(inputs->list, i, Key);

		if (inp->keycode == input)
		{
			if (inp->keystate == KeyUp)
			{
				Vector_Erase(inputs->list, i);
			}
			else
			{
				inp->keystate = KeyUp;
			}
			return;
		}
	}
}

void PrintAllInputs(Inputs* inputs, KeyState keystate)
{
	Key* currentKey = NULL;

	for (int i = 0; i < inputs->list._length; i++)
	{
		currentKey = &Vector_Data(inputs->list, i, Key);
		if (currentKey->keystate == keystate)
		{
			printf("| %c |", currentKey->keycode);
		}
	}
}

char IsKeyDown(Inputs inputs, KEYCODE keycode)
{
	Key inp;
	for (int i = 0; i < inputs.list._length; i++)
	{
		inp = Vector_Data(inputs.list, i, Key);

		if (inp.keycode == keycode && inp.keystate <= KeyDown)
		{
			return 1;
		}
	}
	return 0;
}

char IsKeyUp(Inputs inputs, KEYCODE keycode)
{
	Key inp;
	for (int i = 0; i < inputs.list._length; i++)
	{
		inp = Vector_Data(inputs.list, i, Key);

		if (inp.keycode == keycode && inp.keystate <= KeyDown)
		{
			return 0;
		}
	}
	return 1;
}

char WasKeyPressed(Inputs inputs, KEYCODE keycode)
{
	Key inp;
	for (int i = 0; i < inputs.list._length; i++)
	{
		inp = Vector_Data(inputs.list, i, Key);

		if (inp.keycode == keycode && inp.keystate == KeyUp)
		{
			return 1;
		}
	}
	return 0;
}

char KeyPressStart(Inputs inputs, KEYCODE keycode)
{
	Key inp;
	for (int i = 0; i < inputs.list._length; i++)
	{
		inp = Vector_Data(inputs.list, i, Key);

		if (inp.keycode == keycode && inp.keystate == KeyPressed)
		{
			return 1;
		}
	}
	return 0;
}
