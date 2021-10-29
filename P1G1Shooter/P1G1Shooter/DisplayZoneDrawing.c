#include <math.h>
#include "DisplayZoneDrawing.h"

#define ABS_INT(a) ((a<0)?(-(a)):(a))


unsigned char	greyRamp[] = " .:-=+*#%@";
unsigned int	RGB_ConsoleColorsValues[16] = { 0x0C0C0C,0xC50F1F,0x13A1E0,0xC19C00,0x0037DA,0x881798,0x3A96DD,0xCCCCCC,
												0x767676,0xE74856,0x16C60C,0xF9F1A5,0x3B78FF,0xB4009E,0x61D6D6,0xF2F2F2 };

extern void	Blender(DisplayCharacter* src, DisplayCharacter* dst, DisplayCharacter* blender);

// draw a filled rectangle with given pos, size, border and fill DisplayCharacter
DisplayRectangle	DrawRectangleInDisplayZoneDisplayCharacter(DisplayZone* zone, int px, int py, int sx, int sy, DisplayCharacter* fill, DisplayCharacter* border)
{
	

	if (!zone->mBuffer)
	{
		return;
	}

	DisplayCharacter* dstPixels = zone->mBuffer;
	for (int j = 0; j < sy; j++)
	{
		DisplayCharacter* currentLineDisplay = fill;
		if ((j == 0) || (j == (sy - 1)))
		{
			currentLineDisplay = border;
		}

		int dstPy = py + j;

		if ((dstPy >= 0) && (dstPy < zone->mSizeY))
		{
			for (int i = 0; i < sx; i++)
			{
				DisplayCharacter* currentDisplay = currentLineDisplay;
				if ((i == 0) || (i == (sx - 1)))
				{
					currentDisplay = border;
				}
				int dstPx = px + i;
				if ((dstPx >= 0) && (dstPx < zone->mSizeX))
				{
					DisplayCharacter* currentPixel = dstPixels + dstPx + dstPy * zone->mSizeX;

					DisplayCharacter blended;

					Blender(currentDisplay, currentPixel, &blended);

					*currentPixel = blended;
				}

			}
		}
	}

	DisplayRectangle rectangle = { zone, fill, border, px, py, sx, sy };
	rectangle.mPosX = px;
	rectangle.mPosY = py;
	rectangle.mSizeX = sx;
	rectangle.mSizeY = sy;

	return rectangle;
}

// draw a filled rectangle with given pos, size, border and fill colors and character
DisplayRectangle	DrawRectangleInDisplayZoneEx(DisplayZone* zone, int px, int py, int sx, int sy, ConsoleColors fillFG, ConsoleColors fillBG, char fillCharacter, ConsoleColors borderFG, ConsoleColors borderBG, char borderCharacter)
{
	if (!zone->mBuffer)
	{
		return;
	}


	DisplayCharacter fillPixel;
	CodeColors(&fillPixel, fillFG, fillBG);
	fillPixel.mCharacter = fillCharacter;
	fillPixel.mCharactersFlag = NO_FLAG;
	DisplayCharacter borderPixel;
	CodeColors(&borderPixel,borderFG, borderBG);
	borderPixel.mCharacter = borderCharacter;
	borderPixel.mCharactersFlag = NO_FLAG;

	return DrawRectangleInDisplayZoneDisplayCharacter(zone, px, py, sx, sy, &fillPixel, &borderPixel);
}

// draw a rectangle with given pos, size, colors and character
void	DrawRectangleInDisplayZone(DisplayZone* zone, int px, int py, int sx, int sy, ConsoleColors FG, ConsoleColors BG, char character)
{
	DrawRectangleInDisplayZoneEx(zone, px, py, sx, sy, FG, BG, character, FG, BG, character);
}


void	DrawDiscInDisplayZoneDisplayCharacter(DisplayZone* zone, int cx, int cy, int r, DisplayCharacter* fill, DisplayCharacter* border)
{
	if (!zone->mBuffer)
	{
		return;
	}

	float fr = (float)r;

	DisplayCharacter* dstPixels = zone->mBuffer;
	int lastStartX = 0;
	for (int j = -r; j <= 0; j++)
	{
		int dstPy[2];
		dstPy[0] = cy + j;
		dstPy[1] = cy - j;
		float fj = ((float)ABS_INT(j)) - 0.5f;
		int startX = (int)sqrtf(fr * fr - fj * fj);

		for (int i = -startX; i <= startX; i++)
		{
			int dstPx = cx + i;
			if ((dstPx >= 0) && (dstPx < zone->mSizeX))
			{
				DisplayCharacter* currentLineDisplay = fill;
				if (ABS_INT(i) >= lastStartX)
				{
					currentLineDisplay = border;
				}

				int symLimit = (dstPy[0] == dstPy[1]) ? 1 : 2;
				for (int sym = 0; sym < symLimit; sym++)
				{
					if ((dstPy[sym] >= 0) && (dstPy[sym] < zone->mSizeY))
					{
						DisplayCharacter* currentPixel = dstPixels + dstPx + dstPy[sym] * zone->mSizeX;
						DisplayCharacter blended;
						Blender(currentLineDisplay, currentPixel, &blended);
						*currentPixel = blended;
					}
				}
			}
		}

		lastStartX = startX;
	}
}


