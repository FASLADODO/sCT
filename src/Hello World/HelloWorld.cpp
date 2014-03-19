#include <iostream>
#include "itkImageFileReader.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkExtractImageFilter.h"

#include "QuickView.h"

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		return -1;
	}

	std::cout << "ITK Hello World !" << std::endl;

	typedef float InputPixelType;
	typedef float OutputPixelType;
	typedef itk::Image< OutputPixelType, 2 > OutputImageType;

	typedef itk::Image<InputPixelType, 3>  InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();

	typedef itk::ExtractImageFilter< InputImageType,
		OutputImageType > FilterType;
	FilterType::Pointer filter = FilterType::New();
	filter->InPlaceOn();
	filter->SetDirectionCollapseToSubmatrix();

	reader->SetFileName(argv[1]);
	reader->Update();


	InputImageType* inputImage = reader->GetOutput();

	InputImageType::RegionType inputRegion =
		reader->GetOutput()->GetLargestPossibleRegion();

	InputImageType::SizeType size = inputRegion.GetSize();
	size[2] = 0;

	InputImageType::IndexType start = inputRegion.GetIndex();
	const unsigned int sliceNumber = 50;
	start[2] = sliceNumber;

	InputImageType::RegionType desiredRegion;
	desiredRegion.SetSize(size);
	desiredRegion.SetIndex(start);

	filter->SetExtractionRegion(desiredRegion);

	filter->SetInput(inputImage);

	OutputImageType* outputImage = filter->GetOutput();

	typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(outputImage);
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	QuickView viewer;
	viewer.AddImage(outputImage);
	viewer.AddImage(rescaleFilter->GetOutput());
	viewer.Visualize();

	return 0;
}