#include "vec.h"

namespace Petsc {

Vec::Vec(Int globalSize, std::string_view name)
    : Vec(PETSC_DECIDE, globalSize, name) {}

Vec::Vec(Int localSize, Int globalSize, std::string_view name) {
  PetscCallThrow(VecCreate(PETSC_COMM_WORLD, &data));
  PetscCallThrow(VecSetSizes(data, localSize, globalSize));
  PetscCallThrow(VecSetType(data, VECSTANDARD)); // seq on one process and mpi on multiple
  PetscCallThrow(VecSetUp(data));                // explicitly setting up internal vec structures
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ Vec Vec::FromOptions(Int globalSize, std::string_view name) {
  return FromOptions(PETSC_DECIDE, globalSize, name);
}

/* static */ Vec Vec::FromOptions(Int localSize, Int globalSize, std::string_view name) {
  Vec vec(localSize, globalSize, name);
  PetscCallThrow(VecSetFromOptions(vec.data));
  return vec;
}

/* static */ Vec Vec::Duplicate(const Vec& other) {
  Vec vec;
  PetscCallThrow(VecDuplicate(other.data, &vec.data));
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

Petsc::Vec& Vec::Normalize(Real* prevNorm2 = nullptr) {
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

Vec::iterator Vec::begin() {
  return iterator(*this, 0);
}

Vec::iterator Vec::end() {
  Int localSize = LocalSize();
  return iterator(*this, localSize);
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

Scalar* Vec::GetArray() {
  Scalar* array = nullptr;
  PetscCallThrow(VecGetArray(data, &array));
  return array;
}

const Scalar* Vec::GetArrayRead() const {
  const Scalar* array = nullptr;
  PetscCallThrow(VecGetArrayRead(data, &array));
  return array;
}

void Vec::RestoreArray(Scalar array[]) {
  PetscCallThrow(VecRestoreArray(data, &array));
}

void Vec::RestoreArray(const Scalar array[]) const {
  PetscCallThrow(VecRestoreArrayRead(data, &array));
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


Vec::iterator::iterator(Vec& vec, Int current)
    : vec(vec), current(current) {
  PetscCallThrow(VecGetArray(vec, &array));
}

Vec::iterator::~iterator() noexcept(false) {
  PetscCallThrow(VecRestoreArray(vec, &array));
}

Scalar& Vec::iterator::operator*() {
  return array[current];
}

Scalar* Vec::iterator::operator->() {
  return &array[current];
}

Vec::iterator& Vec::iterator::operator++() {
  current++;
  return *this;
}

bool Vec::iterator::operator!=(const iterator& other) const {
  return current != other.current;
}

Vec::iterator& Vec::iterator::operator+=(Int difference) {
  current += difference;
  return *this;
}

Int Vec::iterator::operator-(const iterator& other) const {
  return current - other.current;
}

}
