/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkElectronMapper.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkElectronMapper - Mapper that renders electronic data associated
// with a molecule
//
// .SECTION Description
// vtkElectronMapper provides common visualization techniques for electronic
// data.

#ifndef __vtkElectronMapper_h
#define __vtkElectronMapper_h

#include "vtkAbstractMapper3D.h"
#include "vtkNew.h" // For vtkNew
#include "vtkSmartPointer.h" // For vtkSmartPointer

class vtkContourFilter;
class vtkElectronActor;
class vtkImageData;
class vtkImageReslice;
class vtkMapper;
class vtkMolecule;
class vtkPiecewiseFunction;
class vtkPolyDataMapper;
class vtkPlane;
class vtkProp3D;
class vtkRenderer;
class vtkScalarsToColors;
class vtkSmartVolumeMapper;
class vtkTransformPolyDataFilter;

class VTK_CHEMISTRY_EXPORT vtkElectronMapper : public vtkAbstractMapper3D
{
public:
  static vtkElectronMapper *New();
  vtkTypeMacro(vtkElectronMapper,vtkAbstractMapper3D);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get/Set the input molecule
  vtkMolecule * GetInput();
  virtual void SetInput(vtkMolecule *);

  // Description:
  // Set the mapper to render the molecule's electron density
  virtual void UseElectronDensity();

  // Description:
  // Set the mapper to render the specified molecular orbital
  virtual void UseMolecularOrbital(vtkIdType orbitalNumber);

  // Description:
  // Set the mapper to render the highest occupied molecular orbital
  virtual void UseHOMO();

  // Description:
  // Set the mapper to render the lowest unoccupied molecular orbital
  virtual void UseLUMO();

  // Description:
  // Set/Get the value used to generate an isosurface.
  vtkSetMacro(IsosurfaceValue, double);
  vtkGetMacro(IsosurfaceValue, double);

  // Description:
  // Set/Get the plane used when rendering a ContourPlane.
  virtual void SetContourPlane(vtkPlane *);
  vtkGetObjectMacro(ContourPlane, vtkPlane);

  // Description:
  // Set/Get the number of contour lines used to create a ContourPlane.
  vtkSetMacro(NumberOfContours, int);
  vtkGetMacro(NumberOfContours, int);

  // Description:
  // Use a default contour range for the ContourPlane. Default: true
  //
  // If plotting density, [0, image max] will be used.
  //
  // If plotting a molecular orbital, [-abs image max, abs image max]
  // will be used, where "abs image max" is the highest absolute value in the
  // image data.
  vtkSetMacro(UseDefaultContourRange, bool);
  vtkGetMacro(UseDefaultContourRange, bool);
  vtkBooleanMacro(UseDefaultContourRange, bool);

  // Description:
  // Set/Get the range of values over which to generate isolines in the
  // ContourPlane.
  vtkSetVector2Macro(ContourRange, double);
  vtkGetVector2Macro(ContourRange, double);

  // Description:
  // Access the underlying mappers
  vtkGetVtkNewMacro(VolumeMapper, vtkSmartVolumeMapper);
  vtkGetVtkNewMacro(IsosurfaceMapper, vtkPolyDataMapper);
  vtkGetVtkNewMacro(ContourPlaneMapper, vtkPolyDataMapper);

  // Description:
  // Access the current vtkImageData.
  virtual vtkImageData * GetImageData();

  // Description:
  // Reimplemented from base class
  virtual void ReleaseGraphicsResources(vtkWindow *);
  double * GetBounds();
  virtual int FillInputPortInformation(int port, vtkInformation* info);

  friend class vtkElectronActor;
  
protected:
  vtkElectronMapper();
  ~vtkElectronMapper();

  enum ImageTypeType {
    Orbital = 0,
    Density
    } ImageType;

  // Description:
  // Get/Set the type of image that will be rendered. Used internally.
  // @sa SetToElectronDensity
  // @sa SetToMolecularOrbital
  // @sa SetToHOMO
  // @sa SetToLUMO
  vtkGetMacro(ImageType, ImageTypeType);
  vtkSetMacro(ImageType, ImageTypeType);

  vtkNew<vtkSmartVolumeMapper> VolumeMapper;

  double IsosurfaceValue;
  vtkNew<vtkContourFilter> IsosurfaceContourFilter;
  vtkNew<vtkPolyDataMapper> IsosurfaceMapper;

  vtkPlane *ContourPlane;
  int NumberOfContours;
  double ContourRange[2];
  bool UseDefaultContourRange;
  vtkNew<vtkImageReslice> ContourPlaneReslice;
  vtkNew<vtkContourFilter> ContourPlaneContourFilter;
  vtkNew<vtkTransformPolyDataFilter> ContourPlaneTransformFilter;
  vtkNew<vtkPolyDataMapper> ContourPlaneMapper;

  virtual void SetImageData(vtkImageData *image);
  vtkSmartPointer<vtkImageData> ImageData;

  bool VolumeMapperInitialized;
  bool IsosurfaceMapperInitialized;
  bool ContourPlaneMapperInitialized;

  // Description:
  // Update the mapper's internal pipeline using the specified ElectronActor
  virtual void UpdateInternal(vtkElectronActor *);
  virtual void UpdateVolumeMapper(vtkElectronActor *);
  virtual void UpdateIsosurfaceMapper(vtkElectronActor *);
  virtual void UpdateContourPlaneMapper(vtkElectronActor *);

private:
  vtkElectronMapper(const vtkElectronMapper&);  // Not implemented.
  void operator=(const vtkElectronMapper&);  // Not implemented.
};

#endif
