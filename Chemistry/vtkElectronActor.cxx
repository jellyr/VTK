/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkElectronActor.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkElectronActor.h"

#include "vtkActor.h"
#include "vtkElectronMapper.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkViewport.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro( vtkElectronActor);

//----------------------------------------------------------------------------
vtkElectronActor::vtkElectronActor()
  : UseDefaultColorMap(true),
    UseDefaultOpacityMap(true),
    RenderStyle(Isosurface),
    Mapper(NULL)
{
  this->VolumeProp->GetProperty()->ShadeOff();
  this->VolumeProp->GetProperty()->SetInterpolationTypeToLinear();
}

//----------------------------------------------------------------------------
vtkElectronActor::~vtkElectronActor()
{
  this->Mapper->Delete();
  this->Mapper =0;
}

//----------------------------------------------------------------------------
void vtkElectronActor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "RenderStyle: " << this->GetRenderStyleAsString();
  os << indent << "UseDefaultColorMap" << this->GetUseDefaultColorMap();
  os << indent << "UseDefaultOpacityMap" << this->GetUseDefaultOpacityMap();

}

//----------------------------------------------------------------------------
void vtkElectronActor::SetMapper(vtkElectronMapper *map)
{
  vtkSetObjectBodyMacro(Mapper, vtkElectronMapper, map);
  if (map)
    {
    this->IsosurfaceProp->SetMapper(map->IsosurfaceMapper.GetPointer());
    this->VolumeProp->SetMapper(map->VolumeMapper.GetPointer());
    this->ContourPlaneProp->SetMapper(map->ContourPlaneMapper.GetPointer());
    }
  else
    {
    this->IsosurfaceProp->SetMapper(NULL);
    this->VolumeProp->SetMapper(NULL);
    this->ContourPlaneProp->SetMapper(NULL);
    }
}

//----------------------------------------------------------------------------
vtkProperty * vtkElectronActor::GetIsosurfaceProperty()
{
  return this->IsosurfaceProp->GetProperty();
}

//----------------------------------------------------------------------------
vtkVolumeProperty * vtkElectronActor::GetVolumeProperty()
{
  return this->VolumeProp->GetProperty();
}

//----------------------------------------------------------------------------
vtkProperty * vtkElectronActor::GetContourPlaneProperty()
{
  return this->ContourPlaneProp->GetProperty();
}

//----------------------------------------------------------------------------
void vtkElectronActor::SetIsosurfaceProperty(vtkProperty *p)
{
  this->IsosurfaceProp->SetProperty(p);
}

//----------------------------------------------------------------------------
void vtkElectronActor::SetVolumeProperty(vtkVolumeProperty *p)
{
  this->VolumeProp->SetProperty(p);
}

//----------------------------------------------------------------------------
void vtkElectronActor::SetContourPlaneProperty(vtkProperty *p)
{
  this->ContourPlaneProp->SetProperty(p);
}

//----------------------------------------------------------------------------
double * vtkElectronActor::GetBounds()
{
  if (this->Mapper)
    {
    double *mbounds = this->Mapper->GetBounds();
    for (int i = 0; i < 6; ++i)
      this->Bounds[i] = mbounds[i];
    }
  else
    {
    vtkMath::UninitializeBounds(this->Bounds);
    }
  return this->Bounds;
}

//----------------------------------------------------------------------------
int vtkElectronActor::RenderOpaqueGeometry(vtkViewport *vp)
{
  if (!this->Mapper)
    {
    vtkDebugMacro(<<"No ElectronMapper set!");
    return 0;
    }

  vtkProp3D *currentProp = this->GetCurrentProp3D();
  if (currentProp == NULL)
    {
    vtkDebugMacro(<<"CurrentProp3D is NULL!");
    return 0;
    }

  this->Mapper->UpdateInternal(this);
  currentProp->RenderOpaqueGeometry(vp);
  return 1;
}

//----------------------------------------------------------------------------
int vtkElectronActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
  if (!this->Mapper)
    {
    vtkDebugMacro(<<"No ElectronMapper set!");
    return 0;
    }

  vtkProp3D *currentProp = this->GetCurrentProp3D();
  if (currentProp == NULL)
    {
    vtkDebugMacro(<<"CurrentProp3D is NULL!");
    return 0;
    }

  this->Mapper->UpdateInternal(this);
  currentProp->RenderTranslucentPolygonalGeometry(vp);
  return 1;
}

//----------------------------------------------------------------------------
int vtkElectronActor::RenderVolumetricGeometry(vtkViewport *vp)
{
  if (!this->Mapper)
    {
    vtkDebugMacro(<<"No ElectronMapper set!");
    return 0;
    }

  vtkProp3D *currentProp = this->GetCurrentProp3D();
  if (currentProp == NULL)
    {
    vtkDebugMacro(<<"CurrentProp3D is NULL!");
    return 0;
    }

  this->Mapper->UpdateInternal(this);
  currentProp->RenderVolumetricGeometry(vp);
  return 1;
}

//----------------------------------------------------------------------------
int vtkElectronActor::RenderOverlay(vtkViewport *vp)
{
  if (!this->Mapper)
    {
    vtkDebugMacro(<<"No ElectronMapper set!");
    return 0;
    }

  vtkProp3D *currentProp = this->GetCurrentProp3D();
  if (currentProp == NULL)
    {
    vtkDebugMacro(<<"CurrentProp3D is NULL!");
    return 0;
    }

  this->Mapper->UpdateInternal(this);
  currentProp->RenderOverlay(vp);
  return 1;
}

//----------------------------------------------------------------------------
vtkProp3D * vtkElectronActor::GetCurrentProp3D()
{
  switch (this->RenderStyle)
  {
  case Isosurface:
    return this->IsosurfaceProp.GetPointer();
  case Volume:
    return this->VolumeProp.GetPointer();
  case ContourPlane:
    return this->ContourPlaneProp.GetPointer();
  }
  vtkDebugMacro(<<"Current Prop3D is NULL!")
  return NULL;
}
