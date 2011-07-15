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

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkElectronActor.h"
#include "vtkElectronMapper.h"
#include "vtkMolecule.h"
#include "vtkMoleculeMapper.h"
#include "vtkNew.h"
#include "vtkOpenQubeMoleculeSource.h"
#include "vtkPlane.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSimpleBondPerceiver.h"
#include "vtkSmartPointer.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkVolumeProperty.h"

int TestElectronMapperClipAlongBond(int argc, char *argv[])
{
  char* fname = vtkTestUtilities::ExpandDataFileName(
    argc, argv, "Data/2h2o.out");

  vtkNew<vtkOpenQubeMoleculeSource> oq;
  oq->SetFileName(fname);
  oq->Update();

  vtkSmartPointer<vtkMolecule> mol = vtkSmartPointer<vtkMolecule>::New();
  mol = oq->GetOutput();

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
  // Molecular geometry
  vtkNew<vtkMoleculeMapper> molMapper;
  molMapper->SetInput(mol);
  molMapper->UseLiqouriceStickSettings();
  molMapper->SetBondRadius(0.1);
  molMapper->SetAtomicRadiusScaleFactor(0.1);

  vtkNew<vtkActor> molActor;
  molActor->SetMapper(molMapper.GetPointer());

  //--------------------------------------------------------------------------
  // Electron cloud
  vtkNew<vtkElectronMapper> eleMapper;
  eleMapper->SetInput(mol);
  eleMapper->UseMolecularOrbital(3);
  eleMapper->SetIsosurfaceValue(0.1);

  // Add clipping plane
  vtkAtom O1 = mol->GetAtom(0);
  vtkAtom H1 = mol->GetAtom(3);
  vtkAtom H2 = mol->GetAtom(2);
  vtkVector3d O1pos = O1.GetPositionAsVector3d();
  vtkVector3d H1pos = H1.GetPositionAsVector3d();
  vtkVector3d H2pos = H2.GetPositionAsVector3d();

  // Add clipping plane
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
  eleMapper->AddClippingPlane(plane.GetPointer());

  vtkNew<vtkElectronActor> eleActor;
  eleActor->SetMapper(eleMapper.GetPointer());
  eleActor->SetRenderStyleToIsosurface();
  eleActor->GetIsosurfaceProperty()->SetOpacity(0.5);

  //--------------------------------------------------------------------------
  // Electron cloud
  vtkNew<vtkElectronMapper> eleVolumeMapper;
  eleVolumeMapper->SetInput(mol);
  eleVolumeMapper->UseMolecularOrbital(3);

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
  eleVolumeMapper->AddClippingPlane(mirroredPlane.GetPointer());

  vtkNew<vtkElectronActor> eleVolumeActor;
  eleVolumeActor->SetMapper(eleVolumeMapper.GetPointer());
  eleVolumeActor->SetRenderStyleToVolume();

  //--------------------------------------------------------------------------
  // Electron cloud
  vtkNew<vtkElectronMapper> eleContourMapper;
  eleContourMapper->SetInput(mol);
  eleContourMapper->UseMolecularOrbital(3);
  eleContourMapper->SetContourPlane(plane.GetPointer());

  vtkNew<vtkElectronActor> eleContourActor;
  eleContourActor->SetMapper(eleContourMapper.GetPointer());
  eleContourActor->SetRenderStyleToContourPlane();

  //--------------------------------------------------------------------------
  // Rendering setup
  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win.GetPointer());

  ren->AddActor(molActor.GetPointer());
  ren->AddActor(eleActor.GetPointer());
  ren->AddActor(eleVolumeActor.GetPointer());
  ren->AddActor(eleContourActor.GetPointer());

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
