#ifndef SRC_KSP_H
#define SRC_KSP_H

#include <string>

#include <petscksp.h>

#include "context.h"
#include "macros.h"
#include "vec.h"
#include "mat.h"

namespace Petsc {

class KSP {
 public:
  KSP(std::string_view name = {});
  static KSP FromOptions(std::string_view name = {});

  void SetOperators(Petsc::Mat& linearOp, Petsc::Mat& preconditionOp);
  void SetTolerances(Real relativeTol, Real absoluteTol, Real divergenceTol, Int itNumber);
  void SetFromOptions();
  void SetUp();

  void Solve(const Petsc::Vec& rhs, Petsc::Vec& solution);

  void View(PetscViewer viewer) const;
  Int GetIterationNumber() const;

  void Destroy();
  ~KSP() noexcept(false);

  /// @brief Conversion to PETSc ksp
  operator _p_KSP*() const { return data; }
  operator _p_KSP**() { return &data; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(data); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&data); }

 private:
  _p_KSP* data = nullptr;
};

}

#endif // SRC_KSP_H
