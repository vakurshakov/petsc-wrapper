#ifndef SRC_MACROS_H
#define SRC_MACROS_H

#include <string>
#include <exception>

#include <petscerror.h>

#define PetscCallThrow(...) \
  do { \
    PetscStackUpdateLine; \
    PetscErrorCode ierr_petsc_call_ = __VA_ARGS__; \
    if (PetscUnlikely(ierr_petsc_call_ != PETSC_SUCCESS)) { \
      const char *text; \
      PetscErrorMessage(ierr_petsc_call_, &text, NULL); \
      std::stringstream ss; \
      ss << "[0]PETSC ERROR: #? " << PETSC_FUNCTION_NAME_CXX << "() at " << __FILE__ << ":" << __LINE__ << "\n"; \
      ss << "code(): (PetscErrorCode)" << ierr_petsc_call_ << "\n"; \
      ss << "what(): " << text << "\n"; \
      throw std::runtime_error(ss.str()); \
    } \
  } while (0)

#endif // SRC_MACROS_H
