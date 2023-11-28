#ifndef SRC_DMDA_H
#define SRC_DMDA_H

#include <string_view>

#include <petscdmda.h>

#include "dm.h"

#include "exception.h"
#include "utils.h"

namespace Petsc {

class DA : public DM {
 public:
  using StencilType = DMDAStencilType;

  DA(std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(DA);

  static DA Create();
  static DA Create1d(BoundaryType boundary, StencilType type, Int global, Int procs, Int dof, Int s, const Int* ranges);
  static DA Create2d(Two<BoundaryType> boundary, StencilType type, Int2 global, Int2 procs, Int dof, Int s, Two<const Int*> ranges);
  static DA Create3d(Three<BoundaryType> boundary, StencilType type, Int3 global, Int3 procs, Int dof, Int s, Three<const Int*> ranges);

  void SetSizes(Int3 global);
  Int3 GetSizes() const;
  void SetNumProcs(Int3 procs);
  Int3 GetNumProcs() const;
  void SetBoundaryType(Three<BoundaryType> type);
  Three<BoundaryType> GetBoundaryType() const;
  void SetDof(Int dof);
  Int GetDof() const;
  void SetStencilType(StencilType type);
  StencilType GetStencilType() const;
  void SetStencilWidth(Int width);
  Int GetStencilWidth() const;
  void SetOwnershipRanges(Three<const Int*> ranges);
  Three<const Int*> GetOwnershipRanges() const;

  std::pair<Int3, Int3> GetCorners() const;
  std::pair<Int3, Int3> GetGhostCorners() const;

  void SetFieldName(Int nf, const char* name);
  const char* GetFieldName(Int nf) const;
  void SetFieldNames(const char** name);
  const char** GetFieldNames() const;
  void SetCoordinateName(Int nf, const char* name);
  const char* GetCoordinateName(Int nf) const;

  /// @todo const correctness for borrowed arrays
  /// @todo guard type T with std::enable_if, T should be at least pointer
  template<typename T> class Borrowed;
  template<typename T> Borrowed<T> GetArray(Vec& vec, GetArrayType type = Default);
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
