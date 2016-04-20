#include "ppm_lib.h"
#include "filter_lib.h"

int main(int argc, char* argv[]) {

	PPMImage* image_Baboon = importPPM("Baboon.512");
	Filter* filter_SobelVertical = importFilter("softer");

	applyFilter(image_Baboon, filter_SobelVertical);

	exportPPM("Baboon_Softer.512", image_Baboon);

	return 0;
}