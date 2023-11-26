#include "binary.h"

namespace Petsc {

Binary::Binary(MPI_Comm comm, std::string_view name, FileMode mode)
    : Viewer(name) {
  PetscCallThrow(PetscViewerBinaryOpen(comm, name.data(), mode, &that));
}

Binary Binary::Open(MPI_Comm comm, std::string_view name, FileMode mode) {
  Binary viewer;
  PetscCallThrow(PetscViewerBinaryOpen(comm, name.data(), mode, viewer));
  return viewer;
}

void Binary::Write(int fd, const void *data, Int size, DataType type) {
  PetscCallThrow(PetscBinaryWrite(fd, data, size, type));
}

void Binary::Read(int fd, const void *data, Int size, DataType type) {
  PetscCallThrow(PetscBinaryWrite(fd, data, size, type));
}

Int Binary::GetFlowControl() {
  Int fc;
  PetscCallThrow(PetscViewerBinaryGetFlowControl(that, &fc));
  return fc;
}

void Binary::SetFlowControl(Int fc) {
  PetscCallThrow(PetscViewerBinarySetFlowControl(that, fc));
}

void Binary::SetUseMPIIO(Bool use) {
  PetscCallThrow(PetscViewerBinarySetUseMPIIO(that, use));
}

Bool Binary::GetUseMPIIO() {
  Bool use;
  PetscCallThrow(PetscViewerBinaryGetUseMPIIO(that, &use));
  return use;
}

#ifdef PETSC_HAVE_MPIIO
MPI_File Binary::GetMPIIODescriptor() {
  MPI_File fd;
  PetscCallThrow(PetscViewerBinaryGetMPIIODescriptor(that, &fd));
  return fd;
}

MPI_Offset Binary::GetMPIIOOffset() {
  MPI_Offset off;
  PetscCallThrow(PetscViewerBinaryGetMPIIOOffset(that, &off));
  return off;
}

void Binary::AddMPIIOOffset(MPI_Offset off) {
  PetscCallThrow(PetscViewerBinaryAddMPIIOOffset(that, off));
}
#endif

int Binary::GetDescriptor() const {
  int fd;
  PetscCallThrow(PetscViewerBinaryGetDescriptor(that, &fd));
  return fd;
}

FILE* Binary::GetInfoPointer() const {
  FILE* file;
  PetscCallThrow(PetscViewerBinaryGetInfoPointer(that, &file));
  return file;
}

Int Binary::Read(void* data, Int size, DataType type) {
  Int count;
  PetscCallThrow(PetscViewerBinaryRead(that, data, size, &count, type));
  return count;
}

void Binary::Write(const void* data, Int size, DataType type) {
  PetscCallThrow(PetscViewerBinaryWrite(that, data, size, type));
}

void Binary::ReadAll(void* data, Int size, Int64 start, Int64 total, DataType type) {
  PetscCallThrow(PetscViewerBinaryReadAll(that, data, size, start, total, type));
}

void Binary::WriteAll(const void* data, Int size, Int64 start, Int64 total, DataType type) {
  PetscCallThrow(PetscViewerBinaryWriteAll(that, data, size, start, total, type));
}

void Binary::SkipInfo() {
  PetscCallThrow(PetscViewerBinarySkipInfo(that));
}

void Binary::SetSkipInfo(Bool skip) {
  PetscCallThrow(PetscViewerBinarySetSkipInfo(that, skip));
}

Bool Binary::GetSkipInfo() const {
  Bool skip;
  PetscCallThrow(PetscViewerBinaryGetSkipInfo(that, &skip));
  return skip;
}

void Binary::SetSkipOptions(Bool skip) {
  PetscCallThrow(PetscViewerBinarySetSkipOptions(that, skip));
}

Bool Binary::GetSkipOptions() const {
  Bool skip;
  PetscCallThrow(PetscViewerBinaryGetSkipOptions(that, &skip));
  return skip;
}

void Binary::SetSkipHeader(Bool skip) {
  PetscCallThrow(PetscViewerBinarySetSkipHeader(that, skip));
}

Bool Binary::GetSkipHeader() const {
  Bool skip;
  PetscCallThrow(PetscViewerBinaryGetSkipHeader(that, &skip));
  return skip;
}

char** Binary::ReadStringArray() const {
  char** arr;
  PetscCallThrow(PetscViewerBinaryReadStringArray(that, &arr));
  return arr;
}

void Binary::WriteStringArray(const char* const* arr) {
  PetscCallThrow(PetscViewerBinaryWriteStringArray(that, arr));
}

}
