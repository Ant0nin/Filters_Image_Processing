#pragma once

#define RGB_COMPONENT_COLOR 255

typedef unsigned char Uint8;

typedef struct {
	Uint8 r, g, b;
} PPMPixel;

typedef struct {
    int h, w;
    PPMPixel *data;
} PPMImage;

PPMImage *readPPM(const char *filename);
void writePPM(const char *filename, PPMImage *img);