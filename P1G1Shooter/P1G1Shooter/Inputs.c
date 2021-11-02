#include "Inputs.h"

#include <stdio.h>
#include <stdlib.h>
#include "Engine/DVector.h"
#include <Windows.h>

void InitInputs(Inputs** inputs)
{
	*inputs = (Inputs*)malloc(sizeof(Inputs));
		
	DVectorInit(&(*inputs)->list, sizeof(Key), NULL, 0);
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
	for (int i = 0; i < inputs->list.mCurrentSize; i++)
	{
		inp = &(DVectorGetTyped(&inputs->list, Key, i));

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

	DVectorPushBack(&inputs->list, &newInput);
}

void DeleteInput(Inputs* inputs, KEYCODE input)
{
	Key* inp = NULL;
	for (int i = 0; i < inputs->list.mCurrentSize; i++)
	{
		inp = &(DVectorGetTyped(&inputs->list, Key, i));

		if (inp->keycode == input)
		{
			if (inp->keystate == KeyUp)
			{
				DVectorErase(&inputs->list, i);
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

	for (int i = 0; i < inputs->list.mCurrentSize; i++)
	{
		currentKey = &(DVectorGetTyped(&inputs->list, Key, i));
		if (currentKey->keystate == keystate)
		{
			printf("| %c |", currentKey->keycode);
		}
	}
}

char IsKeyDown(Inputs inputs, KEYCODE keycode)
{
	Key inp;
	for (int i = 0; i < inputs.list.mCurrentSize; i++)
	{
		inp = DVectorGetTyped(&inputs.list, Key, i);

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
	for (int i = 0; i < inputs.list.mCurrentSize; i++)
	{
		inp = DVectorGetTyped(&inputs.list, Key, i);

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
	for (int i = 0; i < inputs.list.mCurrentSize; i++)
	{
		inp = DVectorGetTyped(&inputs.list, Key, i);

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
	for (int i = 0; i < inputs.list.mCurrentSize; i++)
	{
		inp = DVectorGetTyped(&inputs.list, Key, i);

		if (inp.keycode == keycode && inp.keystate == KeyPressed)
		{
			return 1;
		}
	}
	return 0;
}
