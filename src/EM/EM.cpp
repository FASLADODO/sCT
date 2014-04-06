#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <itkComposeImageFilter.h>

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << std::endl;
		std::cout << "Too few arguments. " << argc - 1 << " supplied, needs at least 4." << std::endl << std::endl;
		std::cout << "Usage: " << std::endl;
		std::cout << "    Arg 1: Image to calculate mean from." << std::endl;
		std::cout << "    Arg 2: Output image containing means." << std::endl;
		return -1;
	}

	const int numberOfImages = atoi(argv[1]);
	const    unsigned int    InputDimension = 3;
	typedef float InputPixelType;
	typedef itk::Image<InputPixelType, InputDimension>  InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;


	typedef itk::VectorImage<InputPixelType, InputDimension>  VectorImageType;
	typedef itk::ComposeImageFilter<InputImageType> FilterType;
	FilterType::Pointer composeFilter = FilterType::New();

	for (int i = 0; i < numberOfImages; i++){
		std::cout << "Reading image " << i << " ." << std::endl;

		ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName(argv[i+2]);
		reader->Update();
		composeFilter->SetInput(i, reader->GetOutput());
	};

	std::cout << "Composing image." << std::endl;
	composeFilter->Update();

	std::cout << "Writing image." << std::endl;

	typedef itk::ImageFileWriter<VectorImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(composeFilter->GetOutput());
	writer->SetFileName("vectorimage.nii");
	writer->Update();

	return 0;
}
