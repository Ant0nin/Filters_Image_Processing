#include <stdio.h>
#include <stdlib.h>
#include "filter_lib.h"
#include "ppm_lib.h"

void applyFilter(PPMImage *image, PPMFilter *filter)
{
	const int halfW = (filter->w - 1) >> 1;
	const int halfH = (filter->h - 1) >> 1;
	int divisionFactor = 0;
	Uint32 tempPixel[3]; // RGB

	PPMImage *bufferImage = clonePPM(image);
	int x, y, i, j;

	#pragma omp parallel for shared(divisionFactor)
	for (y = 0; y < filter->h; y++) {
		for (int x = 0; x < filter->w; x++)
			divisionFactor += filter->data[x + y*filter->w];
		}

	int higherSize = image->w > image->h ? image->w : image->h;

	#pragma omp parallel for schedule(static, higherSize) shared(image)
	for (y = halfH; y < (image->h - halfW); y++) {
		for (int x = halfW; x < (image->w - halfW); x++)
		{
			int offsetFilter = 0;
			tempPixel[0] = 0;
			tempPixel[1] = 0;
			tempPixel[2] = 0;

			for (j = -halfH; j <= halfH; j++)
				for (i = -halfW; i <= halfW; i++)
				{
					int offsetImage = (x + image->w * y) + (i + image->w * j);

					tempPixel[0] += bufferImage->data[offsetImage].r * filter->data[offsetFilter]; // R
					tempPixel[1] += bufferImage->data[offsetImage].g * filter->data[offsetFilter]; // G
					tempPixel[2] += bufferImage->data[offsetImage].b * filter->data[offsetFilter]; // B

					offsetFilter++;
				}
			if (divisionFactor == 0) divisionFactor = 1;
			tempPixel[0] /= divisionFactor;
			tempPixel[1] /= divisionFactor;
			tempPixel[2] /= divisionFactor;

			PPMPixel total;
			total.r = (Uint8)tempPixel[0];
			total.g = (Uint8)tempPixel[1];
			total.b = (Uint8)tempPixel[2];

			image->data[x + image->w * y] = total;
		}
	}
	
	freePPM(bufferImage);
}
