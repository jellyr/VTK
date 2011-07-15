/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkElectronMapper.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkElectronMapper.h"

#include "vtkAbstractElectronicData.h"
#include "vtkActor.h"
#include "vtkColorTransferFunction.h"
#include "vtkElectronActor.h"
#include "vtkExecutive.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkImageReslice.h"
#include "vtkImageShiftScale.h"
#include "vtkInformation.h"
#include "vtkMath.h"
#include "vtkMatrix4x4.h"
#include "vtkMolecule.h"
#include "vtkObjectFactory.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPlane.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkScalarsToColors.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro( vtkElectronMapper );
vtkCxxSetObjectMacro(vtkElectronMapper, ContourPlane, vtkPlane);

//----------------------------------------------------------------------------
vtkElectronMapper::vtkElectronMapper()
  : ImageType(Density),
    IsosurfaceValue(0.1),
    ContourPlane(NULL),
    NumberOfContours(50),
    UseDefaultContourRange(true),
    ImageData(vtkSmartPointer<vtkImageData>::New()),
    VolumeMapperInitialized(false),
    IsosurfaceMapperInitialized(false),
    ContourPlaneMapperInitialized(false)
{
  // Initialize arrays
  ContourRange[0] = -.25;
  ContourRange[1] =  .25;

  // Setup the internal pipelines:
  // Volume:
  this->VolumeMapper->SetBlendModeToComposite();

  // Isosurface:
  this->IsosurfaceMapper->SetInput(
        this->IsosurfaceContourFilter->GetOutput());

  // ContourPlane:
  this->ContourPlaneMapper->SetInput(
        this->ContourPlaneTransformFilter->GetOutput());
  this->ContourPlaneTransformFilter->SetInput(
        this->ContourPlaneContourFilter->GetOutput());
  this->ContourPlaneContourFilter->SetInput(
        this->ContourPlaneReslice->GetOutput());
}

//----------------------------------------------------------------------------
vtkElectronMapper::~vtkElectronMapper()
{
  this->SetContourPlane(NULL);
}

//----------------------------------------------------------------------------
void vtkElectronMapper::SetInput(vtkMolecule *input)
{
  if(input)
    {
    this->SetInputConnection(0, input->GetProducerPort());
    }
  else
    {
    // Setting a NULL input removes the connection.
    this->SetInputConnection(0, 0);
    }
}

