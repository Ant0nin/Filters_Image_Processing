#include <stdio.h>
#include "filter_lib.h"

void applyFilter(PPMImage * image, Filter * filter, AccelerationMode accelerationMode)
{
	/*int x, y, x2, y2, gridCounter;
	Uint *filter, *final;

	for (y = 0; y <= sizeX; y++) // for each pixel in the image
		for (x = 0; x <= sizeY; x++)
		{
			gridCounter = 0; // reset some values
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
		}*/
}

Filter* importFilter(const char *filterName)
{
	FILE* pFile;
	char buffer[MAX_DIGITS_ALLOWED + 2];
	char path[64] = FILTERS_FOLDER_PATH;
	strcat(path, filterName);
	strcat(path, FILTERS_FILE_EXTEND);
	pFile = fopen(path, "r");
	if (pFile == NULL) 
		perror("Error opening file");

	Filter *filter = (Filter*)malloc(sizeof(Filter));
	filter->name = filterName;
	filter->w = atoi(fgets(buffer, MAX_DIGITS_ALLOWED, pFile));
	filter->h = atoi(fgets(buffer, MAX_DIGITS_ALLOWED, pFile));
	filter->data = (int*)malloc(sizeof(int) * filter->w * filter->h);

	for (int j = 0; j < filter->h; j++) {
		for (int i = 0; i < filter->w; i++) {

			int k = 0;
			buffer[k] = fgetc(pFile);
			k++;

			while (1) {
				char c = fgetc(pFile);

				if (c == ' ' || c == '\n')
					break;
				else
					buffer[k] = c;
				k++;
			}
			buffer[k] = '\0';

			filter->data[i + j*filter->w] = atoi(buffer);
		}
	}

	fclose(pFile);

	return filter;
}
