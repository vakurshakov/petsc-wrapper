### Installation guide

#### 1. Install [PETSc](https://gitlab.com/petsc/petsc)
This is the short summary of PETSc installation process. To take an in-depth look on the configuring process, check the [documentation](https://petsc.org/release/install/install/) or run `./configure --help`. First, clone and update PETSc repository.
```console
git clone -b release https://gitlab.com/petsc/petsc.git ./external/petsc
git pull # obtain new release fixes (since a prior clone or pull)
```

Change the directory to `external/petsc/` and configure the library. The following code creates two configurations of the library with use of preinstalled MPI compilers and downloads just `BLAS/LAPACK` to the output directories.
```console
./configure PETSC_ARCH=linux-mpi-debug --with-fc=0 --with-cc=/usr/bin/mpicc --with-cxx=/usr/bin/mpicxx --with-mpiexec=/usr/bin/mpiexec --with-threadsafety=1 --with-openmp=1 --download-f2cblaslapack
make PETSC_ARCH=linux-mpi-debug all

./configure PETSC_ARCH=linux-mpi-opt --with-fc=0 --with-cc=/usr/bin/mpicc --with-cxx=/usr/bin/mpicxx --with-mpiexec=/usr/bin/mpiexec --with-threadsafety=1 --with-openmp=1 --download-f2cblaslapack --with-debugging=0 COPTFLAGS='-O3 -march=native -mtune=native' CXXOPTFLAGS='-O3 -march=native -mtune=native'
make PETSC_ARCH=linux-mpi-opt all
```

If configure cannot automatically download the package, you can use a pre-downloaded one. Once the tarfile is downloaded, the path to this file can be specified to configure and it will proceed to install this package and then configure PETSc with this package.

#### 2. Compiling and running `petsc-wrapper`

Now, the executable can be built successfully. To do so, \
run the following commands from the home directory:
```console
  make [-j]
```
