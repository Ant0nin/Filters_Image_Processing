#pragma once

#define RGB_COMPONENT_COLOR 255
#define PPM_IMAGES_FOLDER_PATH "./res/images/"
#define PPM_IMAGES_FILE_EXTEND ".ppm"

typedef unsigned char Uint8;

typedef struct {
	Uint8 r, g, b;
} PPMPixel;

typedef struct {
    int h, w;
    PPMPixel *data;
} PPMImage;

PPMImage *importPPM(const char *imageName);
void exportPPM(const char *imageName, PPMImage *img);
PPMImage* clonePPM(PPMImage *image);
void freePPM(PPMImage *image);