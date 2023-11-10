#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

#include <petscsys.h>

#include "exception.h"

namespace Petsc {

using Real = PetscReal;
using Int = PetscInt;
using Scalar = PetscScalar;

class Context {
 public:
  static const Context &Instance(int *argc, char ***argv) {
    static Context context(argc, argv);
    return context;
  }

 private:
  static constexpr char help[] = "Petsc library context.\n\n";

  Context(int *argc, char ***argv);
  ~Context() noexcept(false);
};

}

#endif // SRC_CONTEXT_H
