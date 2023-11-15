#ifndef SRC_CONTEXT_H
#define SRC_CONTEXT_H

#include <petscsys.h>

#include "exception.h"

#define PETSC_DEFAULT_COPY_POLICY(T)      \
  T(const T& other) = delete;             \
  T& operator=(const T& other) = delete;  \
  \
  T(T&& other) = default;                 \
  T& operator=(T&& other) = delete        \


namespace Petsc {

using Int = PetscInt;
using Bool = PetscBool;
using Real = PetscReal;
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
