#ifndef SRC_KSP_H
#define SRC_KSP_H

#include <string_view>

#include <petscksp.h>

#include "exception.h"
#include "utils.h"
#include "vec.h"
#include "mat.h"

namespace Petsc {

class KSP {
 public:
  KSP(std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(KSP);

  static KSP FromOptions(std::string_view name = {});

  void SetOperators(Mat& linearOp, Mat& preconditionOp);
  void SetTolerances(Real relativeTol, Real absoluteTol, Real divergenceTol, Int itNumber);
  void SetInitialGuessNonzero(Bool flag);
  void SetFromOptions();
  void SetUp();

  void Solve(const Vec& rhs, Vec& solution);
  void GetSolution(Vec& solution) const;

  void View(PetscViewer viewer) const;
  Int GetIterationNumber() const;
  const char* GetConvergedReason() const;

  void Destroy();
  ~KSP() noexcept(false);

  /// @brief Conversion to PETSc ksp
  operator _p_KSP*() const { return that; }
  operator _p_KSP**() { return &that; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(that); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&that); }

 private:
  _p_KSP* that = nullptr;
};

}

#endif // SRC_KSP_H
