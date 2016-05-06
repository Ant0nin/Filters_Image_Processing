extern "C" {
#include "filter_lib.h"
#include "ppm_lib.h"
}

#define BLOCK_WIDTH 32

__global__ void kernelFilterConvolution(const PPMImage *inputImage, const PPMFilter *filter, PPMPixel *outputImageData)
{
	const int imageWidth = inputImage->w;
	const int imageHeight = inputImage->h;
	const int filterWidth = filter->w;
	const int filterHeight = filter->h;

	int pixelCoordX = blockIdx.y * blockDim.y + threadIdx.y;
	int pixelCoordY = blockIdx.x * blockDim.x + threadIdx.x;

	if((pixelCoordX >= imageWidth) || (pixelCoordY >= imageHeight))
		return;

	int halfW = (filterWidth - 1) >> 1;
	int halfH = (filterHeight - 1) >> 1;
	int k = imageWidth - 1;
	int l = imageHeight - 1;

	int sumR = 0;
	int sumG = 0;
	int sumB = 0;

	int divisionFactor = 0;

	for (int i = -halfW; i <= halfW; i++)
	{
		for (int j = -halfH; j <= halfH; j++)
		{
			int x = min(max(pixelCoordX + i, 0), k);
			int y = min(max(pixelCoordY + j, 0), l);
			int pixelIndex = y * imageWidth + x;
			PPMPixel currentPixel = inputImage->data[pixelIndex];

			int weightIndex = (halfW + i) * filterWidth + (halfH + j);
			int currentWeight = filter->data[weightIndex];

			divisionFactor += currentWeight;
			sumR += (currentPixel.r) * currentWeight;
			sumG += (currentPixel.g) * currentWeight;
			sumB += (currentPixel.b) * currentWeight;
		}
	}	

	PPMPixel newPixel;
	newPixel.r = (Uint8)(sumR / divisionFactor);
	newPixel.g = (Uint8)(sumG / divisionFactor);
	newPixel.b = (Uint8)(sumB / divisionFactor);

	outputImageData[pixelCoordY * imageWidth + pixelCoordX] = newPixel;
}

extern "C"
void applyFilter(PPMImage *image, PPMFilter *filter)
{
	dim3 gridDim;
	gridDim.x = (int)((float)image->w / (float)BLOCK_WIDTH); //+1;
	gridDim.y = (int)((float)image->h / (float)BLOCK_WIDTH); //+1;
	gridDim.z = 1;

	dim3 blockDim;
	blockDim.x = BLOCK_WIDTH;
	blockDim.y = BLOCK_WIDTH;
	blockDim.z = 1;

	PPMImage *deviceInputImage;
	PPMPixel *deviceInputImageData;
	PPMFilter *deviceFilter;
	int* deviceFilterData;
	PPMPixel *deviceOutputImageData;

	int* originalFilterDataPtr = filter->data;
	PPMPixel* originalImageDataPtr = image->data;

	int imageDataSize = image->w*image->h*sizeof(PPMPixel);
	int filterDataSize = filter->w*filter->h*sizeof(int);
	
	cudaError_t check;

	check = cudaMalloc(&deviceInputImageData, imageDataSize);
	check = cudaMemcpy(deviceInputImageData, image->data, imageDataSize, cudaMemcpyHostToDevice);
	check = cudaMalloc(&deviceInputImage, sizeof(PPMImage));
	image->data = deviceInputImageData;
	check = cudaMemcpy(deviceInputImage, image, sizeof(PPMImage), cudaMemcpyHostToDevice);
	
	check = cudaMalloc(&deviceFilterData, filterDataSize);
	check = cudaMemcpy(deviceFilterData, filter->data, filterDataSize, cudaMemcpyHostToDevice);
	check = cudaMalloc(&deviceFilter, sizeof(PPMFilter));
	filter->data = deviceFilterData;
	check = cudaMemcpy(deviceFilter, filter, sizeof(PPMFilter), cudaMemcpyHostToDevice);
	filter->data = originalFilterDataPtr;

	check = cudaMalloc(&deviceOutputImageData, imageDataSize);

	kernelFilterConvolution <<<gridDim, blockDim >>> (deviceInputImage, deviceFilter, deviceOutputImageData);
	cudaDeviceSynchronize();

	image->data = originalImageDataPtr;
	check = cudaMemcpy(image->data, deviceOutputImageData, imageDataSize, cudaMemcpyDeviceToHost);

	check = cudaFree(deviceInputImageData);
	check = cudaFree(deviceInputImage);
	check = cudaFree(deviceFilterData);
	check = cudaFree(deviceFilter);
	check = cudaFree(deviceOutputImageData);
}
