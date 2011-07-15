/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkTestUtilities.h"
#include "vtkRegressionTestImage.h"

#include "vtkAbstractElectronicData.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkElectronActor.h"
#include "vtkElectronMapper.h"
#include "vtkImageData.h"
#include "vtkMolecule.h"
#include "vtkMoleculeMapper.h"
#include "vtkNew.h"
#include "vtkOpenQubeMoleculeSource.h"
#include "vtkOpenQubeElectronicData.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPolyDataMapper.h"
#include "vtkPlane.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSimpleBondPerceiver.h"
#include "vtkSmartPointer.h"
#include "vtkVolumeProperty.h"
#include "vtkSmartVolumeMapper.h"

#include <openqube/basissetloader.h>
#include <openqube/basisset.h>

int TestElectronMapperOrbitalCustomColors(int argc, char *argv[])
{
  char* fname = vtkTestUtilities::ExpandDataFileName(
    argc, argv, "Data/2h2o.out");

  vtkNew<vtkOpenQubeMoleculeSource> oq;
  oq->SetFileName(fname);
  oq->Update();

  vtkSmartPointer<vtkMolecule> mol = vtkSmartPointer<vtkMolecule>::New();
  mol = oq->GetOutput();
  // Increase resolution of grid, decrease extent
  vtkOpenQubeElectronicData::SafeDownCast(
        mol->GetElectronicData())->SetSpacing(0.05);
  vtkOpenQubeElectronicData::SafeDownCast(
        mol->GetElectronicData())->SetPadding(0.75);

  //--------------------------------------------------------------------------
  // If there aren't any bonds, attempt to perceive them
  if (mol->GetNumberOfBonds() == 0)
    {
    cout << "No bonds found. Running simple bond perception...\n";
    vtkNew<vtkSimpleBondPerceiver> bonder;
    bonder->SetInput(mol);
    bonder->Update();
    mol = bonder->GetOutput();
    cout << "Bonds found: " << mol->GetNumberOfBonds() << "\n";
    }

  //--------------------------------------------------------------------------
  // Find clipping plane
  vtkAtom O1 = mol->GetAtom(0);
  vtkAtom H1 = mol->GetAtom(3);
  vtkAtom H2 = mol->GetAtom(2);
  vtkVector3d O1pos = O1.GetPositionAsVector3d();
  vtkVector3d H1pos = H1.GetPositionAsVector3d();
  vtkVector3d H2pos = H2.GetPositionAsVector3d();
  vtkNew<vtkPlane> plane;
  // TODO vtkVector subtraction
//  vtkMolecule::GetPlaneFromBond(O1, H1, (O1pos-H1pos).Cross(O1pos-H2pos),
//                                plane.GetPointer());
  vtkVector3d O1_H1;
  vtkVector3d O1_H2;
  O1_H1[0] = O1pos[0] - H1pos[0];
  O1_H1[1] = O1pos[1] - H1pos[1];
  O1_H1[2] = O1pos[2] - H1pos[2];
  O1_H2[0] = O1pos[0] - H2pos[0];
  O1_H2[1] = O1pos[1] - H2pos[1];
  O1_H2[2] = O1pos[2] - H2pos[2];
  vtkMolecule::GetPlaneFromBond(O1, H1, O1_H1.Cross(O1_H2),
                                plane.GetPointer());
  // end vtkVector TODO

  //--------------------------------------------------------------------------
  // Reduce resolution of image for speed
  vtkOpenQubeElectronicData *oqed = vtkOpenQubeElectronicData::SafeDownCast(
        mol->GetElectronicData());
  oqed->SetSpacing(0.1);
  // Increase padding so contours aren't clipped
  oqed->SetPadding(2.0);

  //--------------------------------------------------------------------------
  // Molecular geometry
  vtkNew<vtkMoleculeMapper> molMapper;
  molMapper->SetInput(mol);
  molMapper->UseLiqouriceStickSettings();
  molMapper->SetBondRadius(0.1);
  molMapper->SetAtomicRadiusScaleFactor(0.1);

  vtkNew<vtkActor> molActor;
  molActor->SetMapper(molMapper.GetPointer());

  //--------------------------------------------------------------------------
  // Isosurface
  vtkNew<vtkElectronMapper> eleIsoMapper;
  eleIsoMapper->SetInput(mol);
  eleIsoMapper->UseMolecularOrbital(3);
  eleIsoMapper->SetIsosurfaceValue(0.1);

  vtkNew<vtkElectronActor> eleIsoActor;
  eleIsoActor->SetMapper(eleIsoMapper.GetPointer());
  eleIsoActor->SetRenderStyleToIsosurface();
  eleIsoActor->GetIsosurfaceProperty()->SetOpacity(0.85);

  // Add clipping plane
  eleIsoMapper->AddClippingPlane(plane.GetPointer());

  // Find range of imagedata
  double range[2];
  eleIsoMapper->Update();
  eleIsoMapper->GetImageData()->GetScalarRange(range);
  double min = range[0];
  double max = range[1];
  double spread = max - min;

  // Set custom color mapping -- pos yellow, neg green
  vtkNew<vtkColorTransferFunction> isoColor;
  isoColor->AddRGBPoint( 1e-2, 0.8, 0.8, 0.1);
  isoColor->AddRGBPoint(-1e-2, 0.1, 0.8, 0.2);
  // Disable default color mapping in actor, and set transfer function on
  // isosurface mapper.
  eleIsoActor->UseDefaultColorMapOff();
  eleIsoMapper->GetIsosurfaceMapper()->SetLookupTable(isoColor.GetPointer());

  //--------------------------------------------------------------------------
  // Volume
  vtkNew<vtkElectronMapper> eleVolMapper;
  eleVolMapper->SetInput(mol);
  eleVolMapper->UseMolecularOrbital(3);

  vtkNew<vtkPlane> mirroredPlane;
  mirroredPlane->SetOrigin(plane->GetOrigin());
  vtkVector3d mirroredNormal (plane->GetNormal());
  // TODO vtkVector negation
//  mirroredNormal = -mirroredNormal;
  mirroredNormal[0] = -mirroredNormal[0];
  mirroredNormal[1] = -mirroredNormal[1];
  mirroredNormal[2] = -mirroredNormal[2];
  // end vtkVector TODO
  mirroredPlane->SetNormal(mirroredNormal.GetData());
  eleVolMapper->AddClippingPlane(mirroredPlane.GetPointer());

  vtkNew<vtkElectronActor> eleVolActor;
  eleVolActor->SetMapper(eleVolMapper.GetPointer());
  eleVolActor->SetRenderStyleToVolume();

  // Set custom color mapping -- Gradient from white to black
  vtkNew<vtkColorTransferFunction> volColor;
  volColor->AddRGBSegment(0.8*min, 1.0, 1.0, 1.0, -1e-2, 0.0, 0.0, 0.0);
  volColor->AddRGBSegment(1e-2, 0.0, 0.0, 0.0, 0.8*max, 1.0, 1.0, 1.0);
  // Disable default color mapping in actor, and set transfer function on
  // volume property.
  eleVolActor->UseDefaultColorMapOff();
  eleVolActor->GetVolumeProperty()->SetColor(volColor.GetPointer());

  // Use custom opacity ramp, too
  vtkNew<vtkPiecewiseFunction> volOpacity;
  volOpacity->AddPoint(0.8 * min, 0.90);
  volOpacity->AddPoint(0.5 * min, 0.75);
  volOpacity->AddPoint(0.0, 0.0);
  volOpacity->AddPoint(0.5 * max, 0.75);
  volOpacity->AddPoint(0.8 * max, 0.90);
  // Disable default opacity mapping in actor, and set transfer function on
  // volume property.
  eleVolActor->UseDefaultOpacityMapOff();
  eleVolActor->GetVolumeProperty()->SetScalarOpacity(volOpacity.GetPointer());

  //--------------------------------------------------------------------------
  // Electron cloud
  vtkNew<vtkElectronMapper> eleCPMapper;
  eleCPMapper->SetInput(mol);
  eleCPMapper->UseMolecularOrbital(3);
  eleCPMapper->SetContourPlane(plane.GetPointer());
  eleCPMapper->SetNumberOfContours(100);

  vtkNew<vtkElectronActor> eleCPActor;
  eleCPActor->SetMapper(eleCPMapper.GetPointer());
  eleCPActor->SetRenderStyleToContourPlane();

  // Set custom color mapping -- Gradient from blue to red
  vtkNew<vtkColorTransferFunction> cpColor;
  cpColor->AddHSVPoint(                min, 0.33, 0.75, 1.0);
  cpColor->AddHSVPoint(min + 0.45 * spread, 0.33, 1.00, 0.5);
  cpColor->AddHSVPoint(min + 0.55 * spread, 0.15, 1.00, 0.5);
  cpColor->AddHSVPoint(                max, 0.15, 0.75, 1.0);
  // Disable default color mapping in actor, and set transfer function on
  // mapper's lookup table.
  eleCPActor->UseDefaultColorMapOff();
  eleCPMapper->GetContourPlaneMapper()->SetLookupTable(cpColor.GetPointer());

  //--------------------------------------------------------------------------
  // Rendering setup
  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win.GetPointer());

  ren->AddActor(molActor.GetPointer());
  ren->AddActor(eleIsoActor.GetPointer());
  ren->AddActor(eleVolActor.GetPointer());
  ren->AddActor(eleCPActor.GetPointer());

  ren->SetBackground(0.0, 0.0, 0.0);
  win->SetSize(450,450);
  win->Render();
  ren->GetActiveCamera()->Zoom(2.4);
  win->Render();

  int retVal = vtkRegressionTestImage(win.GetPointer());
  if ( retVal == vtkRegressionTester::DO_INTERACTOR)
    {
    iren->Start();
    }

  return retVal;
}
