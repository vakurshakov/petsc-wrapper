#include "ksp.h"

namespace Petsc {

KSP::KSP(std::string_view name) {
  PetscCallThrow(KSPCreate(PETSC_COMM_WORLD, &data));
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

KSP KSP::FromOptions(std::string_view name) {
  KSP ksp(name);
  PetscCallThrow(KSPSetFromOptions(ksp.data));
  return ksp;
}

void KSP::SetOperators(Petsc::Mat& linearOp, Petsc::Mat& preconditionOp) {
  PetscCallThrow(KSPSetOperators(data, linearOp, preconditionOp));
}

void KSP::SetTolerances(Real relativeTol, Real absoluteTol, Real divergenceTol, Int itNumber) {
  PetscCallThrow(KSPSetTolerances(data, relativeTol, absoluteTol, divergenceTol, itNumber));
}

void KSP::SetInitialGuessNonzero(Bool flag) {
  PetscCallThrow(KSPSetInitialGuessNonzero(data, flag));
}

void KSP::SetFromOptions() {
  PetscCallThrow(KSPSetFromOptions(data));
}

void KSP::SetUp() {
  PetscCallThrow(KSPSetUp(data));
}

void KSP::Solve(const Petsc::Vec& rhs, Petsc::Vec& solution) {
  PetscCallThrow(KSPSolve(data, rhs, solution));
}

void KSP::GetSolution(Petsc::Vec& solution) const {
  PetscCallThrow(KSPGetSolution(data, solution));
}

void KSP::View(PetscViewer viewer) const {
  PetscCallThrow(KSPView(data, viewer));
}

Int KSP::GetIterationNumber() const {
  Int iterations;
  PetscCallThrow(KSPGetIterationNumber(data, &iterations));
  return iterations;
}

const char* KSP::GetConvergedReason() const {
  const char* reason;
  PetscCallThrow(KSPGetConvergedReasonString(data, &reason));
  return reason;
}

void KSP::Destroy() {
  PetscCallThrow(KSPDestroy(&data));
}

KSP::~KSP() noexcept(false) {
  Destroy();
}

}
