#include "viewer.h"

namespace Petsc {

Viewer::Viewer(std::string_view name) {
  if (!name.empty()) {
    PetscCallThrow(PetscObjectSetName(*this, name.data()));
  }
}

/* static */ Viewer Viewer::Create(MPI_Comm comm) {
  Viewer viewer;
  PetscCallThrow(PetscViewerCreate(comm, viewer));
  return viewer;
}

void Viewer::SetFromOptions() {
  PetscCallThrow(PetscViewerSetFromOptions(that));
}

void Viewer::SetType(PetscViewerType type) {
  PetscCallThrow(PetscViewerSetType(that, type));
}

void Viewer::FileSetMode(FileMode mode) {
  PetscCallThrow(PetscViewerFileSetMode(that, mode));
}

void Viewer::FileSetName(const char name[]) {
  PetscCallThrow(PetscViewerFileSetName(that, name));
}

void Viewer::Destroy() {
  PetscCallThrow(PetscViewerDestroy(&that));
}

Viewer::~Viewer() noexcept(false) {
  Destroy();
}

}
