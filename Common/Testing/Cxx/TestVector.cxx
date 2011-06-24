/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestVector.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkColor.h"
#include "vtkObject.h"
#include "vtkVector.h"

#include <math.h> // For fabs

int TestVector(int,char *[])
{
  // Test out the general vector data types, give nice API and great memory use
  vtkVector2i vec2i;
  cout << "Size of vtkVector2i: " << sizeof(vec2i) << endl;
  int arr2i[2];

  // just to avoid warning
  arr2i[0]=0;
  arr2i[1]=0;

  cout << "Size of int[2]: " << sizeof(arr2i) << endl;

  if (sizeof(vec2i) != sizeof(arr2i))
    {
    // The two should be the same size and memory layout - error out if not
    cerr << "vtkVector2i should be the same size as int[2]." << endl
         << "sizeof(vec2i) = " << sizeof(vec2i) << endl
         << "sizeof(int[2]) = " << sizeof(arr2i) << endl;
    return EXIT_FAILURE;
    }

  vtkVector<float, 3> vector3f;
  if (vector3f.GetSize() != 3)
    {
    cerr << "Incorrect size of vector3f, should be 3, but is "
        << vector3f.GetSize() << endl;
    return EXIT_FAILURE;
    }

  // Test out vtkVector3i and ensure the various access methods are the same
  vtkVector3i vec3i(0, 6, 9);
  if (vec3i.X() != vec3i[0] || vec3i.X() != 0)
    {
    cerr << "vec3i.X() should equal vec3i.GetData()[0] which should equal 0."
        << "\nvec3i.X() = " << vec3i.X() << endl
        << "vec3i[0] = " << vec3i[0] << endl;
    return EXIT_FAILURE;
    }
  if (vec3i.Y() != vec3i[1] || vec3i.Y() != 6)
    {
    cerr << "vec3i.Y() should equal vec3i.GetData()[1] which should equal 6."
        << "\nvec3i.Y() = " << vec3i.Y() << endl
        << "vec3i[1] = " << vec3i[1] << endl;
    return EXIT_FAILURE;
    }
  if (vec3i.Z() != vec3i[2] || vec3i.Z() != 9)
    {
    cerr << "vec3i.Z() should equal vec3i.GetData()[2] which should equal 9."
        << "\nvec3i.Z() = " << vec3i.Z() << endl
        << "vec3i[2] = " << vec3i[2] << endl;
    return EXIT_FAILURE;
    }
  // Assign the data to an int array and ensure the two ways of referencing are
  // the same.
  int *intPtr = vec3i.GetData();
  for (int i = 0; i < 3; ++i)
    {
    if (vec3i[i] != intPtr[i] || vec3i(i) != vec3i[i])
      {
      cerr << "Error: vec3i[i] != intPtr[i]" << endl
          << "vec3i[i] = " << vec3i[i] << endl
          << "intPtr[i] = " << intPtr[i] << endl;
      return EXIT_FAILURE;
      }
    }

  // Test out casting...
  vtkVector3f castVec;
  vec3i.Cast<float>(&castVec);
  vtkVector3d castVecd;
  castVec.Cast<double>(&castVecd);
  if (fabs(castVecd[0]) > 1e-7)
    {
    // Then the number did not make it through within reasonable precision.
    cerr << "Error: castVecd value incorrect. Should be ~0.0 for component 1."
         << "\ncastVecd[0] = " << castVecd[0] << endl;
    return EXIT_FAILURE;
    }
  cout << "castVecd[0] = " << castVecd[0] << endl;

  // Now to test out one of the color classes and memory layouts of arrays
  // Note that the memory layout of a vtkColor3ub[5] is the same as an unsigned
  // char[15], and can be addressed as such.
  vtkColor3ub color[3];
  unsigned char* colorPtr = color->GetData();
  for (int i = 0; i < 3; ++i)
    {
    for (int j = 0; j < 3; ++j)
      {
      if (color[i][j] != 0)
        {
        cerr << "Initializer problem in vtkColor3ub - should be zero, but = "
             << color[i][j] << endl;
        return EXIT_FAILURE;
        }
      if (color[i][j] != colorPtr[i*3+j])
        {
        cerr << "Error: color[i][j] != colorPtr[i*3+j]" << endl
            << "color[i][j] = " << color[i][j] << endl
            << "colorPtr[i*3+j] = " << colorPtr[i*3+j] << endl;
        return EXIT_FAILURE;
        }
      color[i][j] = static_cast<unsigned char>(i * 2 + i);
      }
    }

  for (int i = 0; i < 3; ++i)
    {
    for (int j = 0; j < 3; ++j)
      {
      if (color[i][j] != colorPtr[i*3+j])
        {
        cerr << "Error: color[i][j] != colorPtr[i*3+j]" << endl
            << "color[i][j] = " << color[i][j] << endl
            << "colorPtr[i*3+j] = " << colorPtr[i*3+j] << endl;
        return EXIT_FAILURE;
        }
      }
    }

  // Test some of the math functions
  vtkVector3d v3d1 (1.3, 2.3, -1.5);
  vtkVector3d v3d2 (v3d1.GetData());
  vtkVector3d v3d3 (v3d1 + v3d2);

  if (!v3d3.Compare(v3d1 * 2, 1e-5))
    {
    cerr << "Vector v3d3 should equal 2*v3d1.\n"
         << "\tv3d3: " <<v3d3.X()<<" "<<v3d3.Y()<<" "<<v3d3.Z()<<"\n"
         << "\tv3d1: " <<v3d1.X()<<" "<<v3d1.Y()<<" "<<v3d1.Z()<<"\n"
         << "\t2*v3d1: " <<2*v3d1.X()<<" "<<2*v3d1.Y()<<" "<<2*v3d1.Z()<<"\n"
         << endl;
    return EXIT_FAILURE;
    }

  if (!v3d1.Compare(v3d2, 1e-5))
    {
    cerr << "Vector v3d1 should equal v3d2.\n"
         << "\tv3d1: " <<v3d1.X()<<" "<<v3d1.Y()<<" "<<v3d1.Z()<<"\n"
         << "\tv3d2: " <<v3d2.X()<<" "<<v3d2.Y()<<" "<<v3d2.Z()<<"\n"
         << endl;
    return EXIT_FAILURE;
    }

  vtkVector3d jhat;
  jhat.SetY(1.0);

  if (!(fabs(jhat.GetData()[0]) < 1e-5 &&
        fabs(jhat.GetData()[1] - 1.0) < 1e-5 &&
        fabs(jhat.GetData()[2]) < 1e-5))
    {
    cerr << "Vector jhat should be the unit-y vector\n"
         << "\tjhat: " <<jhat.X()<<" "<<jhat.Y()<<" "<<jhat.Z()<<"\n"
         << endl;
    return EXIT_FAILURE;
    }


  vtkVector3d v3d3Normalized = v3d3.Normalized();
  if (!fabs(v3d3Normalized.Norm() - 1.0 < 1e-5))
    {
    cerr << "Length of v3d3Normalized should be unity, is "
         << v3d3Normalized.Norm()<<"\n"
         << endl;
    return EXIT_FAILURE;
    }

  vtkVector3d crossProduct (jhat.Cross(v3d3Normalized));
  if (!(fabs(crossProduct.Norm() - 0.6533520534) < 1e-5))
    {
    cerr << "Length of cross product vector should be 0.6533520534, is "
         << crossProduct.Norm()<<"\n"
         << "\tcrossProduct: " <<crossProduct.X()<<" "<<crossProduct.Y()<<" "
         <<crossProduct.Z()<<"\n"
         << endl;
    return EXIT_FAILURE;
    }

  // Dot product of crossProduct with v3d3 or jhat should be 0.
  if (!(fabs(crossProduct.Dot(v3d3)) < 1e-5))
    {
    cerr << "Dot product of crossProduct and v3d3 should be 0, is "
         << crossProduct.Dot(v3d3)<<"\n"
         << endl;
    return EXIT_FAILURE;
    }
  if (!(fabs(crossProduct.Dot(jhat)) < 1e-5))
    {
    cerr << "Dot product of crossProduct and jhat should be 0, is "
         << crossProduct.Dot(jhat)<<"\n"
         << endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
