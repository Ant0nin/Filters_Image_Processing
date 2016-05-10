#include "ppm_lib.h"
#include "filter_lib.h"
#include "time.h"

int main(int argc, char* argv[]) {

	if (argc != 5) {
		printf("Wrong input arguments, use following : Filtering.exe <inputImageName> <outputImageName> <filterName> <iterationsQuantity>\n");
		return -1;
	}
	else {
		const char* inputFilename = argv[1];
		const char* outputFilename = argv[2];
		const char* filterName = argv[3];
		const Uint32 iterationsQuantity = atoi(argv[4]);

		PPMImage* image_Baboon = importPPM(inputFilename);
		PPMFilter* filter_SobelVertical = importFilter(filterName);

		printf("Filter %s.filter applying %d times on image '%s.ppm'...\n", filterName, iterationsQuantity, inputFilename);
		clock_t start = clock();
		for (Uint32 i = 0; i < iterationsQuantity; i++)
			applyFilter(image_Baboon, filter_SobelVertical);
		clock_t end = clock();
		double time_taken = ((double)end - (double)start);
		printf("Time measurement : %f ms\n", time_taken);

		exportPPM(outputFilename, image_Baboon);
		printf("Output image exported as '%s.ppm'\n", outputFilename);

		return 0;
	}
}