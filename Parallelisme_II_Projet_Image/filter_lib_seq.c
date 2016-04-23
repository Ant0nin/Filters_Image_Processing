#include <stdio.h>
#include "filter_lib.h"
#include "ppm_lib.h"

void applyFilter(PPMImage *image, Filter *filter)
{
	const int halfW = (filter->w - 1) >> 1;
	const int halfH = (filter->h - 1) >> 1;
	int divisionFactor = 0;
	int tempPixel[3]; // RGB

	PPMImage *bufferImage = clonePPM(image);

	for (int y = 0; y < filter->h; y++) {
		for (int x = 0; x < filter->w; x++)
			divisionFactor += filter->data[x + y*filter->w];
		}

	for (int y = halfH; y < (image->h - halfW); y++)
		for (int x = halfW; x < (image->w - halfW); x++)
		{
			int offsetFilter = 0;
			tempPixel[0] = 0;
			tempPixel[1] = 0;
			tempPixel[2] = 0;

			for (int j = -halfH; j <= halfH; j++) 
				for (int i = -halfW; i <= halfW; i++)
				{
					int offsetImage = (x + image->w * y) + (i + image->w * j);
					tempPixel[0] += bufferImage->data[offsetImage].r * filter->data[offsetFilter]; // R
					tempPixel[1] += bufferImage->data[offsetImage].g * filter->data[offsetFilter]; // G
					tempPixel[2] += bufferImage->data[offsetImage].b * filter->data[offsetFilter]; // B
					offsetFilter++;
				}
			tempPixel[0] /= divisionFactor;
			tempPixel[1] /= divisionFactor;
			tempPixel[2] /= divisionFactor;

			PPMPixel total;
			total.r = tempPixel[0];
			total.g = tempPixel[1];
			total.b = tempPixel[2];

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

	Filter *filter = (Filter *)malloc(sizeof(Filter));
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
