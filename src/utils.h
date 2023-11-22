#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <petscsystypes.h>

namespace Petsc {

using Int = PetscInt;
using Bool = PetscBool;
using Real = PetscReal;
using Scalar = PetscScalar;
using MPIInt = PetscMPIInt;

template<typename T>
struct Two {
  T x; T y;

  Two() = default;
  Two(T v) : x(v), y(v) {}
  Two(T x, T y) : x(x), y(y) {}
};

using Int2 = Two<Int>;
using Real2 = Two<Real>;
using Scalar2 = Two<Scalar>;

template<typename T>
struct Three {
  T x; T y; T z;

  Three() = default;
  Three(T v) : x(v), y(v), z(v) {}
  Three(T x, T y, T z) : x(x), y(y), z(z) {}
};

using Int3 = Three<Int>;
using Real3 = Three<Real>;
using Scalar3 = Three<Scalar>;

enum GetArrayType {
  Default = 0,
  Read,
  Write,
  DOF,
  DOF_Read,
  DOF_Write
};

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

}

#endif // SRC_UTILS_H
