#ifndef SRC_BINARY_H
#define SRC_BINARY_H

#include <string_view>

#include "viewer.h"

#include "exception.h"
#include "utils.h"

namespace Petsc {

/// @todo binary file compression with .gz postfix

using DataType = PetscDataType;

class Binary : public Viewer {
  /// @brief Private constructor to internally create empty instances
  Binary() = default;

 public:
  Binary(MPI_Comm comm, std::string_view name, FileMode mode);
  PETSC_DEFAULT_COPY_POLICY(Binary);

  static Binary Open(MPI_Comm comm, std::string_view name, FileMode mode);

  static void Write(int fd, const void *data, Int size, DataType type);
  static void Read(int fd, const void *data, Int size, DataType type);

  void SetFlowControl(Int fc);
  Int GetFlowControl();
  void SetUseMPIIO(Bool use);
  Bool GetUseMPIIO();

#ifdef PETSC_HAVE_MPIIO
  MPI_File GetMPIIODescriptor();
  MPI_Offset GetMPIIOOffset();
  void AddMPIIOOffset(MPI_Offset off);
#endif

  int GetDescriptor() const;
  FILE* GetInfoPointer() const;

  Int Read(void* data, Int size, DataType type);
  void Write(const void* data, Int size, DataType type);
  void ReadAll(void* data, Int size, Int64 start, Int64 total, DataType type);
  void WriteAll(const void* data, Int size, Int64 start, Int64 total, DataType type);

  void SkipInfo();
  void SetSkipInfo(Bool skip);
  Bool GetSkipInfo() const;
  void SetSkipOptions(Bool skip);
  Bool GetSkipOptions() const;
  void SetSkipHeader(Bool skip);
  Bool GetSkipHeader() const;
  char** ReadStringArray() const;
  void WriteStringArray(const char* const* data);
};

}

#endif // SRC_BINARY_H
