#include "vec.h"

namespace Petsc {

Vec::Vec(Int localSize, Int globalSize, std::string_view name) {
  PetscCallThrow(VecCreate(PETSC_COMM_WORLD, &data));
  PetscCallThrow(VecSetSizes(data, localSize, globalSize));
  PetscCallThrow(VecSetType(data, VECSTANDARD)); // seq on one process and mpi on multiple
  PetscCallThrow(VecSetUp(data));                // explicitly setting up internal vec structures
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ Vec Vec::FromLocals(Int localSize, std::string_view name) {
  return FromOptions(localSize, PETSC_DETERMINE, name);
}

/* static */ Vec Vec::FromGlobals(Int globalSize, std::string_view name) {
  return FromOptions(PETSC_DECIDE, globalSize, name);
}

/* static */ Vec Vec::FromOptions(Int localSize, Int globalSize, std::string_view name) {
  Vec vec(localSize, globalSize, name);
  PetscCallThrow(VecSetFromOptions(vec));
  return vec;
}

Vec Vec::Duplicate() const {
  Vec vec;
  PetscCallThrow(VecDuplicate(data, vec));
  return vec;
}

Vec Vec::Copy() const {
  Vec vec;
  PetscCallThrow(VecDuplicate(data, vec));
  PetscCallThrow(VecCopy(data, vec));
  return vec;
}

Int Vec::Size() const {
  Int size;
  PetscCallThrow(VecGetSize(data, &size));
  return size;
}

Int Vec::LocalSize() const {
  Int localSize;
  PetscCallThrow(VecGetLocalSize(data, &localSize));
  return localSize;
}

std::pair<Int, Int> Vec::GetOwnershipRange() const {
  Int localStart, localEnd;
  PetscCallThrow(VecGetOwnershipRange(data, &localStart, &localEnd));
  return std::make_pair(localStart, localEnd);
}

void Vec::WAXPY(Vec& w, Scalar a, const Vec& x, const Vec& y) {
  PetscCallThrow(VecWAXPY(w, a, x, y));
}

void Vec::PointwiseMult(Vec& w, const Vec& x, const Vec& y) {
  PetscCallThrow(VecPointwiseMult(w, x, y));
}

void Vec::PointwiseDivide(Vec& w, const Vec& x, const Vec& y) {
  PetscCallThrow(VecPointwiseDivide(w, x, y));
}

Petsc::Vec& Vec::AXPY(Scalar a, const Vec& x) {
  PetscCallThrow(VecAXPY(data, a, x));
  return *this;
}

Petsc::Vec& Vec::AYPX(Scalar a, const Vec& x) {
  PetscCallThrow(VecAYPX(data, a, x));
  return *this;
}

Petsc::Vec& Vec::AXPBY(Scalar a, Scalar b, const Vec& x) {
  PetscCallThrow(VecAXPBY(data, a, b, x));
  return *this;
}

Petsc::Vec& Vec::AXPBYPCZ(Scalar a, Scalar b, Scalar c, const Vec& x, const Vec& y) {
  PetscCallThrow(VecAXPBYPCZ(data, a, b, c, x, y));
  return *this;
}

Petsc::Vec& Vec::Set(Scalar scalar) {
  PetscCallThrow(VecSet(data, scalar));
  return *this;
}

Petsc::Vec& Vec::Scale(Scalar scalar) {
  PetscCallThrow(VecScale(data, scalar));
  return *this;
}

Petsc::Vec& Vec::Shift(Scalar scalar) {
  PetscCallThrow(VecShift(data, scalar));
  return *this;
}

Petsc::Vec& Vec::Abs() {
  PetscCallThrow(VecAbs(data));
  return *this;
}

Petsc::Vec& Vec::Reciprocal() {
  PetscCallThrow(VecReciprocal(data));
  return *this;
}

Petsc::Vec& Vec::Normalize(Real* prevNorm2) {
  PetscCallThrow(VecNormalize(data, prevNorm2));
  return *this;
}

Scalar Vec::Dot(const Vec& y) const {
  Scalar result;
  PetscCallThrow(VecDot(data, y, &result));
  return result;
}

Scalar Vec::TDot(const Vec& y) const {
  Scalar result;
  PetscCallThrow(VecTDot(data, y, &result));
  return result;
}

Scalar Vec::Sum() const {
  Scalar result;
  PetscCallThrow(VecSum(data, &result));
  return result;
}

Real Vec::Norm(NormType type) const {
  Scalar result;
  PetscCallThrow(VecNorm(data, type, &result));
  return result;
}

std::pair<Int, Real> Vec::Max() const {
  Int index;
  Real result;
  PetscCallThrow(VecMax(data, &index, &result));
  return std::make_pair(index, result);
}

std::pair<Int, Real> Vec::Min() const {
  Int index;
  Real result;
  PetscCallThrow(VecMin(data, &index, &result));
  return std::make_pair(index, result);
}

void Vec::SetValues(Int size, const Int idx[], const Scalar values[], InsertMode mode) {
  PetscCallThrow(VecSetValues(data, size, idx, values, mode));
}

void Vec::AssemblyBegin() {
  PetscCallThrow(VecAssemblyBegin(data));
}

void Vec::AssemblyEnd() {
  PetscCallThrow(VecAssemblyEnd(data));
}

Vec::BorrowedArray Vec::GetArray(GetArrayType type) {
  if (!(type == Default || type == Write)) {
    PetscCallThrow(PETSC_ERR_ARG_WRONG);
  }
  return BorrowedArray(*this, type);
}

Vec::ConstBorrowedArray Vec::GetArray() const {
  return ConstBorrowedArray(*this, Read);
}

Vec::ConstBorrowedArray Vec::GetArrayRead() const {
  return ConstBorrowedArray(*this, Read);
}

void Vec::Destroy() {
  if (data) {
    PetscCallThrow(VecDestroy(&data));
    data = nullptr;
  }
}

Vec::~Vec() noexcept(false) {
  Destroy();
}

/*
Vec::BorrowedArray::iterator Vec::BorrowedArray::begin() {
  return Vec::BorrowedArray::iterator(*this, 0);
}

Vec::BorrowedArray::iterator Vec::BorrowedArray::end() {
  Int localSize = vec.LocalSize();
  return Vec::BorrowedArray::iterator(*this, localSize);
}


Vec::BorrowedArray::iterator::iterator(Vec::BorrowedArray& array, Int current)
  : array(array), current(current) {}

Scalar& Vec::BorrowedArray::iterator::operator*() {
  return array[current];
}

Scalar* Vec::BorrowedArray::iterator::operator->() {
  return &array[current];
}

Vec::BorrowedArray::iterator& Vec::BorrowedArray::iterator::operator++() {
  current++;
  return *this;
}

bool Vec::BorrowedArray::iterator::operator!=(const iterator& other) const {
  return current != other.current;
}

Vec::BorrowedArray::iterator& Vec::BorrowedArray::iterator::operator+=(Int difference) {
  current += difference;
  return *this;
}

Int Vec::BorrowedArray::iterator::operator-(const iterator& other) const {
  return current - other.current;
}
*/

}
