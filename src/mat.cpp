#include "mat.h"

namespace Petsc {

Mat::Mat(Int globalRows, Int globalCols, std::string_view name)
  : Mat(PETSC_DECIDE, PETSC_DECIDE, globalRows, globalCols, name) {}

Mat::Mat(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name) {
  PetscCallThrow(MatCreate(PETSC_COMM_WORLD, &data));
  PetscCallThrow(MatSetSizes(data, localRows, localCols, globalRows, globalCols));
  PetscCallThrow(MatSetType(data, MATAIJ)); // compressed sparse row storage by default
  PetscCallThrow(MatSetUp(data));           // explicitly setting up internal mat structures
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ Mat Mat::FromOptions(Int globalRows, Int globalCols, std::string_view name) {
  return FromOptions(PETSC_DECIDE, PETSC_DECIDE, globalRows, globalCols, name);
}

/* static */ Mat Mat::FromOptions(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name) {
  Mat mat(localRows, localCols, globalRows, globalCols, name);
  PetscCallThrow(MatSetFromOptions(mat.data));
  return mat;
}

void Mat::SetValues(
    PetscInt rowsSize, const PetscInt rowsIdx[],
    PetscInt colsSize, const PetscInt colsIdx[],
    const PetscScalar values[], InsertMode mode) {
  PetscCallThrow(MatSetValues(data, rowsSize, rowsIdx, colsSize, colsIdx, values,  mode));
}

void Mat::AssemblyBegin(MatAssemblyType mode) {
  PetscCallThrow(MatAssemblyBegin(data, mode));
}

void Mat::AssemblyEnd(MatAssemblyType mode) {
  PetscCallThrow(MatAssemblyEnd(data, mode));
}

void Mat::Destroy() {
  if (data) {
    PetscCallThrow(MatDestroy(&data));
  }
}

Mat::~Mat() noexcept(false) {
  Destroy();
}

}
