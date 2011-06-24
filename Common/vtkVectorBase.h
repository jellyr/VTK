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

// .NAME vtkVectorBase - templated base type for storage of vectors.
//
// .SECTION Description
// This class is a templated data type for storing and manipulating fixed
// size vectors, which can be used to represent two and three dimensional
// points. The memory layout is a contiguous array of the specified type,
// such that a float[2] can be cast to a vtkVector2f and manipulated. Also a
// float[6] could be cast and used as a vtkVector2f[3].
//
// This class should only be used as a parent class for types that would
// benefit from vector functionality (e.g. vtkVector, vtkColor, etc)

#ifndef __vtkVectorBase_h
#define __vtkVectorBase_h

#include <assert.h>

template<typename T, int Size>
class vtkVectorBase
{
public:
  // Description:
  // Construct an uninitialized vector.
  explicit vtkVectorBase() { }

  // Description:
  // Copy constructor
  vtkVectorBase(const vtkVectorBase<T, Size> &vector)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = vector.Data[i];
  }

  // Description:
  // Construct and fill with @a scalar
  explicit vtkVectorBase(const T &scalar)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = scalar;
  }

  // Description:
  // Construct and copy the first @a Size values in @a array
  explicit vtkVectorBase(const T *array)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = array[i];
  }

  // Description:
  // Get the size of the vtkVectorBase.
  int GetSize() const { return Size; }

  // Description:
  // Get a pointer to the underlying data of the vtkVectorBase.
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
  // Fill the vector with a tuple. This function does not perform boundary
  // checking, nor does it modify more than the specified number of values.
  vtkVectorBase<T, Size> & Set(const T &p1, const T &p2)
  {
    this->Data[0] = p1;
    this->Data[1] = p2;
    return *this;
  }
  vtkVectorBase<T, Size> & Set(const T &p1, const T &p2, const T &p3)
  {
    this->Data[0] = p1;
    this->Data[1] = p2;
    this->Data[2] = p3;
    return *this;
  }
  vtkVectorBase<T, Size> &
  Set(const T &p1, const T &p2, const T &p3, const T &p4)
  {
    this->Data[0] = p1;
    this->Data[1] = p2;
    this->Data[2] = p3;
    this->Data[3] = p4;
    return *this;
  }

  // Description:
  // Fill @a this with the first @a Size values in @a init
  vtkVectorBase<T, Size> & Set(const T *init)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = init[i];
    return *this;
  }

  // Description:
  // Fill @a this with @a scalar
  vtkVectorBase<T, Size> & Fill(const T &scalar)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = scalar;
    return *this;
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
      result += this->Data[i] * this->Data[i];
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
      this->Data[i] *= invNorm;
    return norm;
  }

  // Description:
  // Cast @a this to the specified type, store in @a target.
  //
  // Example:
  // @code
  // vtkVectorBase<float, 3> vec3f (1.0);
  // vtkVectorBase<double, 3> vec3d = vec3f.Cast<double>();
  // @endcode
  template<typename TR>
  void Cast(vtkVectorBase<TR, Size> *target) const
  {
    TR *targetArray = target->GetData();
    for (int i = 0; i < Size; ++i)
      targetArray[i] = static_cast<TR>(this->Data[i]);
  }

  // Description:
  // Add this vector to @a other and return the result.
  vtkVectorBase<T, Size> operator+(const vtkVectorBase<T, Size> &other) const
  {
    vtkVectorBase<T, Size> ret;
    for (int i = 0; i < Size; ++i)
      ret.Data[i] = this->Data[i] + other.Data[i];
    return ret;
  }
  vtkVectorBase<T, Size> & operator+=(const vtkVectorBase<T, Size> &other)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] += other.Data[i];
    return *this;
  }

  // Description:
  // Return the negative of this vector
  vtkVectorBase<T, Size> operator-() const
  {
    vtkVectorBase<T, Size> ret;
    for (int i = 0; i < Size; ++i)
      ret.Data[i] = -this->Data[i];
    return ret;
  }

  // Description:
  // Subtract @a other from @a this and return the result
  vtkVectorBase<T, Size> operator-(const vtkVectorBase<T, Size> &other) const
  {
    vtkVectorBase<T, Size> ret;
    for (int i = 0; i < Size; ++i)
      ret.Data[i] = this->Data[i] - other.Data[i];
    return ret;
  }
  vtkVectorBase<T, Size> & operator-=(const vtkVectorBase<T, Size> &other)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] -= other.Data[i];
    return *this;
  }

  // Description:
  // Multiply @a this by @a scalar and return the result
  vtkVectorBase<T, Size> operator*(const T &scalar) const
  {
    vtkVectorBase<T, Size> ret;
    for (int i = 0; i < Size; ++i)
      ret.Data[i] = this->Data[i] * scalar;
    return ret;
  }
  vtkVectorBase<T, Size> & operator*=(const T &scalar)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] *= scalar;
    return *this;
  }

  // Description:
  // Divide @a this by @a scalar and return the result.
  vtkVectorBase<T, Size> operator/(const T &scalar) const
  {
    vtkVectorBase<T, Size> ret;
    const T invScalar = 1.0/scalar;
    for (int i = 0; i < Size; ++i)
      ret.Data[i] = this->Data[i] * invScalar;
    return ret;
  }
  vtkVectorBase<T, Size> & operator/=(const T &scalar)
  {
    const T invScalar = 1.0/scalar;
    for (int i = 0; i < Size; ++i)
      this->Data[i] *= invScalar;
    return *this;
  }

  // Description:
  // Assignment operator
  vtkVectorBase<T, Size> & operator=(const vtkVectorBase<T, Size> &other)
  {
    for (int i = 0; i < Size; ++i)
      this->Data[i] = other.Data[i];
    return *this;
  }

  // Description:
  // Equality operator. Use this for integral types, otherwise, see Compare.
  bool operator==(const vtkVectorBase<T, Size> &other)
  {
    for (int i = 0; i < Size; ++i)
      if (this->Data[i] != other.Data[i])
        return false;
    return true;
  }

  // Description:
  // Inequality operator. Use this for integral types, otherwise, see Compare.
  bool operator!=(const vtkVectorBase<T, Size> &other)
  {
    return !(*this == other);
  }

  // Description:
  // Equality operator with a tolerance. Use this for floating point
  // types, otherwise, see operator==.
  bool Compare(const vtkVectorBase<T, Size> &other, const T tol)
  {
    for (int i = 0; i < Size; ++i)
      if (fabs(this->Data[i] - other.Data[i]) >= tol)
        return false;
    return true;
  }

  // Description:
  // Return the dot product of @a this and @a other
  T Dot(const vtkVectorBase<T, Size> &other) const
  {
    T result = 0;
    for (int i = 0; i < Size; ++i)
      result += this->Data[i] * other.Data[i];
    return result;
  }

  // Description:
  // Return the cross product of @a this X @a other. This is only
  // supported for vectors of size 3.
  vtkVectorBase<T, 3> Cross(const vtkVectorBase<T, 3> &other) const
  {
    vtkVectorBase<T, 3> ret;
    ret.Data[0] = this->Data[1]*other.Data[2] - this->Data[2]*other.Data[1];
    ret.Data[1] = this->Data[2]*other.Data[0] - this->Data[0]*other.Data[2];
    ret.Data[2] = this->Data[0]*other.Data[1] - this->Data[1]*other.Data[0];
    return ret;
  }

  // Description:
  // Return a normalized vector in the direction of this vector.
  // \sa Normalize()
  vtkVectorBase<T, Size> Normalized() const
  {
    return (*this) / this->Norm();
  }

protected:
  // Description:
  // The only thing stored in memory!
  T Data[Size];
};

#endif
