/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVectorOperators.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkVectorOperators_h
#define __vtkVectorOperators_h

// This set of operators enhance the vtkVector classes, allowing various
// operator overloads one might expect.
#include "vtkVector.h"
#include "vtkIOStream.h"

// Description:
// Output the contents of a vector, mainly useful for debugging.
template<typename A, int Size>
ostream& operator<<(ostream& out, const vtkVector<A, Size>& v)
{
  out << "(";
  bool first = true;
  for (int i = 0; i < Size; ++i)
    {
    if (first)
      {
      first = false;
      }
    else
      {
      out << ", ";
      }
    out << v[i];
    }
  out << ")";
  return out;
}

// Description:
// Equality operator performs an equality check on each component.
template<typename A, int Size>
bool operator==(const vtkVector<A, Size>& v1, const vtkVector<A, Size>& v2)
{
  bool ret = true;
  for (int i = 0; i < Size; ++i)
    {
    if (v1[i] != v2[i])
      {
      ret = false;
      }
    }
  return ret;
}

// Description:
// Inequality for vector type.
template<typename A, int Size>
bool operator!=(const vtkVector<A, Size>& v1, const vtkVector<A, Size>& v2)
{
  return !(v1 == v2);
}

// Description:
// Performs addition of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> operator+(const vtkVector<A, Size>& v1,
                             const vtkVector<A, Size>& v2)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v1[i] + v2[i];
    }
  return ret;
}

// Description:
// Performs compound addition of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> & operator+=(vtkVector<A, Size>& v1,
                                const vtkVector<A, Size>& v2)
{
  for (int i = 0; i < Size; ++i)
    {
    v1[i] += v2[i];
    }
  return v1;
}

// Description:
// Performs subtraction of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> operator-(const vtkVector<A, Size>& v1,
                             const vtkVector<A, Size>& v2)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v1[i] - v2[i];
    }
  return ret;
}

// Description:
// Performs compound subtraction of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> & operator-=(vtkVector<A, Size>& v1,
                                const vtkVector<A, Size>& v2)
{
  for (int i = 0; i < Size; ++i)
    {
    v1[i] -= v2[i];
    }
  return v1;
}

// Description:
// Negates the vector
template<typename A, int Size>
vtkVector<A, Size> operator-(const vtkVector<A, Size>& v)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = -v[i];
    }
  return ret;
}

// Description:
// Performs multiplication of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> operator*(const vtkVector<A, Size>& v1,
                             const vtkVector<A, Size>& v2)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v1[i] * v2[i];
    }
  return ret;
}

// Description:
// Performs compound multiplication of vectors of the same basic type.
template<typename A, int Size>
vtkVector<A, Size> & operator*=(vtkVector<A, Size>& v1,
                                const vtkVector<A, Size>& v2)
{
  for (int i = 0; i < Size; ++i)
    {
    v1[i] *= v2[i];
    }
  return v1;
}

// Description:
// Performs scalar multiplication.
template<typename A, int Size>
vtkVector<A, Size> operator*(const A& scalar,
                             const vtkVector<A, Size>& v)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v[i] * scalar;
    }
  return ret;
}

// Description:
// Performs scalar multiplication.
template<typename A, int Size>
vtkVector<A, Size> operator*(const vtkVector<A, Size>& v,
                             const A& scalar)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v[i] * scalar;
    }
  return ret;
}

// Description:
// Performs compound scalar multiplication.
template<typename A, int Size>
vtkVector<A, Size> & operator*=(vtkVector<A, Size>& v,
                                const A& scalar)
{
  for (int i = 0; i < Size; ++i)
    {
    v[i] *= scalar;
    }
  return v;
}

// Description:
// Performs division of vectors of the same type.
template<typename A, int Size>
vtkVector<A, Size> operator/(const vtkVector<A, Size>& v1,
                             const vtkVector<A, Size>& v2)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v1[i] / v2[i];
    }
  return ret;
}

// Description:
// Performs compound division of vectors of the same type.
template<typename A, int Size>
vtkVector<A, Size> & operator/=(vtkVector<A, Size>& v1,
                                const vtkVector<A, Size>& v2)
{
  for (int i = 0; i < Size; ++i)
    {
    v1[i] /= v2[i];
    }
  return v1;
}

// Description:
// Performs scalar division.
template<typename A, int Size>
vtkVector<A, Size> operator/=(const A& scalar,
                              const vtkVector<A, Size>& v)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = scalar / v[i];
    }
  return ret;
}

// Description:
// Performs scalar division.
template<typename A, int Size>
vtkVector<A, Size> operator/=(const vtkVector<A, Size>& v,
                              const A& scalar)
{
  vtkVector<A, Size> ret;
  for (int i = 0; i < Size; ++i)
    {
    ret[i] = v[i] / scalar;
    }
  return ret;
}

// Description:
// Performs compound scalar division.
template<typename A, int Size>
vtkVector<A, Size> & operator/=(vtkVector<A, Size>& v,
                                const A& scalar)
{
  for (int i = 0; i < Size; ++i)
    {
    v[i] /= scalar;
    }
  return v;
}

