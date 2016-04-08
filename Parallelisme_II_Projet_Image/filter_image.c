#include "filter_image.h"

void FilterApply(char * filtername, Uint * image, Uint sizeX, Uint sizeY)
{
	int x, y, x2, y2, gridCounter;
	Uint *filter, *final;
	
	// TODO : load filter from file

	for (y = 0; y <= sizeX; y++) // for each pixel in the image
		for (x = 0; x <= sizeY; x++)
		{ gridCounter = 0; // reset some values
	final = 0;
	for (y2 = -2; y2 <= 2; y2++) // and for each pixel around our
		for (x2 = -2; x2 <= 2; x2++) // "hot pixel"...
		{ // Add to our running total
		final += image[x + x2][y + y2] * filter[gridCounter];
		// Go to the next value on the filter grid
			gridCounter++;
		}
	// and put it back into the right range
	final /= divisionFactor;
	destination[x][y] = final;
	}
}
