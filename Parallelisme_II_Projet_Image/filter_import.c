#include "filter_lib.h"
#include <stdio.h>
#include <stdlib.h>

PPMFilter* importFilter(const char *filterName)
{
	FILE* pFile;
	char buffer[MAX_DIGITS_ALLOWED + 2];
	char path[64] = FILTERS_FOLDER_PATH;
	strcat(path, filterName);
	strcat(path, FILTERS_FILE_EXTEND);
	pFile = fopen(path, "r");
	if (pFile == NULL)
		perror("Error opening file");

	PPMFilter *filter = (PPMFilter*)malloc(sizeof(PPMFilter));
	filter->name = filterName;
	filter->w = atoi(fgets(buffer, MAX_DIGITS_ALLOWED, pFile));
	filter->h = atoi(fgets(buffer, MAX_DIGITS_ALLOWED, pFile));
	filter->data = (int*)malloc(sizeof(int) * filter->w * filter->h);

	for (Uint32 j = 0; j < filter->h; j++) {
		for (Uint32 i = 0; i < filter->w; i++) {

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
