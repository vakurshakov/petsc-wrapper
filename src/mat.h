#ifndef SRC_MAT_H
#define SRC_MAT_H

#include <string>

#include <petscmat.h>

#include "context.h"
#include "macros.h"

namespace Petsc {

class Mat {
 public:
  Mat() = default;

  Mat(Int globalRows, Int globalCols, std::string_view name);
  Mat(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name);

  static Mat FromOptions(Int globalRows, Int globalCols, std::string_view name);
  static Mat FromOptions(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name);

  std::pair<Int, Int> GetSize() const;
  std::pair<Int, Int> GetLocalSize() const;
  std::pair<Int, Int> GetOwnershipRange() const;
  std::pair<Int, Int> GetOwnershipRangeColumn() const;
  const Int* GetOwnershipRanges() const;
  const Int* GetOwnershipRangesColumn() const;

  void ZeroEntries();

  void SetValues(
    PetscInt rowsSize, const PetscInt rowsIdx[],
    PetscInt colsSize, const PetscInt colsIdx[],
    const PetscScalar values[], InsertMode mode);

  void AssemblyBegin(MatAssemblyType mode);
  void AssemblyEnd(MatAssemblyType mode);

  void Destroy();
  ~Mat() noexcept(false);

  /// @brief Conversion to PETSc matrix
  operator _p_Mat*() { return data; }
  operator _p_Mat**() { return &data; }
  operator _p_PetscObject*() { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 private:
  _p_Mat* data = nullptr;
};

}

#endif // SRC_MAT_H
