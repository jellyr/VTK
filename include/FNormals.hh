//
// Floating point representation of 3D normals.
//
//  use internal floating point array to represent data
//
#ifndef __vlFloatNormals_h
#define __vlFloatNormals_h

#include "Normals.hh"
#include "FArray.hh"

class vlFloatNormals : public vlNormals
{
public:
  vlFloatNormals() {};
  int Initialize(const int sz, const int ext=1000) 
    {return this->N.Initialize(3*sz,3*ext);};
  vlFloatNormals(const vlFloatNormals& fn) {this->N = fn.N;};
  vlFloatNormals(const int sz, const int ext=1000):N(3*sz,3*ext){};
  ~vlFloatNormals() {};
  char *GetClassName() {return "vlFloatNormals";};
  int NumNormals() {return (N.GetMaxId()+1)/3;};
  void Reset() {this->N.Reset();};
  vlFloatNormals &operator=(const vlFloatNormals& fn);
  void operator+=(const vlFloatNormals& fn);

  void InsertNormal(const int i, float *x) {
      this->N.InsertValue(3*i+2, x[2]);
      this->N[3*i] =  x[0];
      this->N[3*i+1] =  x[1];
  }
  int InsertNextNormal(float *x) {
    int id = this->N.GetMaxId() + 3;
    this->N.InsertValue(id,x[2]);
    this->N[id-2] = x[0];
    this->N[id-1] = x[1];
    return id/3;
  }

private:
  vlFloatArray N;
};

#endif
