/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVectorForwardDeclarations.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkVectorForwardDeclarations_h
#define __vtkVectorForwardDeclarations_h

#ifndef __vtkVector_h
template<typename T, int Size> class vtkVector;
#endif

typedef vtkVector<unsigned int, 2> vtkVector2u;
typedef vtkVector<unsigned int, 3> vtkVector3u;
typedef vtkVector<unsigned int, 4> vtkVector4u;

typedef vtkVector<int, 2> vtkVector2i;
typedef vtkVector<int, 3> vtkVector3i;
typedef vtkVector<int, 4> vtkVector4i;

typedef vtkVector<float, 2> vtkVector2f;
typedef vtkVector<float, 3> vtkVector3f;
typedef vtkVector<float, 4> vtkVector4f;

typedef vtkVector<double, 2> vtkVector2d;
typedef vtkVector<double, 3> vtkVector3d;
typedef vtkVector<double, 4> vtkVector4d;

#endif
