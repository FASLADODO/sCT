#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkNormalizeImageFilter.h"
#include "itkStatisticsImageFilter.h"


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

	std::cout << "Normalizing." << std::endl;

	typedef itk::NormalizeImageFilter<
		InputImageType, InputImageType > NormalizeFilterType;
	NormalizeFilterType::Pointer normalizeFilter = NormalizeFilterType::New();
	normalizeFilter->SetInput(reader->GetOutput());

    typedef itk::StatisticsImageFilter< InputImageType >
    StatisticsFilterType;
    StatisticsFilterType::Pointer statistics = StatisticsFilterType::New();
    statistics->SetInput(normalizeFilter->GetOutput());
    statistics->Update();
    
	std::cout << "Writing image." << std::endl;

	typedef itk::ImageFileWriter<InputImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(normalizeFilter->GetOutput());
	writer->SetFileName(argv[2]);
	writer->Update();

	return 0;
}
