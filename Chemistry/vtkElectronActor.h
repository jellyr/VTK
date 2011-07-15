/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkElectronActor.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkElectronActor - Mapper that renders electronic data associated
// with a molecule
//
// .SECTION Description
// vtkElectronActor provides common visualization techniques for electronic
// data.

#ifndef __vtkElectronActor_h
#define __vtkElectronActor_h

#include "vtkProp3D.h"
#include "vtkNew.h" // For vtkNew

class vtkActor;
class vtkElectronMapper;
class vtkProperty;
class vtkVolume;
class vtkVolumeProperty;

#define __vtkElectronActor_PassToCurrentProp3D(_rettype,_func,_argtype) \
  virtual _rettype _func( _argtype _arg) \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    return prop->_func(_arg); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  return static_cast<_rettype>(NULL); \
  }

#define __vtkElectronActor_PassToCurrentProp3D_retVoid(_func,_argtype) \
  virtual void _func( _argtype _arg) \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    prop->_func(_arg); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  }

#define __vtkElectronActor_PassToCurrentProp3D_noArg(_rettype,_func) \
  virtual _rettype _func() \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    return prop->_func(); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  return static_cast<_rettype>(NULL); \
  }

#define __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(_func) \
  virtual void _func() \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    prop->_func(); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  }

#define __vtkElectronActor_PassToCurrentProp3D_2arg(_rettype,_func,_argtype1,_argtype2) \
  virtual _rettype _func( _argtype1 _arg1, _argtype2 _arg2) \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    return prop->_func(_arg1, _arg2); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  return static_cast<_rettype>(NULL); \
  }

#define __vtkElectronActor_PassToCurrentProp3D_retVoid_2arg(_func,_argtype1,_argtype2) \
  virtual void _func( _argtype1 _arg1, _argtype2 _arg2) \
  { \
  if (vtkProp3D *prop = this->GetCurrentProp3D()) \
    { \
    prop->_func(_arg1, _arg2); \
    } \
  vtkErrorMacro(<<"vtkElectronActor not initialized."); \
  }

class VTK_CHEMISTRY_EXPORT vtkElectronActor : public vtkProp3D
{
public:
  static vtkElectronActor *New();
  vtkTypeMacro(vtkElectronActor,vtkProp3D);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get/Set the input vtkElectronMapper.
  void SetMapper(vtkElectronMapper *);
  vtkGetObjectMacro(Mapper, vtkElectronMapper);

  enum RenderStyleType {
    Volume = 0,
    Isosurface,
    ContourPlane
    };

  // Description:
  // Get/Set the rendering style.
  //
  // If 'Volume', a vtkSmartVolumeMapper/vtkVolume is internally used to
  // render vtkImageData provided by the input molecule's
  // vtkAbstractElectronicData.
  //
  // If 'Isosurface', a vtkPolyData isosurface is cut from the vtkImageData
  // provided by the input molecule's vtkAbstractElectronicData and rendered
  // using vtkPolyDataMapper/vtkActor.
  //
  // If 'ContourPlane' a plane is sliced from the vtkImageData, contoured,
  // and rendered.
  vtkGetMacro(RenderStyle, RenderStyleType);
  vtkSetMacro(RenderStyle, RenderStyleType);
  const char * GetRenderStyleAsString()
    {
    switch(this->RenderStyle)
      {
      case Volume: return "Volume";
      case Isosurface: return "Isosurface";
      case ContourPlane: return "ContourPlane";
      default: return "";
      }
    }
  void SetRenderStyleToVolume() {this->SetRenderStyle(Volume);}
  void SetRenderStyleToIsosurface() {this->SetRenderStyle(Isosurface);}
  void SetRenderStyleToContourPlane() {this->SetRenderStyle(ContourPlane);}

  // Description:
  // Set/Get whether to use a default color map to color the data. Default:
  // true
  vtkSetMacro(UseDefaultColorMap, bool);
  vtkGetMacro(UseDefaultColorMap, bool);
  vtkBooleanMacro(UseDefaultColorMap, bool);

  // Description:
  // Set/Get whether to use a default opacity map when rendering volumetric
  // data. Default: true
  vtkSetMacro(UseDefaultOpacityMap, bool);
  vtkGetMacro(UseDefaultOpacityMap, bool);
  vtkBooleanMacro(UseDefaultOpacityMap, bool);

  // Description:
  // Access the underlying props that perform the rendering.
  vtkGetNewMacro(IsosurfaceProp, vtkActor);
  vtkGetNewMacro(VolumeProp, vtkVolume);
  vtkGetNewMacro(ContourPlaneProp, vtkActor);

