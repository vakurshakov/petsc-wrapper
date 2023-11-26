#include "mat.h"

namespace Petsc {

Mat::Mat(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name) {
  PetscCallThrow(MatCreate(PETSC_COMM_WORLD, &that));
  PetscCallThrow(MatSetSizes(that, localRows, localCols, globalRows, globalCols));
  PetscCallThrow(MatSetType(that, MATAIJ)); // compressed sparse row storage by default
  PetscCallThrow(MatSetUp(that));           // explicitly setting up internal mat structures
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ Mat Mat::FromLocals(Int localRows, Int localCols, std::string_view name) {
  return FromOptions(localRows, localCols, PETSC_DETERMINE, PETSC_DETERMINE, name);
}

/* static */ Mat Mat::FromGlobals(Int globalRows, Int globalCols, std::string_view name) {
  return FromOptions(PETSC_DECIDE, PETSC_DECIDE, globalRows, globalCols, name);
}

/* static */ Mat Mat::FromOptions(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name) {
  Mat mat(localRows, localCols, globalRows, globalCols, name);
  PetscCallThrow(MatSetFromOptions(mat));
  return mat;
}

std::pair<Int, Int> Mat::GetSize() const {
  Int globalRows, globalCols;
  PetscCallThrow(MatGetSize(that, &globalRows, &globalCols));
  return std::make_pair(globalRows, globalCols);
}

std::pair<Int, Int> Mat::GetLocalSize() const {
  Int localRows, localCols;
  PetscCallThrow(MatGetLocalSize(that, &localRows, &localCols));
  return std::make_pair(localRows, localCols);
}

std::pair<Int, Int> Mat::GetOwnershipRange() const {
  Int localStart, localEnd;
  PetscCallThrow(MatGetOwnershipRange(that, &localStart, &localEnd));
  return std::make_pair(localStart, localEnd);
}

std::pair<Int, Int> Mat::GetOwnershipRangeColumn() const {
  Int localStart, localEnd;
  PetscCallThrow(MatGetOwnershipRangeColumn(that, &localStart, &localEnd));
  return std::make_pair(localStart, localEnd);
}

const Int* Mat::GetOwnershipRanges() const {
  const Int* localRanges;
  PetscCallThrow(MatGetOwnershipRanges(that, &localRanges));
  return localRanges;
}

const Int* Mat::GetOwnershipRangesColumn() const {
  const Int* localRanges;
  PetscCallThrow(MatGetOwnershipRangesColumn(that, &localRanges));
  return localRanges;
}

void Mat::ZeroEntries() {
  PetscCallThrow(MatZeroEntries(that));
}

void Mat::SetValues(PetscInt rowsSize, const PetscInt rowsIdx[], PetscInt colsSize, const PetscInt colsIdx[], const PetscScalar values[], InsertMode mode) {
  PetscCallThrow(MatSetValues(that, rowsSize, rowsIdx, colsSize, colsIdx, values,  mode));
}

void Mat::AssemblyBegin(AssemblyType mode) {
  PetscCallThrow(MatAssemblyBegin(that, mode));
}

void Mat::AssemblyEnd(AssemblyType mode) {
  PetscCallThrow(MatAssemblyEnd(that, mode));
}

void Mat::Mult(const Petsc::Vec& in, Petsc::Vec& out) const {
  PetscCallThrow(MatMult(that, in, out));
}

void Mat::View(PetscViewer viewer) const {
  PetscCallThrow(MatView(that, viewer));
}

void Mat::Destroy() {
  PetscCallThrow(MatDestroy(&that));
}

Mat::~Mat() noexcept(false) {
  Destroy();
}

}