// draw disc with given center, radius, border and fill colors and character
void	DrawDiscInDisplayZoneEx(DisplayZone* zone, int cx, int cy, int r, ConsoleColors fillFG, ConsoleColors fillBG, char fillCharacter, ConsoleColors borderFG, ConsoleColors borderBG, char borderCharacter)
{
	DisplayCharacter fillPixel;
	CodeColors(&fillPixel, fillFG, fillBG);
	fillPixel.mCharacter = fillCharacter;
	fillPixel.mCharactersFlag = NO_FLAG;
	DisplayCharacter borderPixel;
	CodeColors(&borderPixel, borderFG, borderBG);
	borderPixel.mCharacter = borderCharacter;
	borderPixel.mCharactersFlag = NO_FLAG;

	DrawDiscInDisplayZoneDisplayCharacter(zone, cx, cy, r, &fillPixel, &borderPixel);
}

// draw disc with given center, radius, colors and character
void	DrawDiscInDisplayZone(DisplayZone* zone, int cx, int cy, int r, ConsoleColors FG, ConsoleColors BG, char character)
{
	DrawDiscInDisplayZoneEx(zone, cx, cy, r, FG, BG, character, FG, BG, character);
}

typedef struct rgbVect
{
	float R;
	float G;
	float B;

}rgbVect;

rgbVect getRGBFromConsoleIndex(unsigned int index)
{
	rgbVect	result;
	result.R = (int)(RGB_ConsoleColorsValues[index] >> 16);
	result.G = (int)((RGB_ConsoleColorsValues[index] >> 8) & 0xFF);
	result.B = (int)(RGB_ConsoleColorsValues[index] & 0xFF);
	return result;
}

rgbVect getRGBFromValues(unsigned char R, unsigned char G, unsigned char B)
{
	rgbVect	result;
	result.R = (int)R;
	result.G = (int)G;
	result.B = (int)B;
	return result;
}

rgbVect	getRGBDirection(rgbVect start, rgbVect end)
{
	rgbVect	result;
	result.R = end.R - start.R;
	result.G = end.G - start.G;
	result.B = end.B - start.B;
	return result;
}

void RGBMult(rgbVect* c, float f)
{
	c->R *= f;
	c->G *= f;
	c->B *= f;
}

float	getRGBSquaredNorm(rgbVect c)
{
	return c.R * c.R + c.G * c.G + c.B * c.B;
}

float	getRGBDot(rgbVect c1,rgbVect c2)
{
	return c1.R * c2.R + c1.G * c2.G + c1.B * c2.B;
}

float getRGBNorm(rgbVect c)
{
	return sqrtf(getRGBSquaredNorm(c));
}

// 3D distance, don't take visual distance into account
unsigned int	getNearestConsoleColor(unsigned char R, unsigned char G, unsigned char B)
{
	rgbVect input = getRGBFromValues(R, G, B);
	
	float bestSquaredDist = 10000000.0f;
	unsigned int bestIndex = -1;
	for (int i = 0; i < 16; i++)
	{
		rgbVect rgbConsole = getRGBFromConsoleIndex(i);
		
		rgbVect diff = getRGBDirection(rgbConsole, input);

		float squaredDist = getRGBSquaredNorm(diff);
		if (squaredDist < bestSquaredDist)
		{
			bestSquaredDist = squaredDist;
			bestIndex = i;
		}
	}
	return bestIndex;
}

// 3D oriented distance, don't take visual distance into account
unsigned int	getOrientedNearestConsoleColor(unsigned char R, unsigned char G, unsigned char B,unsigned int oppositeIndex)
{
	rgbVect input = getRGBFromValues(R, G, B);
	rgbVect consoleOpposite = getRGBFromConsoleIndex(oppositeIndex);
	
	rgbVect direction = getRGBDirection(consoleOpposite, input);

	unsigned int bestIndex = 16;

	float norm = getRGBNorm(direction);
	if (norm)
	{
		norm = 1.0f / norm;
	
		RGBMult(&direction, norm);

		float bestSquaredDist = 10000000;
	
		for (int i = 0; i < 16; i++)
		{

			rgbVect console = getRGBFromConsoleIndex(i);
			rgbVect colorV = getRGBDirection(input, console);

			norm = getRGBNorm(colorV);

			if (norm)
			{
				float squaredDist = getRGBSquaredNorm(colorV);

				norm = 1.0f / norm;

				RGBMult(&colorV, norm);

				float dot = getRGBDot(colorV, direction);
				if (dot > 0.0)
				{
					squaredDist /= dot;

					if (squaredDist < bestSquaredDist)
					{
						bestSquaredDist = squaredDist;
						bestIndex = i;
					}
				}
			}
		}
	}
	return bestIndex;
}


DisplayCharacter	GetAsciiArtFromRGB(unsigned char R, unsigned char G, unsigned char B)
{
	unsigned int nearest = getNearestConsoleColor(R, G, B);

	unsigned int opposite = getOrientedNearestConsoleColor(R, G, B, nearest);

	DisplayCharacter result;
	if(opposite != 16)
	{
		rgbVect current = getRGBFromValues(R, G, B);
		rgbVect	nearestrgb = getRGBFromConsoleIndex(nearest);
		rgbVect	oppositergb = getRGBFromConsoleIndex(opposite);

		rgbVect	v1 = getRGBDirection(nearestrgb, oppositergb);
		rgbVect	v2 = getRGBDirection(nearestrgb, current);

		float	dot = getRGBDot(v1, v2);
		dot /= getRGBSquaredNorm(v1);

		// dot is in [0,0.5]

		int blendIndex = 10.99f * 2.0f*dot;

		DisplayCharacter blend =  ENCODE_DISPLAY_CHARACTER(opposite,nearest,greyRamp[blendIndex],0) ;
		result = blend;
	}
	else
	{
		DisplayCharacter blend = ENCODE_DISPLAY_CHARACTER(BLACK, nearest, ' ', 0);
		result = blend;
	}
	return result;
}
