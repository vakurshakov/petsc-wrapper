#include "vec.h"

namespace Petsc {

Vec::Vec(Int localSize, Int globalSize, std::string_view name) {
  PetscCallThrow(VecCreate(PETSC_COMM_WORLD, &that));
  PetscCallThrow(VecSetSizes(that, localSize, globalSize));
  PetscCallThrow(VecSetType(that, VECSTANDARD)); // seq on one process and mpi on multiple
  PetscCallThrow(VecSetUp(that));                // explicitly setting up internal vec structures
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
  PetscCallThrow(VecDuplicate(that, vec));
  return vec;
}

Vec Vec::Copy() const {
  Vec vec;
  PetscCallThrow(VecDuplicate(that, vec));
  PetscCallThrow(VecCopy(that, vec));
  return vec;
}

Int Vec::Size() const {
  Int size;
  PetscCallThrow(VecGetSize(that, &size));
  return size;
}

Int Vec::LocalSize() const {
  Int localSize;
  PetscCallThrow(VecGetLocalSize(that, &localSize));
  return localSize;
}

std::pair<Int, Int> Vec::GetOwnershipRange() const {
  Int localStart, localEnd;
  PetscCallThrow(VecGetOwnershipRange(that, &localStart, &localEnd));
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
  PetscCallThrow(VecAXPY(that, a, x));
  return *this;
}

Petsc::Vec& Vec::AYPX(Scalar a, const Vec& x) {
  PetscCallThrow(VecAYPX(that, a, x));
  return *this;
}

Petsc::Vec& Vec::AXPBY(Scalar a, Scalar b, const Vec& x) {
  PetscCallThrow(VecAXPBY(that, a, b, x));
  return *this;
}

Petsc::Vec& Vec::AXPBYPCZ(Scalar a, Scalar b, Scalar c, const Vec& x, const Vec& y) {
  PetscCallThrow(VecAXPBYPCZ(that, a, b, c, x, y));
  return *this;
}

Petsc::Vec& Vec::Set(Scalar scalar) {
  PetscCallThrow(VecSet(that, scalar));
  return *this;
}

Petsc::Vec& Vec::Scale(Scalar scalar) {
  PetscCallThrow(VecScale(that, scalar));
  return *this;
}

Petsc::Vec& Vec::Shift(Scalar scalar) {
  PetscCallThrow(VecShift(that, scalar));
  return *this;
}

Petsc::Vec& Vec::Abs() {
  PetscCallThrow(VecAbs(that));
  return *this;
}

Petsc::Vec& Vec::Reciprocal() {
  PetscCallThrow(VecReciprocal(that));
  return *this;
}

Petsc::Vec& Vec::Normalize(Real* prevNorm2) {
  PetscCallThrow(VecNormalize(that, prevNorm2));
  return *this;
}

Scalar Vec::Dot(const Vec& y) const {
  Scalar result;
  PetscCallThrow(VecDot(that, y, &result));
  return result;
}

Scalar Vec::TDot(const Vec& y) const {
  Scalar result;
  PetscCallThrow(VecTDot(that, y, &result));
  return result;
}

Scalar Vec::Sum() const {
  Scalar result;
  PetscCallThrow(VecSum(that, &result));
  return result;
}

Real Vec::Norm(NormType type) const {
  Scalar result;
  PetscCallThrow(VecNorm(that, type, &result));
  return result;
}

std::pair<Int, Real> Vec::Max() const {
  Int index;
  Real result;
  PetscCallThrow(VecMax(that, &index, &result));
  return std::make_pair(index, result);
}

std::pair<Int, Real> Vec::Min() const {
  Int index;
  Real result;
  PetscCallThrow(VecMin(that, &index, &result));
  return std::make_pair(index, result);
}

void Vec::SetValues(Int size, const Int idx[], const Scalar values[], InsertMode mode) {
  PetscCallThrow(VecSetValues(that, size, idx, values, mode));
}

void Vec::AssemblyBegin() {
  PetscCallThrow(VecAssemblyBegin(that));
}

void Vec::AssemblyEnd() {
  PetscCallThrow(VecAssemblyEnd(that));
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

void Vec::View(PetscViewer viewer) const {
  PetscCallThrow(VecView(that, viewer));
}

void Vec::Destroy() {
  PetscCallThrow(VecDestroy(&that));
}

Vec::~Vec() noexcept(false) {
  Destroy();
}

}
