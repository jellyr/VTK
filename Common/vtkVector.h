/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVector.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkVector - templated base type for storage of vectors.
//
// .SECTION Description
// This class is a templated data type for storing and manipulating fixed
// size vectors, which can be used to represent two and three dimensional
// points. The memory layout is a contiguous array of the specified type,
// such that a float[2] can be cast to a vtkVector2f and manipulated. Also a
// float[6] could be cast and used as a vtkVector2f[3].

#ifndef __vtkVector_h
#define __vtkVector_h

#include "vtkVectorBase.h"

template<typename T, int Size>
class vtkVector : public vtkVectorBase<T, Size>
{
public:
  // Description:
  // Construct an uninitialized vector
  explicit vtkVector() {}

  // Description:
  // Copy constructor
  vtkVector(const vtkVector<T, Size> &vector)
    : vtkVectorBase<T, Size>(static_cast<vtkVectorBase<T, Size> >(vector)) { }
  vtkVector(const vtkVectorBase<T, Size> &vector)
    : vtkVectorBase<T, Size>(vector) { }

  // Description:
  // Construct and initialize all vector components to @a scalar.
  explicit vtkVector(const T &scalar) : vtkVectorBase<T, Size>(scalar) { }

  // Description:
  // Construct and copy the first @a Size values from @a array into @a this.
  explicit vtkVector(const T *array) : vtkVectorBase<T, Size>(array) { }

  // Description:
  // Construct and initialize vector with the passed tuple. This function
  // does not perform boundary checking, nor does it modify more than the
  // specified number of vector components.
  explicit vtkVector(const T &x, const T &y)
  {
    this->Set(x,y);
  }
  explicit vtkVector(const T &x, const T &y, const T &z)
  {
    this->Set(x,y,z);
  }
  explicit vtkVector(const T &x, const T &y, const T &z, const T &w)
  {
    this->Set(x,y,z,w);
  }

  // Description:
  // Cast @a this to the specified type and return the new vector
  template<typename TR>
  vtkVector<TR, Size> Cast() const
  {
    vtkVector<TR, Size> ret;
    this->vtkVectorBase<T, Size>::Cast(&ret);
    return ret;
  }

  // Description:
  // Set/Get values by component. This function does not perform boundary
  // checking, nor will it modify more than the specified component.
  void SetX(const T &x) { this->Data[0] = x; }
  const T & GetX() const { return this->Data[0]; }
  const T & X() const { return this->Data[0]; }
  T & GetX() { return this->Data[0]; }
  T & X() { return this->Data[0]; }

  void SetY(const T &y) { this->Data[1] = y; }
  const T & GetY() const { return this->Data[1]; }
  const T & Y() const { return this->Data[1]; }
  T & GetY() { return this->Data[1]; }
  T & Y() { return this->Data[1]; }

  void SetZ(const T &z) { this->Data[2] = z; }
  const T & GetZ() const { return this->Data[2]; }
  const T & Z() const { return this->Data[2]; }
  T & GetZ() { return this->Data[2]; }
  T & Z() { return this->Data[2]; }

  void SetW(const T &w) { this->Data[3] = w; }
  const T & GetW() const { return this->Data[3]; }
  const T & W() const { return this->Data[3]; }
  T & GetW() { return this->Data[3]; }
  T & W() { return this->Data[3]; }

  };

// Create typedefs
#include "vtkVectorForwardDeclarations.h"

#endif
