#ifndef SRC_MAT_H
#define SRC_MAT_H

#include <string_view>

#include <petscmat.h>

#include "exception.h"
#include "utils.h"
#include "vec.h"

namespace Petsc {

class Mat {
 public:
  using AssemblyType = MatAssemblyType;

  Mat() = default;
  Mat(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(Mat);

  static Mat FromLocals(Int localRows, Int localCols, std::string_view name = {});
  static Mat FromGlobals(Int globalRows, Int globalCols, std::string_view name = {});
  static Mat FromOptions(Int localRows, Int localCols, Int globalRows, Int globalCols, std::string_view name = {});

  std::pair<Int, Int> GetSize() const;
  std::pair<Int, Int> GetLocalSize() const;
  std::pair<Int, Int> GetOwnershipRange() const;
  std::pair<Int, Int> GetOwnershipRangeColumn() const;
  const Int* GetOwnershipRanges() const;
  const Int* GetOwnershipRangesColumn() const;

  void ZeroEntries();

  void SetValues(Int rowsSize, const Int rowsIdx[], Int colsSize, const Int colsIdx[], const Scalar values[], InsertMode mode);
  void AssemblyBegin(AssemblyType mode);
  void AssemblyEnd(AssemblyType mode);

  void Mult(const Vec& in, Vec& out) const;

  void View(PetscViewer viewer) const;

  void Destroy();
  ~Mat() noexcept(false);

  /// @brief Conversion to PETSc matrix
  operator _p_Mat*() const { return data; }
  operator _p_Mat**() { return &data; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 private:
  _p_Mat* data = nullptr;
};

}

#endif // SRC_MAT_H
