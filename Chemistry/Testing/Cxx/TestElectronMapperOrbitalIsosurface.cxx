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
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSimpleBondPerceiver.h"
#include "vtkSmartPointer.h"
#include "vtkVolumeProperty.h"

#include <openqube/basissetloader.h>
#include <openqube/basisset.h>

int TestElectronMapperOrbitalIsosurface(int argc, char *argv[])
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
  eleMapper->UseMolecularOrbital(4);
  eleMapper->SetIsosurfaceValue(0.025);

  vtkNew<vtkElectronActor> eleActor;
  eleActor->SetMapper(eleMapper.GetPointer());
  eleActor->SetRenderStyleToIsosurface();
  eleActor->GetIsosurfaceProperty()->SetOpacity(0.25);

  //--------------------------------------------------------------------------
  // Rendering setup
  vtkNew<vtkRenderer> ren;
  vtkNew<vtkRenderWindow> win;
  win->AddRenderer(ren.GetPointer());
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(win.GetPointer());

  ren->AddActor(molActor.GetPointer());
  ren->AddActor(eleActor.GetPointer());

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
