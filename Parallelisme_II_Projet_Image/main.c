#include "ppm_lib.h"
#include "filter_lib.h"

int main(int argc, char* argv[]) {

	Filter* filter_SobelVertical = importFilter("sobel_vertical");
	PPMImage* image_Baboon = importPPM("Baboon.512");

	return 0;
}