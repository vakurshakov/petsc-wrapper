#include <petscksp.h>

#include <context.h>
#include <exception.h>
#include <vec.h>

void using_read_write_vector(Petsc::Vec& vec) {
  for (auto& value : vec.GetArray()) {
    value = 1.0;  // note that vec is not constant, we can modify it through arr
  }
  PetscCallThrow(VecView(vec, PETSC_VIEWER_STDOUT_WORLD));
  // array is restored in arr destructor
}

void using_read_write_vector_to_read(Petsc::Vec& vec) {
  Petsc::Int i = 0;
  for (auto& value : vec.GetArrayRead()) {
    // value = 0.0;  // compilation error, since arr is ConstArray
    PetscCallThrow(PetscPrintf(PETSC_COMM_WORLD, "arr[%" PetscInt_FMT "]: %g\n", i, value));  // reading is OK
    ++i;
  }
}

void using_read_only_vector(const Petsc::Vec& vec) {
  Petsc::Int i = 0;
  for (auto& value : vec.GetArray()) {
    // value = 0.0;  // compilation error
    PetscCallThrow(PetscPrintf(PETSC_COMM_WORLD, "arr[%" PetscInt_FMT "]: %g\n", i, value));  // reading is OK
    ++i;
  }
}


int main(int argc, char** argv) {
  using namespace Petsc;
  Int globalSize = 10;

  try {
    Context::Instance(&argc, &argv);

    // creating default read-write vector
    auto x = Petsc::Vec::FromGlobals(globalSize);
    using_read_write_vector(x);

    // creating read-write copy of vector
    auto y = x.Copy();
    using_read_write_vector_to_read(y);

    // declaring vector y as read-only with const
    const auto z = x.Copy();
    using_read_only_vector(z);
  }
  catch (const Petsc::Exception& e) {
    PetscCall(PetscPrintf(PETSC_COMM_WORLD, e.what()));
    PetscCallMPI(MPI_Abort(PETSC_COMM_WORLD, (MPIInt)e.code()));
  }
  catch (...) {
    PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Unkown exception captured!"));
  }

  return EXIT_SUCCESS;
}
