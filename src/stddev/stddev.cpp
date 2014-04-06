#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkNoiseImageFilter.h"

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

	const    unsigned int    InputDimension = 3;
	typedef float InputPixelType;
	typedef itk::Image<InputPixelType, InputDimension>  InputImageType;


	std::cout << "Reading image." << std::endl;

	typedef itk::ImageFileReader<InputImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(argv[1]);
	reader->Update();

	std::cout << "Calculating means." << std::endl;

	typedef itk::NoiseImageFilter<
		InputImageType, InputImageType > FilterType;
	FilterType::Pointer noiseFilter = FilterType::New();
	noiseFilter->SetInput(reader->GetOutput());
	InputImageType::SizeType indexRadius;
	indexRadius[0] = 1; // radius along x
	indexRadius[1] = 1; // radius along y
	indexRadius[2] = 1; // radius along z
	noiseFilter->SetRadius(indexRadius);
	noiseFilter->Update();

	std::cout << "Writing image." << std::endl;

	typedef itk::ImageFileWriter<InputImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(noiseFilter->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();

	return 0;
}
