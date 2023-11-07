#include <petscksp.h>

#include "context.h"
#include "vec.h"
#include "mat.h"

#include <iostream>

// #include <Eigen/Sparse>
// using Operator = Eigen::SparseMatrix<double, Eigen::RowMajor>;
// Operator some(2, 3);
// some.insert(0, 1) = 34;
// some.insert(1, 2) = 56;
// for (int k = 0; k < some.outerSize(); ++k) {
//   for (Operator::InnerIterator it(some, k); (bool)it; ++it) {
//     std::cout << it.row() << "\t";
//     std::cout << it.col() << "\t";
//     std::cout << it.value() << std::endl;
//   }
// }

int main(int argc, char** argv) {
  try {
    Petsc::Context::Instance(&argc, &argv);

    Petsc::Int globalSize = 500;

    auto x = Petsc::Vec::FromOptions(globalSize, "Approximate solution");
    auto b = Petsc::Vec::Duplicate(x);
    auto u = Petsc::Vec::Duplicate(x);

    auto [start, end] = x.GetOwnershipRange();
    auto localSize = x.LocalSize();

    Petsc::Mat A(localSize, localSize, globalSize, globalSize, "Linear system");

    // Stencil laplace operator
    {
      Petsc::Int i;
      Petsc::Int col[3];
      Petsc::Scalar value[3];

      if (!start) {
        start    = 1;
        i        = 0;
        col[0]   = 0;
        col[1]   = 1;
        value[0] = +2.0;
        value[1] = -1.0;
        A.SetValues(1, &i, 2, col, value, INSERT_VALUES);
      }
      if (end == globalSize) {
        end      = globalSize - 1;
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
      for (i = start; i < end; i++) {
        col[0] = i - 1;
        col[1] = i;
        col[2] = i + 1;
        A.SetValues(1, &i, 3, col, value, INSERT_VALUES);
      }

      A.AssemblyBegin(MAT_FINAL_ASSEMBLY);
      A.AssemblyEnd(MAT_FINAL_ASSEMBLY);
    }

    // Set exact solution; then compute right-hand-side vector
    PetscCall(VecSet(u, 1.0));
    PetscCall(MatMult(A, u, b));

    // Create the linear solver and set various options
    KSP ksp;
    PetscCall(KSPCreate(PETSC_COMM_WORLD, &ksp));
    PetscCall(KSPSetOperators(ksp, A, A));
    PetscCall(KSPSetTolerances(ksp, 1e-5, 1e-5, PETSC_DEFAULT, PETSC_DEFAULT));
    PetscCall(KSPSetFromOptions(ksp));
    PetscCall(KSPSolve(ksp, b, x));

    // Check the solution and clean up
    Petsc::Int its;
    Petsc::Real error_norm;
    PetscCall(KSPView(ksp, PETSC_VIEWER_STDOUT_WORLD));
    PetscCall(VecAXPY(x, -1.0, u));
    PetscCall(VecNorm(x, NORM_2, &error_norm));
    PetscCall(KSPGetIterationNumber(ksp, &its));
    PetscCall(PetscPrintf(PETSC_COMM_WORLD, "Norm of error %g, Iterations %" PetscInt_FMT "\n", (double)error_norm, its));
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
