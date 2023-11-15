#ifndef SRC_DM_H
#define SRC_DM_H

#include <string_view>

#include <petscdm.h>

#include "context.h"
#include "exception.h"
#include "vec.h"
#include "mat.h"

namespace Petsc {

class DM {
 public:
  using BoundaryType = DMBoundaryType;

  DM(std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(DM);

  void SetFromOptions();
  void SetUp();
  void View(PetscViewer viewer) const;

  Vec CreateGlobalVector() const;
  Vec CreateLocalVector() const;
  Mat CreateMatrix() const;

  enum VectorType {
    LOCAL = 0,
    GLOBAL,
  };
  class BorrowedVec;
  BorrowedVec BorrowVec(VectorType type);

  void GlobalToLocal(const Vec& global, InsertMode mode, Vec& local) const;
  void GlobalToLocalBegin(const Vec& global, InsertMode mode, Vec& local) const;
  void GlobalToLocalEnd(const Vec& global, InsertMode mode, Vec& local) const;
  void LocalToGlobal(const Vec& local, InsertMode mode, Vec& global) const;
  void LocalToGlobalBegin(const Vec& local, InsertMode mode, Vec& global) const;
  void LocalToGlobalEnd(const Vec& local, InsertMode mode, Vec& global) const;
  void LocalToLocalBegin(const Vec& global, InsertMode mode, Vec& local) const;
  void LocalToLocalEnd(const Vec& global, InsertMode mode, Vec& local) const;

  // Topology support
  Int GetDimension() const;
  void SetDimension(Int dim);
  std::pair<Int, Int> GetDimPoints(Int dim) const;
  std::pair<Int, const MPIInt*> GetNeighbors() const;

  void Destroy();
  virtual ~DM() noexcept(false);

  /// @brief Conversion to PETSc DM
  operator _p_DM*() const { return data; }
  operator _p_DM**() { return &data; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 protected:
  _p_DM* data = nullptr;
};

class DM::BorrowedVec {
 public:
  BorrowedVec(DM& dm, DM::VectorType type);
  ~BorrowedVec() noexcept(false);
  PETSC_NO_COPY_POLICY(BorrowedVec);

  operator const Vec&() const { return vec; }
  operator Vec&() { return vec; }

 private:
  DM& dm;
  DM::VectorType type;

  Vec vec;
};

}

#endif // SRC_DM_H
