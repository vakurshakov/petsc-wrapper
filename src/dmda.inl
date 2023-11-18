#include "dmda.h"

namespace Petsc {

template<typename T>
DA::Borrowed<T> DA::GetArray(Vec& vec, GetArrayType type) {
  return Borrowed<T>(*this, vec, type);
}

template<typename T>
DA::Borrowed<T>::Borrowed(DA& da, Vec& vec, GetArrayType type)
    : da(da), vec(vec), type(type) {
  switch (type) {
    case READ: PetscCallThrow(DMDAVecGetArrayRead(da, vec, (void*)&array)); return;
    case WRITE: PetscCallThrow(DMDAVecGetArrayWrite(da, vec, (void*)&array)); return;
    case DOF: PetscCallThrow(DMDAVecGetArrayDOF(da, vec, (void*)&array)); return;
    case DOF_READ: PetscCallThrow(DMDAVecGetArrayDOFRead(da, vec, (void*)&array)); return;
    case DOF_WRITE: PetscCallThrow(DMDAVecGetArrayDOFWrite(da, vec, (void*)&array)); return;
    default: PetscCallThrow(DMDAVecGetArray(da, vec, (void*)&array)); return;
  }
}

template<typename T>
DA::Borrowed<T>::~Borrowed() noexcept(false) {
  switch (type) {
    case READ: PetscCallThrow(DMDAVecRestoreArrayRead(da, vec, (void*)&array)); return;
    case WRITE: PetscCallThrow(DMDAVecRestoreArrayWrite(da, vec, (void*)&array)); return;
    case DOF: PetscCallThrow(DMDAVecRestoreArrayDOF(da, vec, (void*)&array)); return;
    case DOF_READ: PetscCallThrow(DMDAVecRestoreArrayDOFRead(da, vec, (void*)&array)); return;
    case DOF_WRITE: PetscCallThrow(DMDAVecRestoreArrayDOFWrite(da, vec, (void*)&array)); return;
    default: PetscCallThrow(DMDAVecRestoreArray(da, vec, (void*)&array)); return;
  }
}

}
