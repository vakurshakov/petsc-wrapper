#include "is.h"

namespace Petsc {

IS::IS(MPI_Comm comm, std::string_view name) {
  PetscCallThrow(ISCreate(comm, &that));
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ IS IS::Create(MPI_Comm comm, std::string_view name) {
  IS is(comm, name);
  return is;
}

/* static */ IS IS::CreateGeneral(MPI_Comm comm, Int size, const Int idx[], CopyMode mode) {
  IS is;
  PetscCallThrow(ISCreateGeneral(comm, size, idx, mode, is));
  return is;
}

/* static */ IS IS::CreateStride(MPI_Comm comm, Int size, Int start, Int step) {
  IS is;
  PetscCallThrow(ISCreateStride(comm, size, start, step, is));
  return is;
}

/* static */ IS IS::CreateBlock(MPI_Comm comm, Int blockSize, Int blocksNumber, const Int idx[], CopyMode mode) {
  IS is;
  PetscCallThrow(ISCreateBlock(comm, blockSize, blocksNumber, idx, mode, is));
  return is;
}

IS IS::Duplicate() const {
  IS is;
  PetscCallThrow(ISDuplicate(that, is));
  return is;
}

IS IS::Copy() const {
  IS is;
  PetscCallThrow(ISDuplicate(that, is));
  PetscCallThrow(ISCopy(that, is));
  return is;
}

void IS::SetType(ISType type) {
  PetscCallThrow(ISSetType(that, type));
}

void IS::GeneralSetIndices(Int size, const Int idx[], CopyMode mode) {
  PetscCallThrow(ISGeneralSetIndices(that, size, idx, mode));
}

void IS::StrideSetStride(Int size, Int start, Int step) {
  PetscCallThrow(ISStrideSetStride(that, size, start, step));
}

void IS::BlockSetIndices(Int blockSize, Int blocksNumber, const Int idx[], CopyMode mode) {
  PetscCallThrow(ISBlockSetIndices(that, blockSize, blocksNumber, idx, mode));
}

const Int* IS::GetIndices() {
  const Int* idx;
  PetscCallThrow(ISGetIndices(that, &idx));
  return idx;
}

void IS::RestoreIndices(const Int* idx[]) {
  PetscCallThrow(ISRestoreIndices(that, idx));
}

Int IS::GetLocalSize() const {
  Int size;
  PetscCallThrow(ISGetLocalSize(that, &size));
  return size;
}

Int IS::GetSize() const {
  Int size;
  PetscCallThrow(ISGetSize(that, &size));
  return size;
}

void IS::View(PetscViewer viewer) const {
  PetscCallThrow(ISView(that, viewer));
}

void IS::Destroy() {
  PetscCallThrow(ISDestroy(&that));
}

IS::~IS() noexcept(false) {
  Destroy();
}

}
