#ifndef SRC_MACROS_H
#define SRC_MACROS_H

#include <string>
#include <exception>

#include <petscerror.h>

namespace Petsc {

class Exception : public std::runtime_error {
 public:
  Exception(const std::string& what, PetscErrorCode code)
    : std::runtime_error(what), code_(code) {}

  PetscErrorCode code() const { return code_; }

 private:
  PetscErrorCode code_;
};

}

#define PetscCallThrow(...)                                  \
  do {                                                       \
    PetscStackUpdateLine;                                    \
    PetscErrorCode ierr_petsc_call_ = __VA_ARGS__;           \
    if (PetscUnlikely(ierr_petsc_call_ != PETSC_SUCCESS)) {  \
      std::stringstream msg;                                 \
      msg << "PETSC ERROR: "                                 \
          << PETSC_FUNCTION_NAME_CXX << "() "                \
          << "at " << __FILE__ << ":" << __LINE__;           \
      throw Petsc::Exception(msg.str(), ierr_petsc_call_);   \
    }                                                        \
  } while (0)

#endif // SRC_MACROS_H
