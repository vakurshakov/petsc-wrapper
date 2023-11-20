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

#define PETSC_NO_COPY_POLICY(T)           \
  T(const T& other) = delete;             \
  T& operator=(const T& other) = delete;  \
  \
  T(T&& other) = delete;                  \
  T& operator=(T&& other) = delete        \


namespace Petsc {

using Int = PetscInt;
using Bool = PetscBool;
using Real = PetscReal;
using Scalar = PetscScalar;
using MPIInt = PetscMPIInt;

template<typename T> struct Two { T x; T y; };
using Int2 = Two<Int>;
using Real2 = Two<Real>;
using Scalar2 = Two<Scalar>;

template<typename T> struct Three { T x; T y; T z; };
using Int3 = Three<Int>;
using Real3 = Three<Real>;
using Scalar3 = Three<Scalar>;

enum GetArrayType {
  DEFAULT = 0,
  READ,
  WRITE,
  DOF,
  DOF_READ,
  DOF_WRITE
};


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
