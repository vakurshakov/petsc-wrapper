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
    case Default: PetscCallThrow(DMDAVecGetArray(da, vec, (void*)&array)); return;
    case Read: PetscCallThrow(DMDAVecGetArrayRead(da, vec, (void*)&array)); return;
    case Write: PetscCallThrow(DMDAVecGetArrayWrite(da, vec, (void*)&array)); return;
    case DOF: PetscCallThrow(DMDAVecGetArrayDOF(da, vec, (void*)&array)); return;
    case DOF_Read: PetscCallThrow(DMDAVecGetArrayDOFRead(da, vec, (void*)&array)); return;
    case DOF_Write: PetscCallThrow(DMDAVecGetArrayDOFWrite(da, vec, (void*)&array)); return;
    default: PetscCallThrow(PETSC_ERR_ARG_WRONG);
  }
}

template<typename T>
DA::Borrowed<T>::~Borrowed() noexcept(false) {
  switch (type) {
    case Default: PetscCallThrow(DMDAVecRestoreArray(da, vec, (void*)&array)); return;
    case Read: PetscCallThrow(DMDAVecRestoreArrayRead(da, vec, (void*)&array)); return;
    case Write: PetscCallThrow(DMDAVecRestoreArrayWrite(da, vec, (void*)&array)); return;
    case DOF: PetscCallThrow(DMDAVecRestoreArrayDOF(da, vec, (void*)&array)); return;
    case DOF_Read: PetscCallThrow(DMDAVecRestoreArrayDOFRead(da, vec, (void*)&array)); return;
    case DOF_Write: PetscCallThrow(DMDAVecRestoreArrayDOFWrite(da, vec, (void*)&array)); return;
    default: PetscCallThrow(PETSC_ERR_ARG_WRONG);
  }
}

}
