/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOpenQubeMoleculeSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkOpenQubeMoleculeSource - Read a OpenQube readable file and output
// a vtkMolecule object
//
// .SECTION Description

#ifndef __vtkOpenQubeMoleculeSource_h
#define __vtkOpenQubeMoleculeSource_h

#include "vtkDataReader.h"

class vtkMolecule;

namespace OpenQube
{
  class Molecule;
  class BasisSet;
}

class VTK_CHEMISTRY_EXPORT vtkOpenQubeMoleculeSource : public vtkDataReader
{
public:
  static vtkOpenQubeMoleculeSource *New();
  vtkTypeMacro(vtkOpenQubeMoleculeSource,vtkDataReader);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get/Set the output (vtkMolecule) that the reader will fill
  vtkMolecule *GetOutput();
  void SetOutput(vtkMolecule *);

  // Description:
  // Get/Set the name of the OpenQube readable file.
  // @warning: If a both a source OpenQube BasisSet object and a FileName
  // have been set with SetBasisSet and SetFileName, the object takes
  // precedence over the file and the file will not be read.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // Get/Set the OpenQube BasisSet object to read from.
  // @warning: If a both a source OpenQube BasisSet object and a FileName
  // have been set with SetBasisSet and SetFileName, the object takes
  // precedence over the file and the file will not be read.
  vtkSetMacro(BasisSet, OpenQube::BasisSet*);
  vtkGetMacro(BasisSet, OpenQube::BasisSet*);

protected:
  vtkOpenQubeMoleculeSource();
  ~vtkOpenQubeMoleculeSource();

  int RequestData(vtkInformation *, vtkInformationVector **,
                  vtkInformationVector *);
  int FillOutputPortInformation(int, vtkInformation*);

  char *FileName;
  OpenQube::BasisSet *BasisSet;

  // Description:
  // Copy the OpenQube::Molecule object @a oqmol into into the provided
  // vtkMolecule object @a mol.
  void CopyOQMoleculeToVtkMolecule(const OpenQube::Molecule *oqmol,
                                   vtkMolecule *mol);

private:
  // Not implemented:
  vtkOpenQubeMoleculeSource(const vtkOpenQubeMoleculeSource&);
  void operator=(const vtkOpenQubeMoleculeSource&);
};

#endif
