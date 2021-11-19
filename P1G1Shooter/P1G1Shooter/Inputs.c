#include "Inputs.h"

#include <stdio.h>
#include <stdlib.h>
#include "Engine/DVector.h"
#include <Windows.h>

void InitInputs(Inputs** _inputs)
{
	*_inputs = (Inputs*)malloc(sizeof(Inputs));
	memset(*_inputs, 0, sizeof(Inputs));
	
	(*_inputs)->list = (DVector*)malloc(sizeof(DVector));
	(*_inputs)->list->mBuffer = NULL;
	DVectorInit((*_inputs)->list, sizeof(Key), NULL, 0);
}

void UpdateAllInputs(Inputs* _inputs)
{
	for (int i = 0; i < 256; i++)
	{
		UpdateInput(_inputs, i);
	}
}

void UpdateInput(Inputs* _inputs, KEYCODE _input)
{
	if (GetKeyState(_input) & 0x8000)
	{
		AddInput(_inputs, _input);
	}
	else
	{
		DeleteInput(_inputs, _input);
	}
}

void AddInput(Inputs* _inputs, KEYCODE _input)
{
	Key* inp = NULL;
	for (int i = 0; i < _inputs->list->mCurrentSize; i++)
	{
		inp = &(DVectorGetTyped(_inputs->list, Key, i));

		if (inp->keycode == _input)
		{
			if (inp->keystate == KeyPressed)
			{
				inp->keystate = KeyDown;
			}

			return;
		}
	}

	Key newInput;
	newInput.keycode = _input;
	newInput.keystate = KeyPressed;

	DVectorPushBack(_inputs->list, &newInput);
}

void DeleteInput(Inputs* _inputs, KEYCODE _input)
{
	Key* inp = NULL;
	for (int i = 0; i < _inputs->list->mCurrentSize; i++)
	{
		inp = &(DVectorGetTyped(_inputs->list, Key, i));

		if (inp->keycode == _input)
		{
			if (inp->keystate == KeyUp)
			{
				DVectorErase(_inputs->list, i);
			}
			else
			{
				inp->keystate = KeyUp;
			}
			return;
		}
	}
}

void PrintAllInputs(Inputs* _inputs, KeyState _keystate)
{
	Key* currentKey = NULL;

	for (int i = 0; i < _inputs->list->mCurrentSize; i++)
	{
		currentKey = &(DVectorGetTyped(_inputs->list, Key, i));
		if (currentKey->keystate == _keystate)
		{
			printf("| %c |", currentKey->keycode);
		}
	}
}

char IsKeyDown(Inputs _inputs, KEYCODE _keycode)
{
	Key inp;
	for (int i = 0; i < _inputs.list->mCurrentSize; i++)
	{
		inp = DVectorGetTyped(_inputs.list, Key, i);

		if (inp.keycode == _keycode && inp.keystate <= KeyDown)
		{
			return 1;
		}
	}
	return 0;
}

char IsKeyUp(Inputs _inputs, KEYCODE _keycode)
{
	Key inp;
	for (int i = 0; i < _inputs.list->mCurrentSize; i++)
	{
		inp = DVectorGetTyped(_inputs.list, Key, i);

		if (inp.keycode == _keycode && inp.keystate <= KeyDown)
		{
			return 0;
		}
	}
	return 1;
}

char WasKeyPressed(Inputs _inputs, KEYCODE _keycode)
{
	Key inp;
	for (int i = 0; i < _inputs.list->mCurrentSize; i++)
	{
		inp = DVectorGetTyped(_inputs.list, Key, i);

		if (inp.keycode == _keycode && inp.keystate == KeyUp)
		{
			return 1;
		}
	}
	return 0;
}

char KeyPressStart(Inputs _inputs, KEYCODE _keycode)
{
	Key inp;
	for (int i = 0; i < _inputs.list->mCurrentSize; i++)
	{
		inp = DVectorGetTyped(_inputs.list, Key, i);

		if (inp.keycode == _keycode && inp.keystate == KeyPressed)
		{
			return 1;
		}
	}
	return 0;
}

void DestroyInputs(Inputs* _inputs)
{
	DVectorDestroy(_inputs->list);
	free(_inputs);
}
