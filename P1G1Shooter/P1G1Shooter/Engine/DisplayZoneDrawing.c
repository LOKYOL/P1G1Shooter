#include <math.h>
#include "DisplayZoneDrawing.h"

#define ABS_INT(a) ((a<0)?(-(a)):(a))


unsigned char	greyRamp[] = " .:-=+*#%@";
unsigned int	RGB_ConsoleColorsValues[16] = { 0x0C0C0C,0xC50F1F,0x13A1E0,0xC19C00,0x0037DA,0x881798,0x3A96DD,0xCCCCCC,
												0x767676,0xE74856,0x16C60C,0xF9F1A5,0x3B78FF,0xB4009E,0x61D6D6,0xF2F2F2 };


typedef struct rgbVect
{
	int R;
	int G;
	int B;

}rgbVect;

rgbVect	RGB_ConsoleColors[16];

extern void	Blender(DisplayCharacter* src, DisplayCharacter* dst, DisplayCharacter* blender);

// draw a filled rectangle with given pos, size, border and fill DisplayCharacter
void	DrawRectangleInDisplayZoneDisplayCharacter(DisplayZone* zone, int px, int py, int sx, int sy, DisplayCharacter* fill, DisplayCharacter* border)
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
}

// draw a filled rectangle with given pos, size, border and fill colors and character
void	DrawRectangleInDisplayZoneEx(DisplayZone* zone, int px, int py, int sx, int sy, ConsoleColors fillFG, ConsoleColors fillBG, char fillCharacter, ConsoleColors borderFG, ConsoleColors borderBG, char borderCharacter)
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

	DrawRectangleInDisplayZoneDisplayCharacter(zone, px, py, sx, sy, &fillPixel, &borderPixel);
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



rgbVect getRGBFromConsoleIndex(unsigned int index)
{
	rgbVect	result;
	result.R = (int)(RGB_ConsoleColorsValues[index] >> 16);
	result.G = (int)((RGB_ConsoleColorsValues[index] >> 8) & 0xFF);
	result.B = (int)(RGB_ConsoleColorsValues[index] & 0xFF);
	return result;
}
void initRGBConsoleColor()
{
	for (int i = 0; i < 16; i++)
	{
		RGB_ConsoleColors[i] = getRGBFromConsoleIndex(i);
	}
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

unsigned int	getRGBSquaredNorm(rgbVect c)
{
	return c.R * c.R + c.G * c.G + c.B * c.B;
}

int	getRGBDot(rgbVect c1,rgbVect c2)
{
	return c1.R * c2.R + c1.G * c2.G + c1.B * c2.B;
}

// 3D distance, don't take visual distance into account
unsigned int	getNearestConsoleColor(rgbVect input)
{
	unsigned int bestSquaredDist = 3*255*255;
	unsigned int bestIndex = -1;
	for (int i = 0; i < 16; i++)
	{
		rgbVect diff = getRGBDirection(RGB_ConsoleColors[i], input);

		unsigned int squaredDist = getRGBSquaredNorm(diff);
		if (squaredDist < bestSquaredDist)
		{
			bestSquaredDist = squaredDist;
			bestIndex = i;
		}
	}
	return bestIndex;
}

// 3D oriented distance, don't take visual distance into account
unsigned int	getOrientedNearestConsoleColor(rgbVect input,unsigned int nearestIndex)
{
	rgbVect direction = getRGBDirection(RGB_ConsoleColors[nearestIndex], input);

	unsigned int bestIndex = 16;

	int sqrnorm = getRGBSquaredNorm(direction);
	if (sqrnorm)
	{
		unsigned int bestSquaredDist = 10000000;
		
		for (int i = 0; i < 16; i++)
		{
			if (i == nearestIndex)
				continue;

			rgbVect colorV = getRGBDirection(input, RGB_ConsoleColors[i]);

			int dot = 256*getRGBDot(colorV, direction);
			if (dot > 0.0)
			{
				dot /= sqrnorm;
				rgbVect proj = direction;
				proj.R *= dot;
				proj.R /= 256;
				proj.G *= dot;
				proj.G /= 256;
				proj.B *= dot;
				proj.B /= 256;

				rgbVect perp = getRGBDirection(proj, colorV);
				perp.R *= 4;
				perp.G *= 4;
				perp.B *= 4;

				unsigned int dist = getRGBSquaredNorm(proj) + getRGBSquaredNorm(perp);
				
				if (dist < bestSquaredDist)
				{
					bestSquaredDist = dist;
					bestIndex = i;
				}
			}
			
		}
	}
	return bestIndex;
}


DisplayCharacter	GetAsciiArtFromRGB(unsigned char R, unsigned char G, unsigned char B)
{
	rgbVect input = getRGBFromValues(R, G, B);

	unsigned int nearest = getNearestConsoleColor(input);
	unsigned int opposite = getOrientedNearestConsoleColor(input, nearest);

	DisplayCharacter result;
	if(opposite != 16)
	{
		rgbVect	v1 = getRGBDirection(RGB_ConsoleColors[nearest], RGB_ConsoleColors[opposite]);
		rgbVect	v2 = getRGBDirection(RGB_ConsoleColors[nearest], input);

		int	dot = 65536*getRGBDot(v1, v2);
		dot /= getRGBSquaredNorm(v1);

		// dot is in [0,0.5]

		int blendIndex = (11 * dot) / 32768;

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