#include "dm.h"

namespace Petsc {

DM::DM(std::string_view name) {
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

void DM::SetFromOptions() {
  PetscCallThrow(DMSetFromOptions(data));
}

void DM::SetUp() {
  PetscCallThrow(DMSetUp(data));
}

void DM::View(PetscViewer viewer) const {
  PetscCallThrow(DMView(data, viewer));
}

Petsc::Vec DM::CreateGlobalVector() const {
  Vec vec;
  PetscCallThrow(DMCreateGlobalVector(data, vec));
  return vec;
}

Petsc::Vec DM::CreateLocalVector() const {
  Vec vec;
  PetscCallThrow(DMCreateLocalVector(data, vec));
  return vec;
}

Petsc::Mat DM::CreateMatrix() const {
  Mat mat;
  PetscCallThrow(DMCreateMatrix(data, mat));
  return mat;
}

DM::BorrowedVec DM::GetVector(VectorType type) {
  return BorrowedVec(*this, type);
}

void DM::GlobalToLocal(const Vec& global, InsertMode mode, Vec& local) const {
  PetscCallThrow(DMGlobalToLocal(data, global, mode, local));
}

void DM::GlobalToLocalBegin(const Vec& global, InsertMode mode, Vec& local) const {
  PetscCallThrow(DMGlobalToLocalBegin(data, global, mode, local));
}

void DM::GlobalToLocalEnd(const Vec& global, InsertMode mode, Vec& local) const {
  PetscCallThrow(DMGlobalToLocalEnd(data, global, mode, local));
}

void DM::LocalToGlobal(const Vec& local, InsertMode mode, Vec& global) const {
  PetscCallThrow(DMLocalToGlobal(data, local, mode, global));
}

void DM::LocalToGlobalBegin(const Vec& local, InsertMode mode, Vec& global) const {
  PetscCallThrow(DMLocalToGlobalBegin(data, local, mode, global));
}

void DM::LocalToGlobalEnd(const Vec& local, InsertMode mode, Vec& global) const {
  PetscCallThrow(DMLocalToGlobalEnd(data, local, mode, global));
}

void DM::LocalToLocalBegin(const Vec& global, InsertMode mode, Vec& local) const {
  PetscCallThrow(DMLocalToLocalBegin(data, global, mode, local));
}

void DM::LocalToLocalEnd(const Vec& global, InsertMode mode, Vec& local) const {
  PetscCallThrow(DMLocalToLocalEnd(data, global, mode, local));
}

Int DM::GetDimension() const {
  Int dim;
  PetscCallThrow(DMGetDimension(data, &dim));
  return dim;
}

void DM::SetDimension(Int dim) {
  PetscCallThrow(DMSetDimension(data, dim));
}

std::pair<Int, Int> DM::GetDimPoints(Int dim) const {
  Int start, end;
  PetscCallThrow(DMGetDimPoints(data, dim, &start, &end));
  return std::make_pair(start, end);
}

std::pair<Int, const MPIInt*> DM::GetNeighbors() const {
  Int nranks;
  const MPIInt* ranks;
  PetscCallThrow(DMGetNeighbors(data, &nranks, &ranks));
  return std::make_pair(nranks, ranks);
}

void DM::Destroy() {
  PetscCallThrow(DMDestroy(&data));
}

DM::~DM() noexcept(false) {
  Destroy();
}


DM::BorrowedVec::BorrowedVec(DM& dm, DM::VectorType type) : dm(dm), type(type) {
  switch (type) {
    case Local: PetscCallThrow(DMGetLocalVector(dm, vec)); return;
    case Global: PetscCallThrow(DMGetGlobalVector(dm, vec)); return;
    default: PetscCallThrow(PETSC_ERR_ARG_WRONG);
  }
}

DM::BorrowedVec::~BorrowedVec() {
  switch (type) {
    case Local: PetscCallThrow(DMRestoreLocalVector(dm, vec)); return;
    case Global: PetscCallThrow(DMRestoreGlobalVector(dm, vec)); return;
    default: PetscCallThrow(PETSC_ERR_ARG_WRONG);
  }
}

}
