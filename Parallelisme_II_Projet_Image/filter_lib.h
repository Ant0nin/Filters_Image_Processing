#pragma once

#include "ppm_lib.h"

#define MAX_DIGITS_ALLOWED 8
#define FILTERS_FOLDER_PATH "./res/filters/"
#define FILTERS_FILE_EXTEND ".filter"

typedef unsigned int Uint32;

typedef struct Filter {
	char* name;
	Uint32 w;
	Uint32 h;
	int *data;
} Filter;

typedef enum AccelerationMode {
	NONE,
	OPEN_MP,
	CUDA
} AccelerationMode;

void applyFilter(PPMImage* image, Filter* filter, AccelerationMode accelerationMode);
Filter importFilter(char* filterName);
//void deleteFilter(Filter* filter);