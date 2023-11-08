#include <petscksp.h>

#include "src/context.h"
#include "src/vec.h"
#include "src/mat.h"
#include "src/ksp.h"

#include <iostream>

int main(int argc, char** argv) {
  try {
    Petsc::Context::Instance(&argc, &argv);

    Petsc::Int globalSize = 500;

    auto x = Petsc::Vec::FromOptions(globalSize, "Approximate solution");
    auto b = Petsc::Vec::Duplicate(x);
    auto u = Petsc::Vec::Duplicate(x);

    auto [localStart, localEnd] = x.GetOwnershipRange();
    auto localSize = x.LocalSize();

    Petsc::Mat A(localSize, localSize, globalSize, globalSize, "Linear system");

    // Stencil laplace operator
    {
      Petsc::Int i;
      Petsc::Int col[3];
      Petsc::Scalar value[3];

      if (!localStart) {
        localStart = 1;
        i        = 0;
        col[0]   = 0;
        col[1]   = 1;
        value[0] = +2.0;
        value[1] = -1.0;
        A.SetValues(1, &i, 2, col, value, INSERT_VALUES);
      }
      if (localEnd == globalSize) {
        localEnd = globalSize - 1;
        i        = globalSize - 1;
        col[0]   = globalSize - 2;
        col[1]   = globalSize - 1;
        value[0] = -1.0;
        value[1] = +2.0;
        A.SetValues(1, &i, 2, col, value, INSERT_VALUES);
      }

      // Set entries corresponding to the mesh interior
      value[0] = -1.0;
      value[1] = +2.0;
      value[2] = -1.0;
      for (i = localStart; i < localEnd; i++) {
        col[0] = i - 1;
        col[1] = i;
        col[2] = i + 1;
        A.SetValues(1, &i, 3, col, value, INSERT_VALUES);
      }

      A.AssemblyBegin(MAT_FINAL_ASSEMBLY);
      A.AssemblyEnd(MAT_FINAL_ASSEMBLY);
    }

    // Set exact solution; then compute right-hand-side vector
    u.Set(1.0);
    A.Mult(u, b);

    auto ksp = Petsc::KSP::FromOptions("Linear solver");
    ksp.SetOperators(A, A);
    ksp.Solve(b, x);

    // Check the solution and clean up
    ksp.View(PETSC_VIEWER_STDOUT_WORLD);

    Petsc::Real error_norm = x.AXPY(-1.0, u).Norm(NORM_2);
    Petsc::Int iterations = ksp.GetIterationNumber();
    PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Norm of error %g, Iterations %" PetscInt_FMT "\n", (double)error_norm, iterations));
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