  // Description:
  // Get/Set the property objects used by the internal props.
  virtual vtkProperty * GetIsosurfaceProperty();
  virtual vtkVolumeProperty * GetVolumeProperty();
  virtual vtkProperty * GetContourPlaneProperty();
  virtual void SetIsosurfaceProperty(vtkProperty*);
  virtual void SetVolumeProperty(vtkVolumeProperty*);
  virtual void SetContourPlaneProperty(vtkProperty*);

  // Description:
  // Reimplemented from vtkProp:
  virtual double * GetBounds();
  int RenderOpaqueGeometry(vtkViewport *);
  int RenderTranslucentPolygonalGeometry(vtkViewport *);
  int RenderVolumetricGeometry(vtkViewport *);
  int RenderOverlay(vtkViewport *);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(Pick);
  __vtkElectronActor_PassToCurrentProp3D_noArg(unsigned long, GetRedrawMTime);
  __vtkElectronActor_PassToCurrentProp3D(bool, HasKeys, vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int, HasTranslucentPolygonalGeometry);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(ReleaseGraphicsResources, vtkWindow*);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_2arg(BuildPaths, vtkAssemblyPaths*, vtkAssemblyPath*);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetVisibility, int);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int, GetVisibility);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(VisibilityOn);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(VisibilityOff);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetPickable, int);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int, GetPickable);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(PickableOn);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(PickableOff);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetDragable, int);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int, GetDragable);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(DragableOn);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(DragableOff);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetUseBounds, bool);
  __vtkElectronActor_PassToCurrentProp3D_noArg(bool, GetUseBounds);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(UseBoundsOn);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(UseBoundsOff);
  __vtkElectronActor_PassToCurrentProp3D_2arg(bool,RenderFilteredOpaqueGeometry,vtkViewport*,vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D_2arg(bool,RenderFilteredTranslucentPolygonalGeometry,vtkViewport*,vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D_2arg(bool,RenderFilteredVolumetricGeometry,vtkViewport*,vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D_2arg(bool,RenderFilteredOverlay,vtkViewport*,vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D(double,GetEstimatedRenderTime,vtkViewport*);
  __vtkElectronActor_PassToCurrentProp3D_noArg(double,GetEstimatedRenderTime);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetEstimatedRenderTime,double);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(RestoreEstimatedRenderTime);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_2arg(AddEstimatedRenderTime,double,vtkViewport*);
  __vtkElectronActor_PassToCurrentProp3D_noArg(double,GetAllocatedRenderTime);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetRenderTimeMultiplier,double);
  __vtkElectronActor_PassToCurrentProp3D_noArg(double,GetRenderTimeMultiplier);
  __vtkElectronActor_PassToCurrentProp3D_retVoid_noArg(InitPathTraversal);
  __vtkElectronActor_PassToCurrentProp3D_noArg(vtkAssemblyPath*,GetNextPath);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int,GetNumberOfPaths);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(PokeMatrix, vtkMatrix4x4*);
  __vtkElectronActor_PassToCurrentProp3D_noArg(vtkMatrix4x4*,GetMatrix);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(SetPropertyKeys,vtkInformation*);
  __vtkElectronActor_PassToCurrentProp3D_noArg(vtkInformation*,GetPropertyKeys);
  __vtkElectronActor_PassToCurrentProp3D_noArg(bool,GetSupportsSelection);
  __vtkElectronActor_PassToCurrentProp3D_noArg(int,GetNumberOfConsumers);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(AddConsumer,vtkObject*);
  __vtkElectronActor_PassToCurrentProp3D_retVoid(RemoveConsumer,vtkObject*);
  __vtkElectronActor_PassToCurrentProp3D(vtkObject*,GetConsumer,int);
  __vtkElectronActor_PassToCurrentProp3D(int,IsConsumer,vtkObject*);

  friend class vtkElectronMapper;

protected:
  vtkElectronActor();
  ~vtkElectronActor();

  bool UseDefaultColorMap;
  bool UseDefaultOpacityMap;

  RenderStyleType RenderStyle;

  vtkElectronMapper *Mapper;

  vtkNew<vtkActor>  IsosurfaceProp;
  vtkNew<vtkVolume> VolumeProp;
  vtkNew<vtkActor>  ContourPlaneProp;

  void UpdateIsosurfaceProp();
  void UpdateVolumeProp();
  void UpdateContourProp();

  vtkProp3D * GetCurrentProp3D();

private:
  vtkElectronActor(const vtkElectronActor&);  // Not implemented.
  void operator=(const vtkElectronActor&);  // Not implemented.
};

#endif
