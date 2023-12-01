#include <vector>

#include <context.h>
#include <exception.h>
#include <vec.h>
#include <is.h>

void fill_vector(Petsc::Vec& vec);
void setup_index_set(const Petsc::Vec& vec, Petsc::IS& is);
void view_subvector(Petsc::Vec& vec, const Petsc::IS& is);

int main(int argc, char** argv) {
  using namespace Petsc;

  try {
    Context::Instance(&argc, &argv);

    Int globalSize = 9;

    auto x = Petsc::Vec::FromGlobals(globalSize);
    fill_vector(x);

    auto is = Petsc::IS::Create(PETSC_COMM_WORLD);
    setup_index_set(x, is);

    view_subvector(x, is);
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


void fill_vector(Petsc::Vec& vec) {
  Petsc::Printf(PETSC_COMM_WORLD, "Vector:\n");

  auto [rstart, _] = vec.GetOwnershipRange();
  auto arr = vec.GetArray();
  for (auto it = arr.begin(); it != arr.end(); ++it) {
    it.value() = rstart + it.index();
  }
  vec.View(PETSC_VIEWER_STDOUT_WORLD);
}

void setup_index_set(const Petsc::Vec& vec, Petsc::IS& is) {
  using namespace Petsc;

  MPIInt rank;
  MPI_Comm_rank(PETSC_COMM_WORLD, &rank);

  auto [rstart, rend] = vec.GetOwnershipRange();

  std::vector<Int> indices = {
    std::min(rstart + rank + 1, rend - 1),
    std::min(rstart + rank + 2, rend - 1),
    std::min(rstart + rank + 3, rend - 1)
  };

  is.SetType(ISGENERAL);
  is.GeneralSetIndices(indices.size(), indices.data(), PETSC_COPY_VALUES);
  {
    Printf(PETSC_COMM_SELF, "\nPrinting indices directly, process [%i]\n", rank);

    Int isLocalSize = is.GetLocalSize();
    const Int *indices = is.GetIndices();
    for (Int i = 0; i < isLocalSize; i++) {
      Printf(PETSC_COMM_SELF, "%" PetscInt_FMT "\n", indices[i]);
    }
    is.RestoreIndices(&indices);
  }
}

void view_subvector(Petsc::Vec& vec, const Petsc::IS& is) {
  Petsc::Printf(PETSC_COMM_WORLD, "\nSubVector:\n");

  auto sub = vec.GetSubVector(is);
  static_cast<const Petsc::Vec&>(sub).View(PETSC_VIEWER_STDOUT_WORLD);
}
