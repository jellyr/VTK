/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVectorBase.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkVectorBase - Storage and manipulations of fixed-size vectors.
//
// .SECTION Description
// This class is a templated data type for storing and manipulating fixed
// size vectors, which can be used to represent two and three dimensional
// points. The memory layout is a contiguous array of the specified type,
// such that a float[2] can be cast to a vtkVectorBase<float, 2> and
// manipulated. Also a float[6] could be cast and used as a
// vtkVectorBase<float, 2>[3].

#ifndef __vtkVectorBase_h
#define __vtkVectorBase_h

#include <assert.h>

template<typename T, int Size>
class vtkVectorBase
{
public:
  // Description:
  // Construct and initialize vector to zero.
  explicit vtkVectorBase()
  {
    for (int i = 0; i < Size; ++i)
      {
      Data[i] = 0;
      }
  }

  // Description:
  // Construct and initialize vector with values in init.
  explicit vtkVectorBase(const T* init)
  {
    this->InternalAssignment(init);
  }

  // Description:
  // Set values in this vector with values in init.
  vtkVectorBase<T, Size> & operator=(const T* init)
  {
    this->InternalAssignment(init);
    return this;
  }

  // Description:
  // Get the size of the vtkVector.
  int GetSize() const { return Size; }

  // Description:
  // Get a pointer to the underlying data of the vtkVector.
  T* GetData() { return this->Data; }
  const T* GetData() const { return this->Data; }

  // Description:
  // Get a reference to the underlying data element of the
  // vtkVector. Can be used in much the same way as vector[i] is
  // used. No boundary checking is performed, so this method is faster
  // than operator().
  T& operator[](int i) { return this->Data[i]; }
  const T& operator[](int i) const { return this->Data[i]; }

  // Description:
  // Get the value of the vector at the index specified. Does bounds
  // checking, used in much the same way as vector.at(i) is used.
  // \pre index_in_bounds i>=0 && i<Size
  T operator()(int i) const
  {
    assert("pre: index_in_bounds" && i>=0 && i<Size);
    return this->Data[i];
  }

  // Description:
  // Return the L2 norm (length) of this vector
  T Norm() const
  {
    return sqrt(this->SquaredNorm());
  }

  // Description:
  // Return the squared norm of this vector
  T SquaredNorm() const
  {
    T result = 0;
    for (int i = 0; i < Size; ++i)
      {
      result += this->Data[i] * this->Data[i];
      }
    return result;
  }

  // Description:
  // Return the L2 norm (length) of this vector and normalize in
  // place.
  // \sa Normalized()
  T Normalize()
  {
    const T norm = this->Norm();
    const T invNorm = 1.0/norm;
    for (int i = 0; i < Size; ++i)
      {
      this->Data[i] *= invNorm;
      }
    return norm;
  }

 protected:
  // Description:
  // Cast the vector to the specified type.
  template<typename TR> void InternalCast(TR *ret) const
  {
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = static_cast<TR>(this->Data[i]);
      }
  }

  // Description:
  // Construct and initialize vector.
  explicit vtkVectorBase(const T &x, const T &y)
  {
    Data[0] = x;
    Data[1] = y;
  }

  // Description:
  // Construct and initialize vector.
  explicit vtkVectorBase(const T &x, const T &y, const T &z)
  {
    Data[0] = x;
    Data[1] = y;
    Data[2] = z;
  }

  // Description:
  // Construct and initialize vector.
  explicit vtkVectorBase(const T &x, const T &y, const T &z, const T &w)
  {
    Data[0] = x;
    Data[1] = y;
    Data[2] = z;
    Data[3] = w;
  }

  // Description:
  // Add this vector to \a other and store the result in \a ret.
  void InternalAdd(const T *other, T *ret) const
  {
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = this->Data[i] + other[i];
      }
  }

  // Description:
  // Return the negative of this vector
  void InternalUnitaryMinus(T *ret) const
  {
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = -this->Data[i];
      }
  }

  // Description:
  // Subtract \a other from this vector and store the result in \a ret
  void InternalSubtract(const T *other, T *ret) const
  {
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = this->Data[i] - other[i];
      }
  }

  // Description:
  // Multiply this vector by \a scalar and store the result in \a ret
  void InternalMultiplyScalar(const T &scalar, T *ret) const
  {
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = this->Data[i] * scalar;
      }
  }

  // Description:
  // Divide this vector by \a scalar and store the result in \a ret.
  void InternalDivideScalar(const T &scalar, T *ret) const
  {
    return (*this) * (1.0/scalar);
  }

  // Description:
  // Assignment operator
  void InternalAssignment(const T *other)
  {
    for (int i = 0; i < Size; ++i)
      {
      this->Data[i] = other[i];
      }
  }

  // Description:
  // Equality operator. Use this for integral types, otherwise, see Compare.
  bool InternalExactEquality(const T *other)
  {
    for (int i = 0; i < Size; ++i)
      {
      if (this->Data[i] != other[i])
        {
        return false;
        }
      }
    return true;
  }

  // Description:
  // Inequality operator. Use this for integral types, otherwise, see Compare.
  bool InternalExactInequality(const T *other)
  {
    return !this->InternalExactEquality(other);
  }

  // Description:
  // Equality operator with a tolerance. Use this for floating point
  // types, otherwise, see operator==.
  bool InternalFuzzyCompare(const T *other, const T &tol)
  {
    for (int i = 0; i < Size; ++i)
      {
      if (fabs(this->Data[i] - other[i]) >= tol)
        {
        return false;
        }
      }
    return true;
  }

  // Description:
  // Return the dot product of this vector and \a other
  T InternalDot(const T *other) const
  {
    T result = 0;
    for (int i = 0; i < Size; ++i)
      {
      result += this->Data[i] * other[i];
      }
    return result;
  }

  // Description:
  // Return the cross product of this vector and \a other. This is only
  // implemented for vectors of size 3
  void InternalCross(const T *other, T *ret) const
  {
    ret[0] = this->Data[1]*other[2] - this->Data[2]*other[1];
    ret[1] = this->Data[2]*other[0] - this->Data[0]*other[2];
    ret[2] = this->Data[0]*other[1] - this->Data[1]*other[0];
  }

  // Description:
  // Return a normalized vector in the direction of this vector.
  // \sa Normalize()
  void InternalNormalized(T *ret) const
  {
    T invNorm = 1.0/this->Norm();
    for (int i = 0; i < Size; ++i)
      {
      ret[i] = this->Data[i] * invNorm;
      }
  }


  // Description:
  // The only thing stored in memory!
  T Data[Size];
};

#endif
