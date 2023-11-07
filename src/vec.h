#ifndef SRC_VEC_H
#define SRC_VEC_H

#include <string>
#include <type_traits>

#include <petscvec.h>

#include "context.h"
#include "macros.h"


#include <iostream>

namespace Petsc {

class Vec {
 public:
  Vec() = default;
  Vec(Int globalSize, std::string_view name);
  Vec(Int localSize, Int globalSize, std::string_view name);

  static Vec FromOptions(Int globalSize, std::string_view name);
  static Vec FromOptions(Int localSize, Int globalSize, std::string_view name);
  static Vec Duplicate(const Vec &other);

  Int Size() const;
  Int LocalSize() const;
  std::pair<Int, Int> GetOwnershipRange() const;

  void SetValues(Int size, const Int idx[], const Scalar values[], InsertMode mode);
  void AssemblyBegin();
  void AssemblyEnd();

  class iterator;
  iterator begin();
  iterator end();

  /// @todo create (template?) array_view
  Scalar* GetArray();
  const Scalar* GetArrayRead();
  void RestoreArray(Scalar* array);
  void RestoreArray(const Scalar* array);

  void Destroy();
  ~Vec() noexcept(false);

  /// @brief Conversion to PETSc vector
  operator _p_Vec*() { return data; }
  operator _p_Vec**() { return &data; }
  operator _p_PetscObject*() { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 private:
  _p_Vec* data = nullptr;
};


class Vec::iterator {
 public:
  iterator(Vec& vec, Int current);
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
  Vec& vec;
  Scalar* array;
  Int current;
};

}

#endif // SRC_VEC_H