// Description:
// Several macros to define the various operator overloads for the vectors.
#define vtkVectorOperatorPlus(vectorType, type, size) \
inline vectorType operator+(const vectorType& v1, const vectorType& v2) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v1) + \
    static_cast<vtkVector<type, size> >(v2)).GetData()); \
}
#define vtkVectorOperatorPlusAssignment(vectorType, type, size) \
inline vectorType & operator+=(vectorType& v1, const vectorType& v2) \
{ \
  return static_cast<vectorType>(static_cast<vtkVector<type, size> >(v1) += \
    static_cast<vtkVector<type, size> >(v2)); \
}
#define vtkVectorOperatorMinus(vectorType, type, size) \
inline vectorType operator-(const vectorType& v1, const vectorType& v2) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v1) - \
    static_cast<vtkVector<type, size> >(v2)).GetData()); \
}
#define vtkVectorOperatorMinusAssignment(vectorType, type, size) \
inline vectorType & operator-=(vectorType& v1, const vectorType& v2) \
{ \
  return static_cast<vectorType>(static_cast<vtkVector<type, size> >(v1) -= \
    static_cast<vtkVector<type, size> >(v2)); \
}
#define vtkVectorOperatorUnaryMinus(vectorType, type, size) \
inline vectorType operator-(const vectorType& v) \
{ \
  return vectorType((-static_cast<vtkVector<type, size> >(v)).GetData()); \
}
#define vtkVectorOperatorMultiply(vectorType, type, size) \
inline vectorType operator*(const vectorType& v1, const vectorType& v2) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v1) * \
    static_cast<vtkVector<type, size> >(v2)).GetData()); \
}
#define vtkVectorOperatorMultiplyAssignment(vectorType, type, size) \
inline vectorType & operator*=(vectorType& v1, const vectorType& v2) \
{ \
  return static_cast<vectorType>(static_cast<vtkVector<type, size> >(v1) *= \
    static_cast<vtkVector<type, size> >(v2)); \
}
#define vtkVectorOperatorScalarMultiplyLeft(vectorType, type, size) \
inline vectorType operator*(const type& scalar, const vectorType& v) \
{ \
  return vectorType((scalar * \
    static_cast<vtkVector<type, size> >(v)).GetData()); \
}
#define vtkVectorOperatorScalarMultiplyRight(vectorType, type, size) \
inline vectorType operator*(const vectorType& v, const type& scalar) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v) * \
    scalar).GetData()); \
}
#define vtkVectorOperatorScalarMultiplyAssignment(vectorType, type, size) \
inline vectorType & operator*=(vectorType& v, const type& scalar) \
{ \
  return static_cast<vectorType>(static_cast<vtkVector<type, size> >(v) *= \
    scalar); \
}
#define vtkVectorOperatorDivide(vectorType, type, size) \
inline vectorType operator/(const vectorType& v1, const vectorType& v2) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v1) / \
    static_cast<vtkVector<type, size> >(v2)).GetData()); \
}
#define vtkVectorOperatorScalarDivideLeft(vectorType, type, size) \
inline vectorType operator/(const type& scalar, const vectorType& v) \
{ \
  return vectorType((scalar / \
    static_cast<vtkVector<type, size> >(v)).GetData()); \
}
#define vtkVectorOperatorScalarDivideRight(vectorType, type, size) \
inline vectorType operator/(const vectorType& v, const type& scalar) \
{ \
  return vectorType((static_cast<vtkVector<type, size> >(v) / \
    scalar).GetData()); \
}
#define vtkVectorOperatorScalarDivideAssignment(vectorType, type, size) \
inline vectorType & operator/=(vectorType& v, const type& scalar) \
{ \
  return static_cast<vectorType>(static_cast<vtkVector<type, size> >(v) /= \
    scalar); \
}

#define vtkVectorOperatorMacro(vectorType, type, size) \
vtkVectorOperatorPlus(vectorType, type, size) \
vtkVectorOperatorPlusAssignment(vectorType, type, size) \
vtkVectorOperatorMinus(vectorType, type, size) \
vtkVectorOperatorMinusAssignment(vectorType, type, size) \
vtkVectorOperatorUnaryMinus(vectorType, type, size) \
vtkVectorOperatorMultiply(vectorType, type, size) \
vtkVectorOperatorMultiplyAssignment(vectorType, type, size) \
vtkVectorOperatorScalarMultiplyLeft(vectorType, type, size) \
vtkVectorOperatorScalarMultiplyRight(vectorType, type, size) \
vtkVectorOperatorScalarMultiplyAssignment(vectorType, type, size) \
vtkVectorOperatorDivide(vectorType, type, size) \
vtkVectorOperatorDivideAssignment(vectorType, type, size) \
vtkVectorOperatorScalarDivideLeft(vectorType, type, size) \
vtkVectorOperatorScalarDivideRight(vectorType, type, size) \
vtkVectorOperatorScalarDivideAssignment(vectorType, type, size)

// Description:
// Overload the operators for the common types.
vtkVectorOperatorMacro(vtkVector2i, int,    2)
vtkVectorOperatorMacro(vtkVector2f, float,  2)
vtkVectorOperatorMacro(vtkVector2d, double, 2)
vtkVectorOperatorMacro(vtkVector3i, int,    3)
vtkVectorOperatorMacro(vtkVector3f, float,  3)
vtkVectorOperatorMacro(vtkVector3d, double, 3)

#endif
