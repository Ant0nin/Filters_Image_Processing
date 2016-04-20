#include <stdio.h>
#include "filter_lib.h"
#include "ppm_lib.h"

void applyFilter(PPMImage *image, Filter *filter)
{
	int halfW = (filter->w - 1) >> 1;
	int halfH = (filter->h - 1) >> 1;
	int divisionFactor = 0;
	PPMPixel total;

	PPMImage *bufferImage = clonePPM(image);

	for (int x = 0; x < filter->w; x++)
		for (int y = 0; y < filter->h; y++) {
			divisionFactor += filter->data[x + y*filter->w];
		}

	for (int x = halfW; x <= (image->w - halfW); x++)
		for (int y = halfH; y <= (image->h - halfW); y++)
		{
			int offsetFilter = 0;
			total.r = 0; total.g = 0; total.b = 0;

			for (int j = -halfH; j <= halfH; j++) 
				for (int i = -halfW; i <= halfW; i++)
				{
					int offsetImage = (x + image->w * y) + (i + image->w * j);
					total.r += bufferImage->data[offsetImage].r * filter->data[offsetFilter];
					total.g += bufferImage->data[offsetImage].g * filter->data[offsetFilter];
					total.b += bufferImage->data[offsetImage].b * filter->data[offsetFilter];
					offsetFilter++;
				}
			total.r /= divisionFactor;
			total.g /= divisionFactor;
			total.b /= divisionFactor;
			image->data[x + image->w * y] = total;
		}
	
	freePPM(bufferImage);
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
