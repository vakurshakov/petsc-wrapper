#ifndef SRC_VEC_H
#define SRC_VEC_H

#include <string_view>

#include <petscvec.h>

#include "context.h"
#include "exception.h"

namespace Petsc {

/// @todo Create indexes<T> to reduce the need in array size.
/// @note Is it important that views should be restored before next gather/scatter?

class Vec {
  template<bool isConst> class BasicBorrowedArray;
  using BorrowedArray = BasicBorrowedArray<false>;
  using ConstBorrowedArray = BasicBorrowedArray<true>;

 public:
  Vec() = default;
  Vec(Int localSize, Int globalSize, std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(Vec);

  static Vec FromLocals(Int localSize, std::string_view name = {});
  static Vec FromGlobals(Int globalSize, std::string_view name = {});
  static Vec FromOptions(Int localSize, Int globalSize, std::string_view name = {});

  Vec Duplicate() const;
  Vec Copy() const;

  Int Size() const;
  Int LocalSize() const;
  std::pair<Int, Int> GetOwnershipRange() const;

  static void WAXPY(Vec& w, Scalar a, const Vec& x, const Vec& y);
  static void PointwiseMult(Vec& w, const Vec& x, const Vec& y);
  static void PointwiseDivide(Vec& w, const Vec& x, const Vec& y);

  Vec& AXPY(Scalar a, const Vec& x);
  Vec& AYPX(Scalar a, const Vec& x);
  Vec& AXPBY(Scalar a, Scalar b, const Vec& x);
  Vec& AXPBYPCZ(Scalar a, Scalar b, Scalar c, const Vec& x, const Vec& y);
  Vec& Set(Scalar scalar);
  Vec& Scale(Scalar scalar);
  Vec& Shift(Scalar scalar);
  Vec& Abs();
  Vec& Reciprocal();
  Vec& Normalize(Real* prevNorm2 = nullptr);

  Scalar Dot(const Vec& y) const;
  Scalar TDot(const Vec& y) const;
  Scalar Sum() const;
  Real Norm(NormType type) const;
  std::pair<Int, Real> Max() const;
  std::pair<Int, Real> Min() const;

  void SetValues(Int size, const Int idx[], const Scalar values[], InsertMode mode);
  void AssemblyBegin();
  void AssemblyEnd();

  BorrowedArray GetArray(GetArrayType type = Default);
  ConstBorrowedArray GetArray() const;
  ConstBorrowedArray GetArrayRead() const;

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


template<bool isConst>
class Vec::BasicBorrowedArray {
  using VecRef = std::conditional_t<isConst, const Vec&, Vec&>;
  using ArrayPointer = std::conditional_t<isConst, const Scalar*, Scalar*>;
  template<bool iterConst> class BasicIterator;

 public:
  BasicBorrowedArray(VecRef vec, GetArrayType type);
  PETSC_NO_COPY_POLICY(BasicBorrowedArray);

  void Restore();
  ~BasicBorrowedArray() noexcept(false);

  operator const Scalar*() const { return array; }
  operator ArrayPointer() { return array; }

  using Iterator = BasicIterator<isConst>;
  Iterator begin();
  Iterator end();

  using ConstIterator = BasicIterator<true>;
  ConstIterator begin() const;
  ConstIterator end() const;

 private:
  VecRef vec;
  GetArrayType type;

  ArrayPointer array;
};


template<bool arrConst>
template<bool iterConst>
class Vec::BasicBorrowedArray<arrConst>::BasicIterator {
 public:
  using Reference = std::conditional_t<arrConst || iterConst, const Scalar&, Scalar&>;
  using Pointer = std::conditional_t<arrConst || iterConst, const Scalar*, Scalar*>;

  BasicIterator(Vec::BasicBorrowedArray<arrConst>& array, Int current);
  ~BasicIterator() = default;

  Int index() const;
  Reference value();

  // Input iterator requirements
  Reference operator*();
  Pointer operator->();
  BasicIterator& operator++();
  bool operator!=(const BasicIterator& other) const;

  // Random access iterator requirements for OpenMP
  BasicIterator& operator+=(Int difference);
  Int operator-(const BasicIterator& other) const;

 private:
  Vec::BasicBorrowedArray<arrConst>& array;
  Int current;
};

}

#include "vec.inl"

#endif // SRC_VEC_H
