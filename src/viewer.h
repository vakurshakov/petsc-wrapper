#ifndef SRC_VIEWER_H
#define SRC_VIEWER_H

#include <string_view>

#include <petscviewer.h>

#include "exception.h"
#include "utils.h"

namespace Petsc {

using FileMode = PetscFileMode;

class Viewer {
 public:
  Viewer(std::string_view name = {});
  PETSC_DEFAULT_COPY_POLICY(Viewer);

  static Viewer Create(MPI_Comm comm);
  void SetFromOptions();
  void SetType(PetscViewerType type);

  void FileSetMode(FileMode mode);
  void FileSetName(const char name[]);

  void Destroy();
  virtual ~Viewer() noexcept(false);

  /// @brief Conversion to PETSc viewer
  operator _p_PetscViewer*() const { return that; }
  operator _p_PetscViewer**() { return &that; }
  operator _p_PetscObject*() const { return reinterpret_cast<PetscObject>(that); }
  operator _p_PetscObject**() { return reinterpret_cast<PetscObject*>(&that); }

 protected:
  _p_PetscViewer* that = nullptr;
};

}

#endif // SRC_VIEWER_H
