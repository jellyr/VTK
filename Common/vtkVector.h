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

// .NAME vtkVector - Storage of unsigned/signed int, float, or double vectors
// of size 2, 3, or 4.
//
// .SECTION Description
// This class is a templated data type for storing and manipulating fixed size
// vectors, which can be used to represent two and three dimensional points. The
// memory layout is a contiguous array of the specified type, such that a
// float[2] can be cast to a vtkVector2f and manipulated. Also a float[6] could
// be cast and used as a vtkVector2f[3].

/* This comment is to stop the doc parsers from continuing. */

// If this is the first time this file has been included, run through the
// class declaration, compile definitions, and re-inclusions of this file in
// the section below. If this pass through the file is from one of the
// following reinclusions, skip to the actual macro'd implementation section.
#ifndef __vtkVector_concrete_generating

#ifndef __vtkVector_h
#define __vtkVector_h

// This header defines the base template interface for vtkVector.
#include <vtkVectorBase.h>

// For backwards compatibility:
template<typename T, int Size>
class vtkVector : public vtkVectorBase<T, Size>
  {
public:
  explicit vtkVector() : vtkVectorBase<T, Size>() {}
  explicit vtkVector(const T *init) : vtkVectorBase<T, Size>(init) {}
  };

// Generate the concrete classes of vtkVector:
#define __vtkVector_concrete_generating

// Set the following compiler definitions. These will be expanded in the
// preprocessor to form the concrete implementation when this file is
// reincluded.

// Unsigned integer vectors, vtkVector[234]u
#define __vtkVector_concrete_type unsigned int

#define __vtkVector_concrete_size 2
#define __vtkVector_concrete_class vtkVector2u
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 3
#define __vtkVector_concrete_class vtkVector3u
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 4
#define __vtkVector_concrete_class vtkVector4u
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#undef __vtkVector_concrete_type

// Integer vectors, vtkVector[234]i
#define __vtkVector_concrete_type int

#define __vtkVector_concrete_size 2
#define __vtkVector_concrete_class vtkVector2i
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 3
#define __vtkVector_concrete_class vtkVector3i
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 4
#define __vtkVector_concrete_class vtkVector4i
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#undef __vtkVector_concrete_type

// Float vectors, vtkVector[234]f
#define __vtkVector_concrete_type float

#define __vtkVector_concrete_size 2
#define __vtkVector_concrete_class vtkVector2f
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 3
#define __vtkVector_concrete_class vtkVector3f
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 4
#define __vtkVector_concrete_class vtkVector4f
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#undef __vtkVector_concrete_type

// Double vectors, vtkVector[234]d
#define __vtkVector_concrete_type double

#define __vtkVector_concrete_size 2
#define __vtkVector_concrete_class vtkVector2d
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 3
#define __vtkVector_concrete_class vtkVector3d
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#define __vtkVector_concrete_size 4
#define __vtkVector_concrete_class vtkVector4d
#include "vtkVector.h"
#undef __vtkVector_concrete_size
#undef __vtkVector_concrete_class

#undef __vtkVector_concrete_type

// Done generating concrete classes.
#undef __vtkVector_concrete_generating

#endif // __vtkVector_h
#endif // ifndef __vtkVector_concrete_generating

//----------------------------------------------------------------------------
// The following section actually generates the implementation of the
// class. The compiler definitions:
//
// - __vtkVector_concrete_class: class name
// - __vtkVector_concrete_type: typename T template arg
// - __vtkVector_concrete_size: int Size template arg
//
// are expanded in the preprocessor to appear as a full implementation
// of the specified class.
#ifdef __vtkVector_concrete_generating

#if !defined(__vtkVector_concrete_class)
#error Concrete class name undefined!
#endif

#if !defined(__vtkVector_concrete_type)
#error Concrete class type undefined!
#endif

#if !defined(__vtkVector_concrete_size)
#error Concrete class size undefined!
#endif

