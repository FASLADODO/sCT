#include <iostream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkOtsuThresholdImageFilter.h"

#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

#include "itkNeighborhoodConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkMaskImageFilter.h"

class CommandIterationUpdate : public itk::Command
{
public:
	typedef CommandIterationUpdate   Self;
	typedef itk::Command             Superclass;
	typedef itk::SmartPointer<Self>  Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate() {};

public:

	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef const OptimizerType*                     OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		OptimizerPointer optimizer =
			dynamic_cast<OptimizerPointer>(object);

		if (!itk::IterationEvent().CheckEvent(&event))
		{
			return;
		}

		std::cout << optimizer->GetCurrentIteration() << " = ";
		std::cout << optimizer->GetValue() << " : ";
		std::cout << optimizer->GetCurrentPosition() << std::endl;
	}

};

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Too few arguments." << argc - 1 << "supplied, needs at least 2." << std::endl;
		char str[2];
		const char* format = "%1s";
		scanf(format, str);
		return -1;
	}

	const    unsigned int    InputDimension = 3;
	typedef float InputPixelType;
	typedef itk::Image<InputPixelType, InputDimension>  InputImageType;

	typedef itk::TranslationTransform< double, InputDimension > TransformType;

	typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;

	typedef itk::MattesMutualInformationImageToImageMetric<
		InputImageType,
		InputImageType >    MetricType;

	typedef itk::LinearInterpolateImageFunction<
		InputImageType,
		double          >    InterpolatorType;

	typedef itk::ImageRegistrationMethod<
		InputImageType,
		InputImageType >    RegistrationType;

	MetricType::Pointer         metric = MetricType::New();
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();

	unsigned int numberOfBins = 24;
	unsigned int numberOfSamples = 10000;
	metric->SetNumberOfHistogramBins(numberOfBins);
	metric->SetNumberOfSpatialSamples(numberOfSamples);

	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetTransform(transform);
	registration->SetInterpolator(interpolator);

	std::cout << "Reading information." << std::endl;

	typedef itk::ImageFileReader<InputImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	ReaderType::Pointer reader2 = ReaderType::New();

	reader->SetFileName(argv[1]);
	reader->Update();
	reader2->SetFileName(argv[2]);
	reader2->Update();

	InputImageType* fixedImage = reader->GetOutput();
	InputImageType* floatingImage = reader2->GetOutput();

	std::cout << "Starting registration." << std::endl;

	registration->SetFixedImage(fixedImage);
	registration->SetMovingImage(floatingImage);

	typedef RegistrationType::ParametersType ParametersType;
	ParametersType initialParameters(transform->GetNumberOfParameters());

	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y
	initialParameters[2] = 0.0;  // Initial offset in mm along Z

	registration->SetInitialTransformParameters(initialParameters);

	optimizer->SetMaximumStepLength(1.00);
	optimizer->SetMinimumStepLength(0.01);
	optimizer->MinimizeOn();
	optimizer->SetNumberOfIterations(200);

	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);
	
	registration->Update();

	ParametersType finalParameters = registration->GetLastTransformParameters();

	const double TranslationAlongX = finalParameters[0];
	const double TranslationAlongY = finalParameters[1];
	const double TranslationAlongZ = finalParameters[2];

	const unsigned int numberOfIterations = optimizer->GetCurrentIteration();

	const double bestValue = optimizer->GetValue();

	std::cout << "Result = " << std::endl;
	std::cout << " Translation X = " << TranslationAlongX << std::endl;
	std::cout << " Translation Y = " << TranslationAlongY << std::endl;
	std::cout << " Translation Z = " << TranslationAlongZ << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	typedef itk::ResampleImageFilter<
		InputImageType,
		InputImageType >    ResampleFilterType;

	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetInput(floatingImage);
	resampler->SetTransform(registration->GetOutput()->Get());
	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(0);


	std::cout << "Generating Otsu threshold image." << std::endl;

	typedef itk::OtsuThresholdImageFilter< InputImageType, InputImageType >
		OtsuFilterType;
	OtsuFilterType::Pointer otsuFilter = OtsuFilterType::New();
	otsuFilter->SetInput(fixedImage);
	otsuFilter->SetOutsideValue(0);
	otsuFilter->SetInsideValue(255);
	otsuFilter->Update();


	// NNC
	typedef itk::CurvatureFlowImageFilter< InputImageType, InputImageType> CurvatureFlowImageFilterType;

	CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

	typedef itk::NeighborhoodConnectedImageFilter<InputImageType, InputImageType > ConnectedFilterType;


	ConnectedFilterType::Pointer neighborhoodConnected = ConnectedFilterType::New();

	typedef itk::CastImageFilter< InputImageType, InputImageType >
		CastingFilterType;
	CastingFilterType::Pointer caster = CastingFilterType::New();


	std::cout << "Generating neighborhood connected image." << std::endl;

	smoothing->SetInput(otsuFilter->GetOutput());
	neighborhoodConnected->SetInput(smoothing->GetOutput());
	caster->SetInput(neighborhoodConnected->GetOutput());
	neighborhoodConnected->Update();

	smoothing->SetNumberOfIterations(5);
	smoothing->SetTimeStep(0.125);

	neighborhoodConnected->SetLower(254);
	neighborhoodConnected->SetUpper(255);

	InputImageType::SizeType radius;

	radius[0] = 2;   // two pixels along X
	radius[1] = 2;   // two pixels along Y
	radius[2] = 3;   // two pixels along Z

	neighborhoodConnected->SetRadius(radius);

	InputImageType::IndexType  index;

	index[0] = 0;
	index[1] = 0;
	index[2] = 0;

	neighborhoodConnected->SetSeed(index);
	neighborhoodConnected->SetReplaceValue(255);

	typedef itk::InvertIntensityImageFilter< InputImageType, InputImageType >
		InvertFilterType;
	InvertFilterType::Pointer invertFilter = InvertFilterType::New();
	invertFilter->SetInput(caster->GetOutput());
	invertFilter->SetMaximum(255);

	std::cout << "Generating mask." << std::endl;

	typedef itk::ImageFileWriter<InputImageType> WriterType;

	WriterType::Pointer writer3 = WriterType::New();
	writer3->SetInput(otsuFilter->GetOutput());
	writer3->SetFileName("otsu.nii");
	writer3->Update();

	if (argc < 4)
	{
		WriterType::Pointer writer2 = WriterType::New();
		writer2->SetInput(caster->GetOutput());
		writer2->SetFileName("mask.nii");
		writer2->Update();
	}

	ReaderType::Pointer maskreader = ReaderType::New();
	maskreader->SetFileName("mask.nii");
	maskreader->Update();

	typedef itk::MaskImageFilter< InputImageType, InputImageType >
		MaskImageType;
	MaskImageType::Pointer maskFilter = MaskImageType::New();
	maskFilter->SetInput(resampler->GetOutput());
	maskFilter->SetMaskImage(maskreader->GetOutput());
	maskFilter->Update();

	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(maskFilter->GetOutput());
	writer->SetFileName("out.nii");
	writer->Update();

	return 0;
}
