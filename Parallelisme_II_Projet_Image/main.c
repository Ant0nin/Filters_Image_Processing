#include "ppm_lib.h"
#include "filter_lib.h"

int main(int argc, char* argv[]) {

	const char* inputFilename = "gare_parallelisme2";
	const char* outputFilename = "_output";
	const char* filterName = "softer";
	const Uint32 iterations = 10;

	PPMImage* image_Baboon = importPPM(inputFilename);
	PPMFilter* filter_SobelVertical = importFilter(filterName);
	for (Uint32 i = 0; i < iterations; i++)
		applyFilter(image_Baboon, filter_SobelVertical);
	exportPPM(outputFilename, image_Baboon);

	return 0;
}