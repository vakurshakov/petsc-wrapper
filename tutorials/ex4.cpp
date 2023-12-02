#include <context.h>
#include <exception.h>
#include <binary.h>
#include <dmda.h>

constexpr const char* output_filename = "ex4_out";

void fill_vector(Petsc::DA& da, Petsc::Vec& vec);
void compare_vectors(const Petsc::Vec& lhs, const Petsc::Vec& rhs);

void load_backup(Petsc::Vec& vec);
void save_backup(const Petsc::Vec& vec);


int main(int argc, char** argv) {
  using namespace Petsc;

  try {
    Context::Instance(&argc, &argv);

#ifndef PETSC_HAVE_MPIIO
    Printf(PETSC_COMM_WORLD, "Warning: Executing requires a working MPI-2 implementation\n");
    return EXIT_SUCCESS;
#endif

    Int3 globalSize = {2, 3, 4};
    auto da = Petsc::DA::Create3d(DM_BOUNDARY_NONE, DMDA_STENCIL_BOX, globalSize, PETSC_DECIDE, 1, 1, NULL);
    da.SetFromOptions();
    da.SetUp();

    Petsc::Vec x = da.CreateGlobalVector();
    fill_vector(da, x);

    save_backup(x);

    Petsc::Vec y = x.Duplicate();
    load_backup(y);

    compare_vectors(x, y);
  }
  catch (const Petsc::Exception& e) {
    Printf(PETSC_COMM_WORLD, e.what());
    PetscCallMPI(MPI_Abort(PETSC_COMM_WORLD, (MPIInt)e.code()));
  }
  catch (...) {
    Printf(PETSC_COMM_WORLD, "Unkown exception captured!");
  }

  return EXIT_SUCCESS;
}


void fill_vector(Petsc::DA& da, Petsc::Vec& vec)  {
  using namespace Petsc;

  auto [localStart, localSize] = da.GetCorners();
  auto globalSize = da.GetSizes();
  auto dof = da.GetDof();

  // Note the index ordering: array[z][y][x][DOF], and that indexes are global
  auto array = da.GetArray<Scalar****>(vec, DOF);
  for (Int k = localStart.z; k < localStart.z + localSize.z; ++k) {
  for (Int j = localStart.y; j < localStart.y + localSize.y; ++j) {
  for (Int i = localStart.x; i < localStart.x + localSize.x; ++i) {
    for (Int l = 0; l < dof; l++) {
      array[k][j][i][l] = l + dof * (i + globalSize.x * (j + globalSize.y * k));
    }
  }}}
}

void save_backup(const Petsc::Vec& vec) {
  MPI_Comm comm;
  PetscCallThrow(PetscObjectGetComm(vec, &comm));

  auto viewer = Petsc::Binary::Open(comm, output_filename, FILE_MODE_WRITE);
  viewer.SetUseMPIIO(PETSC_TRUE);
  viewer.SetSkipInfo(PETSC_TRUE);
  viewer.SetSkipHeader(PETSC_TRUE);

  vec.View(viewer);
}

void load_backup(Petsc::Vec& vec) {
  MPI_Comm comm;
  PetscCallThrow(PetscObjectGetComm(vec, &comm));

  auto viewer = Petsc::Binary::Open(comm, output_filename, FILE_MODE_READ);
  viewer.SetUseMPIIO(PETSC_TRUE);
  viewer.SetSkipInfo(PETSC_TRUE);

  // the same flags should be chosen as in save_backup()
  viewer.SetSkipHeader(PETSC_TRUE);

  vec.Load(viewer);
}

void compare_vectors(const Petsc::Vec& lhs, const Petsc::Vec& rhs) {
  using namespace Petsc;
  Printf(PETSC_COMM_WORLD, "Vectors comparison:\n");

  auto [l_argmin, l_min] = lhs.Min();
  auto [l_argmax, l_max] = lhs.Max();
  Printf(PETSC_COMM_WORLD, "  min(a)     = %+1.2e [argmin %" PetscInt_FMT "]\n", l_min, l_argmin);
  Printf(PETSC_COMM_WORLD, "  max(a)     = %+1.2e [argmax %" PetscInt_FMT "]\n", l_max, l_argmax);

  auto [r_argmin, r_min] = rhs.Min();
  auto [r_argmax, r_max] = rhs.Max();

  Printf(PETSC_COMM_WORLD, "  min(b)     = %+1.2e [argmin %" PetscInt_FMT "]\n", r_min, r_argmin);
  Printf(PETSC_COMM_WORLD, "  max(b)     = %+1.2e [argmax %" PetscInt_FMT "]\n", r_max, r_argmax);

  auto [argmin, min] = Petsc::Vec::WAXPY(-1.0, rhs, lhs).Min();
  if (double diff = PetscAbsReal(min); diff > 1.0e-10) {
    Printf(PETSC_COMM_WORLD, "  ERROR: min(|a-b|) > 1.0e-10\n");
    Printf(PETSC_COMM_WORLD, "  min(|a-b|) = %+1.10e\n", diff);
  } else {
    Printf(PETSC_COMM_WORLD, "  min(|a-b|) < 1.0e-10\n");
  }
}
