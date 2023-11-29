#ifndef SRC_IS_H
#define SRC_IS_H

#include <string_view>

#include <petscis.h>

#include "exception.h"
#include "utils.h"

namespace Petsc {

/// @todo Generalize borrowing concept on different types, @see GetIndices.

class IS {
 public:
  IS() = default;
  IS(MPI_Comm comm, std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(IS);

  static IS Create(MPI_Comm comm, std::string_view name = {});
  static IS CreateGeneral(MPI_Comm comm, Int size, const Int idx[], CopyMode mode);
  static IS CreateStride(MPI_Comm comm, Int size, Int start, Int step);
  static IS CreateBlock(MPI_Comm comm, Int blockSize, Int blocksNumber, const Int idx[], CopyMode mode);

  IS Duplicate() const;
  IS Copy() const;

  void SetType(ISType type);
  void GeneralSetIndices(Int size, const Int idx[], CopyMode mode);
  void StrideSetStride(Int size, Int start, Int step);
  void BlockSetIndices(Int blockSize, Int blocksNumber, const Int idx[], CopyMode mode);

  const Int* GetIndices();
  void RestoreIndices(const Int* idx[]);

  Int GetLocalSize() const;
  Int GetSize() const;

  void View(PetscViewer viewer) const;

  void Destroy();
  ~IS() noexcept(false);

  /// @brief Conversion to PETSc index set
  operator _p_IS*() const { return that; }
  operator _p_IS**() { return &that; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(that); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&that); }

 private:
  _p_IS* that = nullptr;
};

}

#endif // SRC_IS_H
