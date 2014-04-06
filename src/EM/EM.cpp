#include <iostream>

#include "itkImage.h"
#include "itkVector.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <itkComposeImageFilter.h>

#include "itkGaussianMixtureModelComponent.h"
#include "itkExpectationMaximizationMixtureModelEstimator.h"

#include "itkImageToListSampleAdaptor.h"

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

	unsigned int numberOfImages = atoi(argv[1]);
	const    unsigned int    InputDimension = 3;
	typedef double InputPixelType;
	const unsigned int numberOfClasses = 2;

	typedef itk::Image<InputPixelType, InputDimension>  InputImageType;
	typedef itk::ImageFileReader<InputImageType> ReaderType;

	typedef itk::VectorImage<InputPixelType, InputDimension>  VectorImageType;
	typedef itk::ComposeImageFilter<InputImageType> ImageToVectorImageFilterType;
	ImageToVectorImageFilterType::Pointer composeFilter = ImageToVectorImageFilterType::New();

	for (unsigned int i = 0; i < numberOfImages; i++){
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

	std::cout << "EM." << std::endl;

	typedef itk::Statistics::ImageToListSampleAdaptor< VectorImageType > SampleType;
	SampleType::Pointer sample = SampleType::New();
	sample->SetImage(composeFilter->GetOutput());
	sample->Update();

	typedef itk::Array< double > ParametersType;
	ParametersType params(5);
	std::vector< ParametersType > initialParameters(numberOfClasses);
	params[0] = 110.0;
	params[1] = 1.0;
	params[2] = 0.5;
	params[3] = 0.5;
	params[4] = 1.0;

	initialParameters[0] = params;
	params[0] = 210.0;
	params[1] = 1.0;
	params[2] = 0.5;
	params[3] = 0.5;
	params[4] = 1.0;

	initialParameters[1] = params;
	typedef itk::Statistics::GaussianMixtureModelComponent< SampleType >
		ComponentType;
	std::vector< ComponentType::Pointer > components;
	for (unsigned int i = 0; i < numberOfClasses; i++)
	{
		components.push_back(ComponentType::New());
		(components[i])->SetSample(sample);
		(components[i])->SetParameters(initialParameters[i]);
	}

	typedef itk::Statistics::ExpectationMaximizationMixtureModelEstimator<
		SampleType > EstimatorType;
	EstimatorType::Pointer estimator = EstimatorType::New();
	estimator->SetSample(sample);
	estimator->SetMaximumIteration(200);
	itk::Array< double > initialProportions(numberOfClasses);
	initialProportions[0] = 0.5;
	initialProportions[1] = 0.5;
	estimator->SetInitialProportions(initialProportions);
	for (unsigned int i = 0; i < numberOfClasses; i++)
	{
		estimator->AddComponent((ComponentType::Superclass*)
			(components[i]).GetPointer());
	}
	estimator->Update();

	for (unsigned int i = 0; i < numberOfClasses; i++)
	{
		std::cout << "Cluster[" << i << "]" << std::endl;
		std::cout << " Parameters:" << std::endl;
		std::cout << " " << (components[i])->GetFullParameters()
			<< std::endl;
		std::cout << " Proportion: ";
		std::cout << " " << estimator->GetProportions()[i] << std::endl;
	}
	
	return 0;
}
