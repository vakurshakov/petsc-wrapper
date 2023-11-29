#include <petscksp.h>

#include <context.h>
#include <exception.h>
#include <vec.h>
#include <mat.h>
#include <ksp.h>

int main(int argc, char** argv) {
  using namespace Petsc;

  try {
    Context::Instance(&argc, &argv);

    Int globalSize = 500;

    auto x = Petsc::Vec::FromGlobals(globalSize, "Approximate solution");
    auto b = x.Duplicate();
    auto u = x.Duplicate();

    auto [localStart, localEnd] = x.GetOwnershipRange();
    auto localSize = x.GetLocalSize();

    Petsc::Mat A(localSize, localSize, globalSize, globalSize, "Linear system");

    // Stencil laplace operator
    {
      Int i;
      Int col[3];
      Scalar value[3];

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

    auto reason = ksp.GetConvergedReason();
    auto iterations = ksp.GetIterationNumber();
    auto error_norm = x.AXPY(-1.0, u).Norm(NORM_2);
    Printf(PETSC_COMM_WORLD, "Converged reason: %s\n", reason);
    Printf(PETSC_COMM_WORLD, "Iterations: %" PetscInt_FMT "\n", iterations);
    Printf(PETSC_COMM_WORLD, "Norm of error: %g\n", (double)error_norm);
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
