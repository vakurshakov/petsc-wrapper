#include "vec.h"

namespace Petsc {

Vec::Vec(Int globalSize, std::string_view name)
    : Vec(PETSC_DECIDE, globalSize, name) {}

Vec::Vec(Int localSize, Int globalSize, std::string_view name) {
  PetscCallThrow(VecCreate(PETSC_COMM_WORLD, &data));
  PetscCallThrow(VecSetSizes(data, localSize, globalSize));
  PetscCallThrow(VecSetType(data, VECSTANDARD)); // seq on one process and mpi on multiple
  PetscCallThrow(VecSetUp(data));
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

/* static */ Vec Vec::Duplicate(const Vec &other) {
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
  Int rstart, rend;
  PetscCallThrow(VecGetOwnershipRange(data, &rstart, &rend));
  return std::make_pair(rstart, rend);
}

Vec::iterator Vec::begin() {
  auto [start, _] = GetOwnershipRange();
  return iterator(*this, start);
}

Vec::iterator Vec::end() {
  auto [_, end] = GetOwnershipRange();
  return iterator(*this, end);
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

const Scalar* Vec::GetArrayRead() {
  const Scalar* array = nullptr;
  PetscCallThrow(VecGetArrayRead(data, &array));
  return array;
}

void Vec::RestoreArray(Scalar* array) {
  PetscCallThrow(VecRestoreArray(data, &array));
}

void Vec::RestoreArray(const Scalar* array) {
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
