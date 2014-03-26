#include <iostream>
#include "itkImageFileReader.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkOtsuThresholdImageFilter.h"

#include "itkExtractImageFilter.h"

#include "itkImageRegistrationMethod.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"

#include "itkSubtractImageFilter.h"
#include "itkImageDuplicator.h"

#include "QuickView.h"
#include "itkNeighborhoodConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"

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
			dynamic_cast< OptimizerPointer >(object);

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
		return -1;
	}

	const unsigned int sliceNumber = 89;

	std::cout << "ITK Hello World !" << std::endl;

	const    unsigned int    InputDimension = 3;
	const    unsigned int    OutputDimension = 2;
	typedef float InputPixelType;
	typedef float OutputPixelType;
	typedef itk::Image< OutputPixelType, OutputDimension > OutputImageType;
	typedef itk::Image<InputPixelType, InputDimension>  InputImageType;

	typedef itk::TranslationTransform< double, InputDimension > TransformType;

	typedef itk::RegularStepGradientDescentOptimizer       OptimizerType;

	typedef itk::MeanSquaresImageToImageMetric<
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

	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);
	registration->SetTransform(transform);
	registration->SetInterpolator(interpolator);

	typedef itk::ImageFileReader<InputImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	ReaderType::Pointer reader2 = ReaderType::New();

	typedef itk::ExtractImageFilter< InputImageType,
		OutputImageType > FilterType;
	FilterType::Pointer filter = FilterType::New();
	filter->InPlaceOn();
	filter->SetDirectionCollapseToSubmatrix();

	reader->SetFileName(argv[1]);
	reader->Update();


	InputImageType* inputImage = reader->GetOutput();

	reader2->SetFileName(argv[2]);
	reader2->Update();

	registration->SetFixedImage(reader->GetOutput());
	registration->SetMovingImage(reader2->GetOutput());

	typedef RegistrationType::ParametersType ParametersType;
	ParametersType initialParameters(transform->GetNumberOfParameters());

	initialParameters[0] = 0.0;  // Initial offset in mm along X
	initialParameters[1] = 0.0;  // Initial offset in mm along Y
	initialParameters[2] = 0.0;  // Initial offset in mm along Z

	registration->SetInitialTransformParameters(initialParameters);

	optimizer->SetMaximumStepLength(4.00);
	optimizer->SetMinimumStepLength(0.01);

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
	resampler->SetInput(reader2->GetOutput());
	resampler->SetTransform(registration->GetOutput()->Get());
	InputImageType::Pointer fixedImage = reader->GetOutput();
	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);

	InputImageType::RegionType inputRegion =
		reader->GetOutput()->GetLargestPossibleRegion();

	InputImageType::SizeType size = inputRegion.GetSize();
	size[2] = 0;

	InputImageType::IndexType start = inputRegion.GetIndex();
	start[2] = sliceNumber;

	InputImageType::RegionType desiredRegion;
	desiredRegion.SetSize(size);
	desiredRegion.SetIndex(start);

	filter->SetExtractionRegion(desiredRegion);

	filter->SetInput(inputImage);
	filter->Update();

	typedef  itk::ImageDuplicator< OutputImageType > DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	duplicator->SetInputImage(filter->GetOutput());
	duplicator->Update();

	OutputImageType* outputImage = duplicator->GetModifiableOutput();

	filter->SetInput(reader2->GetOutput());
	filter->Update();

	DuplicatorType::Pointer duplicator2 = DuplicatorType::New();
	duplicator2->SetInputImage(filter->GetOutput());
	duplicator2->Update();

	OutputImageType* outputImage2 = duplicator2->GetModifiableOutput();

	typedef itk::SubtractImageFilter<
		InputImageType,
		InputImageType,
		InputImageType > DifferenceFilterType;

	DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

	difference->SetInput1(reader->GetOutput());
	difference->SetInput2(resampler->GetOutput());

	filter->SetInput(difference->GetOutput());

    OutputImageType* outputImage3 = filter->GetOutput();


	typedef itk::OtsuThresholdImageFilter< OutputImageType, OutputImageType >
		OtsuFilterType;
	OtsuFilterType::Pointer otsuFilter = OtsuFilterType::New();
	otsuFilter->SetInput(outputImage);
	otsuFilter->Update();

	typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType > RescaleFilterType;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(outputImage);
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);


    // NNC
    typedef   itk::CurvatureFlowImageFilter< OutputImageType, OutputImageType> CurvatureFlowImageFilterType;

    CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

    typedef itk::NeighborhoodConnectedImageFilter<OutputImageType, OutputImageType > ConnectedFilterType;


    ConnectedFilterType::Pointer neighborhoodConnected = ConnectedFilterType::New();
   
    typedef itk::CastImageFilter< OutputImageType, OutputImageType >
                                    CastingFilterType;
    CastingFilterType::Pointer caster = CastingFilterType::New();


    smoothing->SetInput( outputImage );
    neighborhoodConnected->SetInput( smoothing->GetOutput() );
    caster->SetInput( neighborhoodConnected->GetOutput() );
    neighborhoodConnected->Update();

    smoothing->SetNumberOfIterations( 5 );
    smoothing->SetTimeStep( 0.125 );

    neighborhoodConnected->SetLower(  0  );
    neighborhoodConnected->SetUpper(  10  );

    OutputImageType::SizeType radius;

    radius[0] = 10;   // two pixels along X
    radius[1] = 10;   // two pixels along Y

    neighborhoodConnected->SetRadius( radius );

    OutputImageType::IndexType  index;

    index[0] = 0;
    index[1] = 0;
    index[2] = 0;

    neighborhoodConnected->SetSeed( index );
    neighborhoodConnected->SetReplaceValue( 255 );



	QuickView viewer;
	viewer.AddImage(outputImage);
	viewer.AddImage(rescaleFilter->GetOutput());
	rescaleFilter->SetInput(outputImage3);
	viewer.AddImage(otsuFilter->GetOutput());
	viewer.AddImage(outputImage2);
	viewer.AddImage(rescaleFilter->GetOutput());
    viewer.AddImage(caster->GetOutput());
	viewer.Visualize();

	return 0;
}
