#include "vec.h"

namespace Petsc {

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::BasicBorrowedArray(VecRef vec, GetArrayType type)
    : vec(vec), type(type), array(nullptr) {
  if constexpr (isConst) {
    PetscCallThrow(VecGetArrayRead(vec, &array));
  }
  else if (type == Write) {
    PetscCallThrow(VecGetArrayWrite(vec, &array));
  }
  else {
    PetscCallThrow(VecGetArray(vec, &array));
  }
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::~BasicBorrowedArray() noexcept(false) {
  if constexpr (isConst) {
    PetscCallThrow(VecRestoreArrayRead(vec, &array));
  }
  else if (type == Write) {
    PetscCallThrow(VecRestoreArrayWrite(vec, &array));
  }
  else {
    PetscCallThrow(VecRestoreArray(vec, &array));
  }
}

}
