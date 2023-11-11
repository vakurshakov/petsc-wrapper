#ifndef SRC_VEC_H
#define SRC_VEC_H

#include <string_view>

#include <petscvec.h>

#include "context.h"
#include "exception.h"

namespace Petsc {

/// @todo Create indexes<T> to reduce the need in array size.
/// @todo Create [const_]view<T> for objects that should be restored after usage.
/// @todo Implement Vec::iterator::value(), Vec::iterator::index() methods.
/// @note Is it important that views should be restored before next gather/scatter?

class Vec {
 public:
  Vec() = default;
  Vec(Int localSize, Int globalSize, std::string_view name = {});

  static Vec FromLocals(Int localSize, std::string_view name = {});
  static Vec FromGlobals(Int globalSize, std::string_view name = {});
  static Vec FromOptions(Int localSize, Int globalSize, std::string_view name = {});
  static Vec Duplicate(const Vec& other);

  Int Size() const;
  Int LocalSize() const;
  std::pair<Int, Int> GetOwnershipRange() const;

  static void WAXPY(Vec& w, Scalar a, const Vec& x, const Vec& y);
  static void PointwiseMult(Vec& w, const Vec& x, const Vec& y);
  static void PointwiseDivide(Vec& w, const Vec& x, const Vec& y);

  Petsc::Vec& AXPY(Scalar a, const Vec& x);
  Petsc::Vec& AYPX(Scalar a, const Vec& x);
  Petsc::Vec& AXPBY(Scalar a, Scalar b, const Vec& x);
  Petsc::Vec& AXPBYPCZ(Scalar a, Scalar b, Scalar c, const Vec& x, const Vec& y);
  Petsc::Vec& Set(Scalar scalar);
  Petsc::Vec& Scale(Scalar scalar);
  Petsc::Vec& Shift(Scalar scalar);
  Petsc::Vec& Abs();
  Petsc::Vec& Reciprocal();
  Petsc::Vec& Normalize(Real* prevNorm2 = nullptr);

  Scalar Dot(const Vec& y) const;
  Scalar TDot(const Vec& y) const;
  Scalar Sum() const;
  Real Norm(NormType type) const;
  std::pair<Int, Real> Max() const;
  std::pair<Int, Real> Min() const;

  void SetValues(Int size, const Int idx[], const Scalar values[], InsertMode mode);
  void AssemblyBegin();
  void AssemblyEnd();

  class iterator;
  iterator begin();
  iterator end();

  Scalar* GetArray();
  const Scalar* GetArrayRead() const;
  void RestoreArray(Scalar array[]);
  void RestoreArray(const Scalar array[]) const;

  void Destroy();
  ~Vec() noexcept(false);

  /// @brief Conversion to PETSc vector
  operator _p_Vec*() const { return data; }
  operator _p_Vec**() { return &data; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 private:
  _p_Vec* data = nullptr;
};


class Vec::iterator {
 public:
  iterator(Petsc::Vec& vec, Int current);
  ~iterator() noexcept(false);

  // Input iterator requirements
  Scalar& operator*();
  Scalar* operator->();
  iterator& operator++();
  bool operator!=(const iterator& other) const;

  // Random access iterator requirements for OpenMP
  iterator& operator+=(Int difference);
  Int operator-(const iterator& other) const;

 private:
  Petsc::Vec& vec;
  Scalar* array;
  Int current;
};

}

#endif // SRC_VEC_H
