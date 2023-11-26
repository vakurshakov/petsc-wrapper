#include "viewer.h"

namespace Petsc {

Viewer::Viewer(_p_PetscViewer* other) : that(other) {}

Viewer Viewer::STDERR_WORLD = PETSC_VIEWER_STDERR_WORLD;
Viewer Viewer::STDERR_SELF = PETSC_VIEWER_STDERR_SELF;
Viewer Viewer::STDOUT_WORLD = PETSC_VIEWER_STDOUT_WORLD;
Viewer Viewer::STDOUT_SELF = PETSC_VIEWER_STDOUT_SELF;
Viewer Viewer::BINARY_WORLD = PETSC_VIEWER_BINARY_WORLD;
Viewer Viewer::BINARY_SELF = PETSC_VIEWER_BINARY_SELF;

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
