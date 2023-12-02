#include <vector>

#include <context.h>
#include <exception.h>
#include <binary.h>
#include <dmda.h>
#include <is.h>

constexpr const char* output_filename = "ex6_out";

void fill_vector(Petsc::DA& da, Petsc::Vec& vec);
void setup_index_set(const Petsc::DA& da, Petsc::IS& is);
void save_component(const Petsc::Vec& vec);
void load_component(Petsc::Vec& vec);
void compare_components(const Petsc::Vec& lhs, const Petsc::Vec& rhs);

int main(int argc, char** argv) {
  using namespace Petsc;

  try {
    Context::Instance(&argc, &argv);

    Int dof = 3;
    Int3 globalSize = {2, 3, 4};
    auto da = Petsc::DA::Create3d(DM_BOUNDARY_NONE, DMDA_STENCIL_BOX, globalSize, PETSC_DECIDE, dof, 1, NULL);
    da.SetUp();

    auto x = da.CreateGlobalVector();
    fill_vector(da, x);

    auto is = Petsc::IS::Create(PETSC_COMM_WORLD);
    setup_index_set(da, is);

    auto subVec = x.GetSubVector(is);
    save_component(subVec);

    auto x_comp = Petsc::Vec::FromGlobals(is.GetSize());
    load_component(x_comp);

    compare_components(subVec, x_comp);
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

  auto [rstart, _] = vec.GetOwnershipRange();
  auto arr = vec.GetArray();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    it.value() = rstart + it.index();
  }
  // vec.View(PETSC_VIEWER_STDOUT_WORLD);
}

void setup_index_set(const Petsc::DA& da, Petsc::IS& is) {
  using namespace Petsc;

  auto [localStart, localSize] = da.GetCorners();
  auto globalSize = da.GetSizes();
  auto dof = da.GetDof();

  Int comp = 1;
  Int j = globalSize.y / 2;

  std::vector<Int> indices;
  for (Int k = localStart.z; k < localStart.z + localSize.z; ++k) {
  // for (Int j = localStart.y; j < localStart.y + localSize.y; ++j) {
  for (Int i = localStart.x; i < localStart.x + localSize.x; ++i) {
    indices.emplace_back(comp + dof * (i + globalSize.x * (j + globalSize.y * k)));
  }}

  is.SetType(ISGENERAL);
  is.GeneralSetIndices(indices.size(), indices.data(), PETSC_COPY_VALUES);
}

void save_component(const Petsc::Vec& vec) {
  MPI_Comm comm;
  PetscCallThrow(PetscObjectGetComm(vec, &comm));

  auto viewer = Petsc::Binary::Open(comm, output_filename, FILE_MODE_WRITE);
  viewer.SetUseMPIIO(PETSC_TRUE);
  viewer.SetSkipInfo(PETSC_TRUE);
  viewer.SetSkipHeader(PETSC_TRUE);

  vec.View(viewer);
}

void load_component(Petsc::Vec& vec) {
  MPI_Comm comm;
  PetscCallThrow(PetscObjectGetComm(vec, &comm));

  auto viewer = Petsc::Binary::Open(comm, output_filename, FILE_MODE_READ);
  viewer.SetUseMPIIO(PETSC_TRUE);
  viewer.SetSkipInfo(PETSC_TRUE);
  viewer.SetSkipHeader(PETSC_TRUE);

  vec.Load(viewer);
}

void compare_components(const Petsc::Vec& lhs, const Petsc::Vec& rhs) {
  using namespace Petsc;
  Printf(PETSC_COMM_WORLD, "Vector slices comparison:\n");

  auto [argmin, min] = Petsc::Vec::WAXPY(-1.0, rhs, lhs).Min();
  if (double diff = PetscAbsReal(min); diff > 1.0e-10) {
    Printf(PETSC_COMM_WORLD, "  ERROR: min(|a-b|) > 1.0e-10\n");
    Printf(PETSC_COMM_WORLD, "  min(|a-b|) = %+1.10e, at %" PetscInt_FMT "\n", diff, argmin);
  } else {
    Printf(PETSC_COMM_WORLD, "  min(|a-b|) < 1.0e-10\n");
  }
}
