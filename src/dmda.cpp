#include "dmda.h"

namespace Petsc {

DA::DA(std::string_view name) : DM(name) {
  PetscCallThrow(DMDACreate(PETSC_COMM_WORLD, &that));
}

/* static */ DA DA::Create() {
  DA da;
  PetscCallThrow(DMDACreate(PETSC_COMM_WORLD, da));
  return da;
}

/* static */ DA DA::Create1d(BoundaryType boundary, Int global, Int dof, Int s, const Int* ranges) {
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

/* static */ DA DA::Create3d(Three<BoundaryType> boundary, StencilType type, Int3 global, Int3 procs, Int dof, Int s, Three<const Int*> ranges) {
  DA da;
  PetscCallThrow(DMDACreate3d(PETSC_COMM_WORLD,
    boundary.x, boundary.y, boundary.z, type, global.x, global.y, global.z, procs.x, procs.y, procs.z, dof, s, ranges.x, ranges.y, ranges.z, da));
  return da;
}

void DA::SetSizes(Int3 global) {
  PetscCallThrow(DMDASetSizes(that, global.x, global.y, global.z));
}

Int3 DA::GetSizes() const {
  Int3 size;
  PetscCallThrow(DMDAGetInfo(that, NULL, &size.x, &size.y, &size.z, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
  return size;
}

void DA::SetNumProcs(Int3 procs) {
  PetscCallThrow(DMDASetNumProcs(that, procs.x, procs.y, procs.z));
}

Int3 DA::GetNumProcs() const {
  Int3 procs;
  PetscCallThrow(DMDAGetInfo(that, NULL, NULL, NULL, NULL, &procs.x, &procs.y, &procs.y, NULL, NULL, NULL, NULL, NULL, NULL));
  return procs;
}

void DA::SetBoundaryType(Three<BoundaryType> boundary) {
  PetscCallThrow(DMDASetBoundaryType(that, boundary.x, boundary.y, boundary.z));
}

Three<DM::BoundaryType> DA::GetBoundaryType() const {
  Three<DM::BoundaryType> bound;
  PetscCallThrow(DMDAGetInfo(that, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &bound.x, &bound.y, &bound.z, NULL));
  return bound;
}

void DA::SetDof(Int dof) {
  PetscCallThrow(DMDASetDof(that, dof));
}

Int DA::GetDof() const {
  Int dof;
  PetscCallThrow(DMDAGetDof(that, &dof));
  return dof;
}

void DA::SetStencilType(StencilType type) {
  PetscCallThrow(DMDASetStencilType(that, type));
}

DA::StencilType DA::GetStencilType() const {
  DA::StencilType type;
  PetscCallThrow(DMDAGetStencilType(that, &type));
  return type;
}

void DA::SetStencilWidth(Int s) {
  PetscCallThrow(DMDASetStencilWidth(that, s));
}

Int DA::GetStencilWidth() const {
  Int width;
  PetscCallThrow(DMDAGetStencilWidth(that, &width));
  return width;
}

void DA::SetOwnershipRanges(Three<const Int*> ranges) {
  PetscCallThrow(DMDASetOwnershipRanges(that, ranges.x, ranges.y, ranges.z));
}

Three<const Int*> DA::GetOwnershipRanges() const {
  Three<const Int*> ranges;
  PetscCallThrow(DMDAGetOwnershipRanges(that, &ranges.x, &ranges.y, &ranges.z));
  return ranges;
}

std::pair<Int3, Int3> DA::GetCorners() const {
  Int3 corner, size;
  PetscCallThrow(DMDAGetCorners(that, &corner.x, &corner.y, &corner.z, &size.x, &size.y, &size.z));
  return std::make_pair(corner, size);
}

std::pair<Int3, Int3> DA::GetGhostCorners() const {
  Int3 corner, size;
  PetscCallThrow(DMDAGetGhostCorners(that, &corner.x, &corner.y, &corner.z, &size.x, &size.y, &size.z));
  return std::make_pair(corner, size);
}

void DA::SetFieldName(Int nf, const char* name) {
  PetscCallThrow(DMDASetFieldName(that, nf, name));
}

const char* DA::GetFieldName(Int nf) const  {
  const char* name;
  PetscCallThrow(DMDAGetFieldName(that, nf, &name));
  return name;
}

void DA::SetFieldNames(const char** names)  {
  PetscCallThrow(DMDASetFieldNames(that, names));
}

const char** DA::GetFieldNames() const  {
  const char** names;
  PetscCallThrow(DMDAGetFieldNames(that, const_cast<const char* const**>(&names)));
  return names;
}

void DA::SetCoordinateName(Int nf, const char* name)  {
  PetscCallThrow(DMDASetCoordinateName(that, nf, name));
}

const char* DA::GetCoordinateName(Int nf) const  {
  const char* name;
  PetscCallThrow(DMDAGetCoordinateName(that, nf, &name));
  return name;
}

}