//----------------------------------------------------------------------------
vtkMolecule * vtkElectronMapper::GetInput()
{
  return vtkMolecule::SafeDownCast(this->GetExecutive()->GetInputData(0, 0));
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UseElectronDensity()
{
  if (!this->GetInput() || !this->GetInput()->GetElectronicData())
    {
    vtkWarningMacro(<<"No input set, or no electronic data associated "
                    " with input molecule.");
    return;
    }

  this->SetImageType(Density);
  this->SetImageData(
        this->GetInput()->GetElectronicData()->GetElectronDensity());
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UseMolecularOrbital(vtkIdType mo)
{
  if (!this->GetInput() || !this->GetInput()->GetElectronicData())
    {
    vtkWarningMacro(<<"No input set, or no electronic data associated "
                    "with input molecule. this->GetInput() = "
                    << this->GetInput());
    return;
    }

  this->SetImageType(Orbital);
  this->SetImageData(this->GetInput()->GetElectronicData()->GetMO(mo));
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UseHOMO()
{
  if (!this->GetInput() || !this->GetInput()->GetElectronicData())
    {
    vtkWarningMacro(<<"No input set, or no electronic data associated "
                    " with input molecule.");
    return;
    }

  this->SetImageType(Orbital);
  this->SetImageData(this->GetInput()->GetElectronicData()->GetHOMO());
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UseLUMO()
{
  if (!this->GetInput() || !this->GetInput()->GetElectronicData())
    {
    vtkWarningMacro(<<"No input set, or no electronic data associated "
                    " with input molecule.");
    return;
    }

  this->SetImageType(Orbital);
  this->SetImageData(this->GetInput()->GetElectronicData()->GetLUMO());
}

void vtkElectronMapper::SetImageData(vtkImageData *image)
{
  // this->ImageData is a smart pointer, registration handled automatically.
  this->ImageData = image;

  // Update internal pipelines
  this->VolumeMapper->SetInput(this->ImageData);
  this->IsosurfaceContourFilter->SetInput(this->ImageData);
  this->ContourPlaneReslice->SetInput(this->ImageData);
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UpdateInternal(vtkElectronActor *act)
{
  vtkDebugMacro(<<"UpdateInternal called.");
  switch (act->RenderStyle)
    {
    case vtkElectronActor::Volume:
      this->UpdateVolumeMapper(act);
      break;
    default:
    case vtkElectronActor::Isosurface:
      this->UpdateIsosurfaceMapper(act);
      break;
    case vtkElectronActor::ContourPlane:
      this->UpdateContourPlaneMapper(act);
      break;
  }
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UpdateVolumeMapper(vtkElectronActor *act)
{
  vtkDebugMacro(<<"UpdateVolumeMapper called.");
  // We need the electron actor and its volume actor in order to set up
  // color mapping
  if (!act || !act->VolumeProp.GetPointer())
    {
    vtkWarningMacro(<<"vtkElectronMapper must be connected to a "
                    "vtkElectronActor before attempting to render an "
                    "isosurface. Aborting render.");
    return;
    }

  vtkMolecule *molecule = this->GetInput();

  // If the volume mapper is up-to-date, just return
  if ( this->GetMTime() < this->VolumeMapper->GetMTime() &&
       this->VolumeMapperInitialized)
    {
    return;
    }

  vtkDebugMacro(<< "Volume mapper needs update.");

  if (!this->ImageData)
    {
    vtkDebugMacro(<<"No image data set. Aborting render.");
    return;
    }

  // Setup the color, etc based on the image type
  switch (this->ImageType)
    {
    case Orbital:
      {
      // Scaling factors for mapping
      double range[2];
      this->ImageData->Update();
      this->ImageData->GetScalarRange(range);
      double maxAbsVal = (fabs(range[0]) > fabs(range[1])) ? fabs(range[0])
                                                           : fabs(range[1]);

      // Setup opacity ramp
      if (act->UseDefaultOpacityMap)
        {
        vtkDebugMacro(<<"Generating default opacity map.");
        vtkNew<vtkPiecewiseFunction> opacity;
        opacity->AddPoint(-1.0 * maxAbsVal, 0.95);
        opacity->AddPoint(-0.6 * maxAbsVal, 0.90);
        opacity->AddPoint(-0.1 * maxAbsVal, 0.05);
        opacity->AddPoint( 0.0 * maxAbsVal, 0.00);
        opacity->AddPoint( 0.1 * maxAbsVal, 0.05);
        opacity->AddPoint( 0.6 * maxAbsVal, 0.90);
        opacity->AddPoint( 1.0 * maxAbsVal, 0.95);
        act->GetVolumeProperty()->SetScalarOpacity(opacity.GetPointer());
        }

      // Setup color ramp
      if (act->UseDefaultColorMap)
        {
        vtkDebugMacro(<<"Generating default color map.");
        vtkNew<vtkColorTransferFunction> color;
        color->AddHSVPoint(-1.00 * maxAbsVal, 0.00, 0.85, 1.00);
        color->AddHSVPoint(-0.80 * maxAbsVal, 0.00, 0.85, 0.80);
        color->AddHSVPoint(-0.01 * maxAbsVal, 0.00, 0.85, 0.25);
        color->AddHSVPoint( 0.01 * maxAbsVal, 0.66, 0.85, 0.25);
        color->AddHSVPoint( 0.80 * maxAbsVal, 0.66, 0.85, 0.80);
        color->AddHSVPoint( 1.00 * maxAbsVal, 0.66, 0.85, 1.00);
        act->GetVolumeProperty()->SetColor(color.GetPointer());
        }

      break;
      }
    case Density:
      {
      // Scaling factors for mapping
      double range[2];
      this->ImageData->Update();
      this->ImageData->GetScalarRange(range);
      double magnitude = range[1];

      // Setup opacity ramp
      if (act->UseDefaultOpacityMap)
        {
        vtkDebugMacro(<<"Generating default opacity map.");
        vtkNew<vtkPiecewiseFunction> op;
        op->AddPoint(  0.0 * magnitude, 0.05);
        op->AddPoint( 1e-3 * magnitude, 0.10);
        op->AddPoint( 1e-2 * magnitude, 0.20);
        op->AddPoint( 5e-2 * magnitude, 0.40);
        op->AddPoint(  0.2 * magnitude, 0.80);
        op->AddPoint(  1.0 * magnitude, 0.95);
        act->VolumeProp->GetProperty()->SetScalarOpacity(op.GetPointer());
        }

      // Setup color ramp
      if (act->UseDefaultColorMap)
        {
        vtkNew<vtkColorTransferFunction> color;
        vtkDebugMacro(<<"Generating default color map.");
        color->AddHSVPoint( 1e-2 * magnitude, 0.66, 0.5, 0.25);
        color->AddHSVPoint( 5e-2 * magnitude, 0.66, 0.5, 0.40);
        color->AddHSVPoint(  0.2 * magnitude, 0.66, 0.5, 0.80);
        color->AddHSVPoint(  1.0 * magnitude, 0.66, 0.5, 1.00);
        act->VolumeProp->GetProperty()->SetColor(color.GetPointer());
        }

      break;
      }
      // Shouldn't be reached
    default:
      vtkErrorMacro(<<"Unrecognized ImageType: " << this->ImageType);
      break;

    }

  // Setup clipping planes
  this->VolumeMapper->SetClippingPlanes(this->ClippingPlanes);

  this->VolumeMapperInitialized = true;
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UpdateIsosurfaceMapper(vtkElectronActor *act)
{
  vtkDebugMacro(<<"UpdateIsosurfaceMapper called.");
  // We need the electron actor and its isosurface actor in order to set up
  // color mapping
  if (!act || !act->IsosurfaceProp.GetPointer())
    {
    vtkWarningMacro(<<"vtkElectronMapper must be connected to a "
                    "vtkElectronActor before attempting to render an "
                    "isosurface. Aborting render.");
    return;
    }

  vtkMolecule *molecule = this->GetInput();

  // If the volume mapper is up-to-date, just return
  if ( this->GetMTime() < this->IsosurfaceMapper->GetMTime() &&
       this->IsosurfaceMapperInitialized)
    {
    return;
    }

  vtkDebugMacro(<< "Isosurface mapper needs update.");

  if (!this->ImageData)
    {
    vtkDebugMacro(<<"No image data set. Aborting render.");
    this->IsosurfaceMapper->SetInputConnection(NULL);
    return;
    }

  // Setup the color, etc based on the image type
  switch (this->ImageType)
    {
    case Orbital:
      {
      // Cut the isosurface(s)
      this->IsosurfaceContourFilter->GenerateValues(2, -this->IsosurfaceValue,
                                                    this->IsosurfaceValue);
      vtkDebugMacro(<<"Slicing image at +/-" << this->IsosurfaceValue);

      // Set up colors
      if (act->UseDefaultColorMap)
        {
        vtkNew<vtkColorTransferFunction> color;
        vtkDebugMacro(<<"Generating default color map.");
        color->AddRGBSegment(-1.0, 1.0, 0.0, 0.0, -1e-5, 1.0, 0.0, 0.0);
        color->AddRGBSegment(1e-5, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0);
        this->IsosurfaceMapper->SetLookupTable(color.GetPointer());
        }

      this->IsosurfaceMapper->SetScalarRange(-this->IsosurfaceValue,
                                             this->IsosurfaceValue);
      break;
      }
    case Density:
      {
      // Cut the isosurface
      this->IsosurfaceContourFilter->SetValue(0, this->IsosurfaceValue);
      vtkDebugMacro(<<"Cutting isosurface at " << this->IsosurfaceValue);

      // Set up colors
      if (act->UseDefaultColorMap)
        {
        vtkDebugMacro(<<"Generating default color map.");
        vtkNew<vtkColorTransferFunction> color;
        color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
        this->IsosurfaceMapper->SetLookupTable(color.GetPointer());
        }

      this->IsosurfaceMapper->SetScalarRange(this->IsosurfaceValue,
                                             this->IsosurfaceValue);

      break;
      }
      // Shouldn't be reached
    default:
      vtkErrorMacro(<<"Unrecognized ImageType: " << this->ImageType);
      break;
    }

  // Setup clipping planes
  this->IsosurfaceMapper->SetClippingPlanes(this->ClippingPlanes);

  this->IsosurfaceMapperInitialized = true;
}

//----------------------------------------------------------------------------
void vtkElectronMapper::UpdateContourPlaneMapper(vtkElectronActor *act)
{
  vtkDebugMacro(<<"UpdateContourPlaneMapper called.");
  // We need the electron actor and its isosurface actor in order to set up
  // color mapping
  if (!act || !act->ContourPlaneProp.GetPointer())
    {
    vtkWarningMacro(<<"vtkElectronMapper must be connected to a "
                    "vtkElectronActor before attempting to render an "
                    "contour plane. Aborting render.");
    return;
    }

  // If the contour plane mapper is up-to-date, just return
  if ( this->GetMTime() < this->ContourPlaneMapper->GetMTime() &&
       this->ContourPlaneMapperInitialized)
    {
    return;
    }

  vtkDebugMacro(<< "Contour plane mapper needs update.");

  if (!this->ImageData)
    {
    vtkDebugMacro(<<"No image data set. Aborting render.");
    this->ContourPlaneMapper->SetInputConnection(NULL);
    return;
    }

  if (!this->ContourPlane)
    {
    vtkDebugMacro(<<"No contour plane set. Aborting render.");
    this->ContourPlaneMapper->SetInputConnection(NULL);
    return;
    }

  // Set the default contour range if needed
  if (this->UseDefaultContourRange)
    {
    // Find the highest absolute contour value
    double imageRange[2];
    this->ImageData->GetScalarRange(imageRange);
    double absMaxValue = (fabs(imageRange[0]) > fabs(imageRange[1])) ?
          fabs(imageRange[0]) : fabs(imageRange[1]);

    // Set ContourRange
    switch (this->ImageType)
      {
      case Orbital:
        this->ContourRange[0] = -absMaxValue;
        this->ContourRange[1] =  absMaxValue;
        break;
      case Density:
        this->ContourRange[0] = 0.0;
        this->ContourRange[1] = imageRange[1];
        break;
        // Shouldn't be reached
      default:
        vtkErrorMacro(<<"Unrecognized ImageType: " << this->ImageType);
        break;
      }
    vtkDebugMacro(<<"Image range is " << imageRange[0]
                  << " " << imageRange[1] << ".");
    vtkDebugMacro(<<"Setting contour range to " << this->ContourRange[0]
                  << " " << this->ContourRange[1] << ".");
    }

  // Slice the image
  vtkVector3d origV1 (1.0, 0.0, 0.0);
  vtkVector3d origV2 (0.0, 1.0, 0.0);
  vtkVector3d origV3 (0.0, 0.0, 1.0);
  vtkVector3d normal (this->ContourPlane->GetNormal());
  vtkVector3d pointInPlane (this->ContourPlane->GetOrigin());
  // Normalize vectors:
  origV1.Normalize();
  origV2.Normalize();
  origV3.Normalize();
  normal.Normalize();
  // Calculate new basis of image slice, v3 is the normal.
  vtkVector3d dims[3];
  vtkVector3d *nextVector = &(dims[0]);
  if (!normal.Compare(origV1, 0.1))
    {
    *nextVector = normal.Cross(origV1);
    ++nextVector;
    }
  if (!normal.Compare(origV2, 0.1))
    {
    *nextVector = normal.Cross(origV2);
    ++nextVector;
    }
  if (!normal.Compare(origV3, 0.1) && nextVector != &(dims[2]))
    {
    *nextVector = normal.Cross(origV3);
    ++nextVector;
    }
  *nextVector = normal;
  assert ("Reslice basis properly set" && nextVector == &(dims[2]));

  vtkNew<vtkMatrix4x4> mat;
  mat->SetElement(0,0, dims[0].X());
  mat->SetElement(1,0, dims[0].Y());
  mat->SetElement(2,0, dims[0].Z());
  mat->SetElement(3,0, 0.0);
  mat->SetElement(0,1, dims[1].X());
  mat->SetElement(1,1, dims[1].Y());
  mat->SetElement(2,1, dims[1].Z());
  mat->SetElement(3,1, 0.0);
  mat->SetElement(0,2, dims[2].X());
  mat->SetElement(1,2, dims[2].Y());
  mat->SetElement(2,2, dims[2].Z());
  mat->SetElement(3,2, 0.0);
  mat->SetElement(0,3, pointInPlane.X());
  mat->SetElement(1,3, pointInPlane.Y());
  mat->SetElement(2,3, pointInPlane.Z());
  mat->SetElement(3,3, 1.0);

  this->ContourPlaneReslice->SetResliceAxes(mat.GetPointer());
  this->ContourPlaneReslice->SetOutputDimensionality(2);
  this->ContourPlaneReslice->SetOutputSpacing(this->ImageData->GetSpacing());
  this->ContourPlaneReslice->AutoCropOutputOn();
  this->ContourPlaneReslice->SetInterpolationModeToCubic();

  // Find the contours
  this->ContourPlaneContourFilter->GenerateValues(this->NumberOfContours,
                                                  this->ContourRange);

  // Transform polydata back from slice basis
  vtkNew<vtkTransform> xform;
  xform->SetMatrix(mat.GetPointer());
  this->ContourPlaneTransformFilter->SetTransform(xform.GetPointer());

  // Find the highest absolute contour value
  double maxContour =
      (fabs(this->ContourRange[0]) > fabs(this->ContourRange[1])) ?
        fabs(this->ContourRange[0]) : fabs(this->ContourRange[1]);

  // Setup the color, etc based on the image type
  switch (this->ImageType)
    {
    case Orbital:
      {
      // Set up colors
      if (act->UseDefaultColorMap)
        {
        vtkDebugMacro(<<"Generating default color map.");
        vtkNew<vtkColorTransferFunction> color;
        color->AddHSVPoint( -maxContour, 0.00, 1.00, 1.0);
        color->AddHSVPoint(       -1e-9, 0.00, 1.00, 1.0);
        color->AddHSVPoint(        1e-9, 0.66, 1.00, 1.0);
        color->AddHSVPoint(  maxContour, 0.66, 1.00, 1.0);
        this->ContourPlaneMapper->SetLookupTable(color.GetPointer());
        this->ContourPlaneMapper->SetScalarRange(this->ContourRange);
        }

      break;
      }
    case Density:
      {
      // Set up colors
      if (act->UseDefaultColorMap)
        {
        vtkDebugMacro(<<"Generating default color map.");
        double min = this->ContourRange[0];
        double max = this->ContourRange[1];
        double spread = max - min;
        vtkNew<vtkColorTransferFunction> color;
        color->AddHSVPoint( min + 1e-02 * spread, 0.66, 1.0, 1.0);
        color->AddHSVPoint( min + 5e-02 * spread, 0.33, 1.0, 1.0);
        color->AddHSVPoint( min + 1e-01 * spread, 0.17, 1.0, 1.0);
        color->AddHSVPoint( min +   1.0 * spread, 0.00, 1.0, 1.0);

        this->ContourPlaneMapper->SetLookupTable(color.GetPointer());
        this->ContourPlaneMapper->SetScalarRange(this->ContourRange);
        }

      break;
      }
      // Shouldn't be reached
    default:
      vtkErrorMacro(<<"Unrecognized ImageType: " << this->ImageType);
      break;
    }

  // Setup clipping planes
  this->ContourPlaneMapper->SetClippingPlanes(this->ClippingPlanes);

  this->ContourPlaneMapperInitialized = true;
}

//----------------------------------------------------------------------------
vtkImageData * vtkElectronMapper::GetImageData()
{
  return this->ImageData.GetPointer();
}

//----------------------------------------------------------------------------
void vtkElectronMapper::ReleaseGraphicsResources(vtkWindow *w)
{
  this->VolumeMapper->ReleaseGraphicsResources(w);
  this->IsosurfaceMapper->ReleaseGraphicsResources(w);
  this->ContourPlaneMapper->ReleaseGraphicsResources(w);
}

//----------------------------------------------------------------------------
double * vtkElectronMapper::GetBounds()
{
  if (!this->ImageData.GetPointer())
    {
    vtkMath::UninitializeBounds(this->Bounds);
    }
  else
    {
    this->ImageData->GetBounds(this->Bounds);
    }
  return this->Bounds;
}

//----------------------------------------------------------------------------
int vtkElectronMapper::FillInputPortInformation(int vtkNotUsed(port),
                                                vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMolecule");
  return 1;
}

//----------------------------------------------------------------------------
void vtkElectronMapper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "ImageType: ";
  switch (this->ImageType)
    {
    case Orbital:
      os << "Orbital\n";
      break;
    case Density:
      os << "Density\n";
      break;
    default:
      os << "Invalid\n";
      break;
    }

  os << indent << "VolumeMapper: @" << this->VolumeMapper << ":\n";
  this->VolumeMapper->PrintSelf(os, indent.GetNextIndent());

  os << indent << "IsosurfaceValue: " << this->IsosurfaceValue << "\n";

  os << indent << "IsosurfaceContourFilter: @"
     << this->IsosurfaceContourFilter << ":\n";
  this->IsosurfaceContourFilter->PrintSelf(os, indent.GetNextIndent());
  os << indent << "IsosurfaceMapper: @" << this->IsosurfaceMapper << ":\n";
  this->IsosurfaceMapper->PrintSelf(os, indent.GetNextIndent());
  os << indent << "ImageData: @" << this->ImageData << ":\n";
  this->ImageData->PrintSelf(os, indent.GetNextIndent());

  os << indent << "ContourPlaneReslice: @" <<this->ContourPlaneReslice<<":\n";
  this->ContourPlaneReslice->PrintSelf(os, indent.GetNextIndent());
  os << indent << "ContourPlaneContourFilter: @"
     << this->ContourPlaneContourFilter << ":\n";
  this->ContourPlaneContourFilter->PrintSelf(os, indent.GetNextIndent());
  os << indent << "ContourPlaneMapper: @"<< this->ContourPlaneMapper << ":\n";
  this->ContourPlaneMapper->PrintSelf(os, indent.GetNextIndent());

  os << indent << "VolumeMapperInitialized: "
     << this->VolumeMapperInitialized << "\n";
  os << indent << "IsosurfaceMapperInitialized: "
     << this->IsosurfaceMapperInitialized << "\n";
  os << indent << "ContourPlaneMapperInitialized: "
     << this->ContourPlaneMapperInitialized << "\n";
}
