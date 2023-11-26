#include "ksp.h"

namespace Petsc {

KSP::KSP(std::string_view name) {
  PetscCallThrow(KSPCreate(PETSC_COMM_WORLD, &that));
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

KSP KSP::FromOptions(std::string_view name) {
  KSP ksp(name);
  PetscCallThrow(KSPSetFromOptions(ksp));
  return ksp;
}

void KSP::SetOperators(Petsc::Mat& linearOp, Petsc::Mat& preconditionOp) {
  PetscCallThrow(KSPSetOperators(that, linearOp, preconditionOp));
}

void KSP::SetTolerances(Real relativeTol, Real absoluteTol, Real divergenceTol, Int itNumber) {
  PetscCallThrow(KSPSetTolerances(that, relativeTol, absoluteTol, divergenceTol, itNumber));
}

void KSP::SetInitialGuessNonzero(Bool flag) {
  PetscCallThrow(KSPSetInitialGuessNonzero(that, flag));
}

void KSP::SetFromOptions() {
  PetscCallThrow(KSPSetFromOptions(that));
}

void KSP::SetUp() {
  PetscCallThrow(KSPSetUp(that));
}

void KSP::Solve(const Petsc::Vec& rhs, Petsc::Vec& solution) {
  PetscCallThrow(KSPSolve(that, rhs, solution));
}

void KSP::GetSolution(Petsc::Vec& solution) const {
  PetscCallThrow(KSPGetSolution(that, solution));
}

void KSP::View(PetscViewer viewer) const {
  PetscCallThrow(KSPView(that, viewer));
}

Int KSP::GetIterationNumber() const {
  Int iterations;
  PetscCallThrow(KSPGetIterationNumber(that, &iterations));
  return iterations;
}

const char* KSP::GetConvergedReason() const {
  const char* reason;
  PetscCallThrow(KSPGetConvergedReasonString(that, &reason));
  return reason;
}

void KSP::Destroy() {
  PetscCallThrow(KSPDestroy(&that));
}

KSP::~KSP() noexcept(false) {
  Destroy();
}

}
