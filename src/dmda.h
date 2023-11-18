#ifndef SRC_DMDA_H
#define SRC_DMDA_H

#include <string_view>

#include <petscdmda.h>

#include "dm.h"
#include "exception.h"

namespace Petsc {

class DA : public DM {
 public:
  using StencilType = DMDAStencilType;

  DA(std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(DA);

  static DA Create();
  void SetSizes(Three<Int> global);
  void SetNumProcs(Three<Int> procs);
  void SetBoundaryType(Three<BoundaryType> type);
  void SetDof(Int dof);
  void SetStencilType(StencilType type);
  void SetStencilWidth(Int width);
  void SetOwnershipRanges(Three<const Int*> ranges);

  static DA Create(Three<BoundaryType> boundary, StencilType type,
    Three<Int> global, Three<Int> procs, Int dof, Int s, Three<const Int*> ranges);

  void GetCorners(Three<Int*> corner, Three<Int*> size) const;
  void GetGhostCorners(Three<Int*> corner, Three<Int*> size) const;

  void SetFieldName(Int nf, const char* name);
  const char* GetFieldName(Int nf) const;
  void SetFieldNames(const char** name);
  const char** GetFieldNames() const;
  void SetCoordinateName(Int nf, const char* name);
  const char* GetCoordinateName(Int nf) const;

  template<typename T> class Borrowed;
  template<typename T> Borrowed<T> GetArray(Vec& vec, GetArrayType type = DEFAULT);
};

template<typename T>
class DA::Borrowed {
 public:
  Borrowed(DA& da, Vec& vec, GetArrayType type);
  ~Borrowed() noexcept(false);
  PETSC_NO_COPY_POLICY(Borrowed);

  operator const T&() const { return array; }
  operator T&() { return array; }

 private:
  DA& da;
  Vec& vec;
  GetArrayType type;

  T array;
};

}

#include "dmda.inl"

#endif // SRC_DMDA_H