class __vtkVector_concrete_class :
  public vtkVectorBase< __vtkVector_concrete_type , __vtkVector_concrete_size>
{
 public:

  // Description:
  // Construct and initialize vector to zero.
  __vtkVector_concrete_class() : vtkVectorBase() {}

  // Description:
  // Copy constructor
  __vtkVector_concrete_class(const __vtkVector_concrete_class &init)
    : vtkVectorBase(init.Data) {}

  // Description:
  // Construct and initialize vector with values in init.
  explicit __vtkVector_concrete_class(const __vtkVector_concrete_type *init)
    : vtkVectorBase(init) {}

#if __vtkVector_concrete_size == 2
  // Description:
  // Construct and initialize vector.
  explicit __vtkVector_concrete_class(const __vtkVector_concrete_type &x,
                                      const __vtkVector_concrete_type &y)
    : vtkVectorBase(x, y) {}

  // Description:
  // Set the x and y components of the vector.
  void Set(const __vtkVector_concrete_type &x,
           const __vtkVector_concrete_type &y)
  {
    this->Data[0] = x;
    this->Data[1] = y;
  }
#endif

#if __vtkVector_concrete_size == 3
  // Description:
  // Construct and initialize vector.
  explicit __vtkVector_concrete_class(const __vtkVector_concrete_type &x,
                                      const __vtkVector_concrete_type &y,
                                      const __vtkVector_concrete_type &z)
    : vtkVectorBase(x, y, z) {}

  // Description:
  // Set the x, y, and z components of the vector.
  void Set(const __vtkVector_concrete_type &x,
           const __vtkVector_concrete_type &y,
           const __vtkVector_concrete_type &z)
  {
    this->Data[0] = x;
    this->Data[1] = y;
    this->Data[2] = z;
  }
#endif

#if __vtkVector_concrete_size == 4
  // Description:
  // Construct and initialize vector.
  explicit __vtkVector_concrete_class(const __vtkVector_concrete_type &x,
                                      const __vtkVector_concrete_type &y,
                                      const __vtkVector_concrete_type &z,
                                      const __vtkVector_concrete_type &w)
    : vtkVectorBase(x, y, z, w) {}

  // Description:
  // Set the x, y, z, and w components of the vector.
  void Set(const __vtkVector_concrete_type &x,
           const __vtkVector_concrete_type &y,
           const __vtkVector_concrete_type &z,
           const __vtkVector_concrete_type &w)
  {
    this->Data[0] = x;
    this->Data[1] = y;
    this->Data[2] = z;
    this->Data[3] = w;
  }
#endif

#if __vtkVector_concrete_size >= 2
  // Description:
  // Set the x component of the vector, i.e. element 0.
  void SetX(const __vtkVector_concrete_type & x)
  {
    this->Data[0] = x;
  }

  // Description:
  // Get the x component of the vector, i.e. element 0.
  const __vtkVector_concrete_type & GetX() const
  {
    return this->Data[0];
  }
  const __vtkVector_concrete_type & X() const
  {
    return this->Data[0];
  }
  __vtkVector_concrete_type & GetX()
  {
    return this->Data[0];
  }
  __vtkVector_concrete_type & X()
  {
    return this->Data[0];
  }

  // Description:
  // Set the y component of the vector, i.e. element 1.
  void SetY(const __vtkVector_concrete_type & y)
  {
    this->Data[1] = y;
  }

  // Description:
  // Get the y component of the vector, i.e. element 1.
  const __vtkVector_concrete_type & GetY() const
  {
    return this->Data[1];
  }
  const __vtkVector_concrete_type & Y() const
  {
    return this->Data[1];
  }
  __vtkVector_concrete_type & GetY()
  {
    return this->Data[1];
  }
  __vtkVector_concrete_type & Y()
  {
    return this->Data[1];
  }
#endif

#if __vtkVector_concrete_size >= 3
  // Description:
  // Set the z component of the vector, i.e. element 2.
  void SetZ(const __vtkVector_concrete_type & z)
  {
    this->Data[2] = z;
  }

  // Description:
  // Get the z component of the vector, i.e. element 2.
  const __vtkVector_concrete_type & GetZ() const
  {
    return this->Data[2];
  }
  const __vtkVector_concrete_type & Z() const
  {
    return this->Data[2];
  }
  __vtkVector_concrete_type & GetZ()
  {
    return this->Data[2];
  }
  __vtkVector_concrete_type & Z()
  {
    return this->Data[2];
  }
#endif

#if __vtkVector_concrete_size >= 4
  // Description:
  // Set the w component of the vector, i.e. element 3.
  void SetW(const __vtkVector_concrete_type & w)
  {
    this->Data[3] = w;
  }

  // Description:
  // Get the w component of the vector, i.e. element 3.
  const __vtkVector_concrete_type & GetW() const
  {
    return this->Data[3];
  }
  const __vtkVector_concrete_type & W() const
  {
    return this->Data[3];
  }
  __vtkVector_concrete_type & GetW()
  {
    return this->Data[3];
  }
  __vtkVector_concrete_type & W()
  {
    return this->Data[3];
  }
#endif

  // Description:
  // Cast this vector to the specified type and overwrite \a other.
  template<typename TR>
  void Cast(vtkVectorBase<TR, __vtkVector_concrete_size> *other) const
  {
    this->InternalCast<TR>(other->GetData());
  }

  // Description:
  // Add this vector to \a other and return the result.
  __vtkVector_concrete_class
  operator+(const __vtkVector_concrete_class &other) const
  {
    __vtkVector_concrete_class result;
    this->InternalAdd(other.Data, result.Data);
    return result;
  }

  // Description:
  // Add \a other to this vector
  __vtkVector_concrete_class &
  operator+=(const __vtkVector_concrete_class &other)
  {
    this->InternalAdd(other.Data, this->Data);
    return *this;
  }

  // Description:
  // Return the negative of this vector
  __vtkVector_concrete_class
  operator-() const
  {
    __vtkVector_concrete_class result;
    this->InternalUnitaryMinus(result.Data);
    return result;
  }

  // Description:
  // Subtract \a other from this vector and return the result.
  __vtkVector_concrete_class
  operator-(const __vtkVector_concrete_class &other) const
  {
    __vtkVector_concrete_class result;
    this->InternalSubtract(other.Data, result.Data);
    return result;
  }

  // Description:
  // Subtract \a other from this vector
  __vtkVector_concrete_class &
  operator-=(const __vtkVector_concrete_class &other)
  {
    this->InternalSubtract(other.Data, this->Data);
    return *this;
  }

  // Description:
  // Multiply this vector by \a scalar and return the result
  __vtkVector_concrete_class
  operator*(const __vtkVector_concrete_type &scalar) const
  {
    __vtkVector_concrete_class result;
    this->InternalMultiplyScalar(scalar, result.Data);
    return result;
  }

  // Description:
  // Multiply this vector by \a scalar
  __vtkVector_concrete_class &
  operator*=(const __vtkVector_concrete_type &scalar)
  {
    this->InternalMultiplyScalar(scalar, this->Data);
    return *this;
  }

  // Description:
  // Divide this vector by \a scalar and return the result
  __vtkVector_concrete_class
  operator/(const __vtkVector_concrete_type &scalar) const
  {
    return (*this) * (1.0/scalar);
  }

  // Description:
  // Divide this vector by \a scalar
  __vtkVector_concrete_class &
  operator/=(const __vtkVector_concrete_type &scalar)
  {
    return (*this) *= (1.0/scalar);
  }

  // Description:
  // Assignment operator
  __vtkVector_concrete_class &
  operator=(const __vtkVector_concrete_class &other)
  {
    this->InternalAssignment(other.Data);
  }

  // Description:
  // Assignment operator
  __vtkVector_concrete_class &
  operator=(const __vtkVector_concrete_type *other)
  {
    this->InternalAssignment(other);
  }

  // Description:
  // Equality operator. Use this for integral types, otherwise, see
  // Compare.
  bool operator==(const __vtkVector_concrete_class &other)
  {
    return this->InternalExactEquality(other.Data);
  }

  // Description:
  // Inequality operator. Use this for integral types, otherwise, see
  // Compare.
  bool operator!=(const __vtkVector_concrete_class &other)
  {
    return this->InternalExactInequality(other.Data);
  }

  // Description:
  // Equality operator with a tolerance. Use this for floating point
  // types, otherwise, see operator==.
  bool Compare(const __vtkVector_concrete_class &other,
               const __vtkVector_concrete_type &tol)
  {
    return this->InternalFuzzyCompare(other.Data, tol);
  }

  // Description:
  // Return the dot product of this vector and \a other
  __vtkVector_concrete_type
  Dot(const __vtkVector_concrete_class &other) const
  {
    return this->InternalDot(other.Data);
  }

#if __vtkVector_concrete_size == 3
  // Description:
  // Return the cross product of this vector and \a other.
  __vtkVector_concrete_class
  Cross(const __vtkVector_concrete_class &other) const
  {
    __vtkVector_concrete_class ret;
    this->InternalCross(other.Data, ret.Data);
    return ret;
  }
#endif

  // Description:
  // Return a normalized vector in the direction of this vector.
  // \sa Normalize()
  __vtkVector_concrete_class Normalized() const
  {
     __vtkVector_concrete_class result;
    this->InternalNormalized(result.Data);
    return result;
  }

};

#endif // ifndef__vtkVector_concrete_generating
