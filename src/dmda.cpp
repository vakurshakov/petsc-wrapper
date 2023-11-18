#include "dmda.h"

namespace Petsc {

DA::DA(std::string_view name) : DM(name) {
  PetscCallThrow(DMDACreate(PETSC_COMM_WORLD, &data));
}

/* static */ DA DA::Create() {
  DA da;
  PetscCallThrow(DMDACreate(PETSC_COMM_WORLD, da));
  return da;
}

/* static */ DA DA::Create1d(BoundaryType boundary, StencilType type, Int global, Int procs, Int dof, Int s, const Int* ranges) {
  DA da;
  PetscCallThrow(DMDACreate1d(PETSC_COMM_WORLD,
    boundary, global, dof, s, ranges, da));
  return da;
}

/* static */ DA DA::Create2d(Two<BoundaryType> boundary, StencilType type, Two<Int> global, Two<Int> procs, Int dof, Int s, Two<const Int*> ranges) {
  DA da;
  PetscCallThrow(DMDACreate2d(PETSC_COMM_WORLD,
    boundary.x, boundary.y, type, global.x, global.y, procs.x, procs.y, dof, s, ranges.x, ranges.y, da));
  return da;
}

/* static */ DA DA::Create3d(Three<BoundaryType> boundary, StencilType type, Three<Int> global, Three<Int> procs, Int dof, Int s, Three<const Int*> ranges) {
  DA da;
  PetscCallThrow(DMDACreate3d(PETSC_COMM_WORLD,
    boundary.x, boundary.y, boundary.z, type, global.x, global.y, global.z, procs.x, procs.y, procs.z, dof, s, ranges.x, ranges.y, ranges.z, da));
  return da;
}

void DA::SetSizes(Three<Int> global) {
  PetscCallThrow(DMDASetSizes(data, global.x, global.y, global.z));
}

void DA::SetNumProcs(Three<Int> procs) {
  PetscCallThrow(DMDASetNumProcs(data, procs.x, procs.y, procs.z));
}

void DA::SetBoundaryType(Three<BoundaryType> boundary) {
  PetscCallThrow(DMDASetBoundaryType(data, boundary.x, boundary.y, boundary.z));
}

void DA::SetDof(Int dof) {
  PetscCallThrow(DMDASetDof(data, dof));
}

void DA::SetStencilType(StencilType type) {
  PetscCallThrow(DMDASetStencilType(data, type));
}

void DA::SetStencilWidth(Int s) {
  PetscCallThrow(DMDASetStencilWidth(data, s));
}

void DA::SetOwnershipRanges(Three<const Int*> ranges) {
  PetscCallThrow(DMDASetOwnershipRanges(data, ranges.x, ranges.y, ranges.z));
}

void DA::GetCorners(Three<Int*> corner, Three<Int*> size) const {
  PetscCallThrow(DMDAGetCorners(data, corner.x, corner.y, corner.z, size.x, size.y, size.z));
}

void DA::GetGhostCorners(Three<Int*> corner, Three<Int*> size) const {
  PetscCallThrow(DMDAGetGhostCorners(data, corner.x, corner.y, corner.z, size.x, size.y, size.z));
}

void DA::SetFieldName(Int nf, const char* name) {
  PetscCallThrow(DMDASetFieldName(data, nf, name));
}

const char* DA::GetFieldName(Int nf) const  {
  const char* name;
  PetscCallThrow(DMDAGetFieldName(data, nf, &name));
  return name;
}

void DA::SetFieldNames(const char** names)  {
  PetscCallThrow(DMDASetFieldNames(data, names));
}

const char** DA::GetFieldNames() const  {
  const char** names;
  PetscCallThrow(DMDAGetFieldNames(data, const_cast<const char* const**>(&names)));
  return names;
}

void DA::SetCoordinateName(Int nf, const char* name)  {
  PetscCallThrow(DMDASetCoordinateName(data, nf, name));
}

const char* DA::GetCoordinateName(Int nf) const  {
  const char* name;
  PetscCallThrow(DMDAGetCoordinateName(data, nf, &name));
  return name;
}

}
