#include "context.h"

namespace Petsc {

Context::Context(int *argc, char ***argv) {
  PetscCallThrow(PetscInitialize(argc, argv, NULL, help));
}

Context::~Context() noexcept(false) {
  PetscCallThrow(PetscFinalize());
}

}
