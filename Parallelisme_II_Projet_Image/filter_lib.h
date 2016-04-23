#pragma once

#include "ppm_lib.h"

#define MAX_DIGITS_ALLOWED 8
#define FILTERS_FOLDER_PATH "./res/filters/"
#define FILTERS_FILE_EXTEND ".filter"

typedef unsigned int Uint32;

typedef struct {
	char* name;
	Uint32 w, h;
	int *data;
} PPMFilter;

void applyFilter(PPMImage* image, PPMFilter* filter);
PPMFilter* importFilter(const char* filterName);
//void deleteFilter(Filter* filter);