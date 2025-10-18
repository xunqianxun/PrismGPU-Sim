# Copilot Instructions for PrismSim

## Project Overview
- **PrismSim** integrates the Eigen C++ template library for linear algebra, including matrices, vectors, solvers, and tensor operations.
- Major code is organized under `src/` (project-specific code) and `eigen/` (third-party Eigen library).
- Eigen provides advanced tensor support (`unsupported/Eigen/CXX11/src/Tensor/`), with classes like `Tensor<data_type, rank>` for multidimensional arrays.

## Architecture & Key Components
- `src/`: Main simulation logic and custom project code (e.g., `GPC.hpp`, `Vram.hpp`).
- `eigen/`: Contains Eigen library source, benchmarks, BLAS/LAPACK wrappers, and documentation.
- Tensor operations are handled via Eigen's tensor classes, e.g., `Tensor<float, 3> t_3d(2, 3, 4);`.

## Developer Workflows
- **Build:**
  - Use CMake (`CMakeLists.txt` in `eigen/`) for configuration and building.
  - Build scripts and helpers are in `build/` (e.g., `buildtests.sh`, `check.sh`).
  - Example build command:
    ```bash
    cd build
    ./buildtests.sh
    ```
- **Testing:**
  - Tests are run via scripts in `build/` and CI jobs defined in `eigen/ci/README.md`.
  - CI stages: `checkformat` (clang-format), `build` (unit tests), `test` (run unit tests).
- **Debugging:**
  - Debug scripts are available in `build/debug.sh`.

## Project-Specific Conventions
- Eigen code is kept in `eigen/` and should not be modified unless contributing upstream.
- Project code in `src/` should use Eigen types for all linear algebra and tensor operations.
- Tensor classes require explicit rank and size parameters; resizing is done by assignment.
- Follow formatting conventions enforced by `clang-format` (see CI pipeline).

## Integration Points & Dependencies
- Eigen is the primary external dependency; BLAS/LAPACK integration is available via `eigen/blas/` and `eigen/lapack/`.
- Benchmarks and performance tests are in `eigen/bench/`.
- For advanced tensor operations, see `eigen/unsupported/Eigen/CXX11/src/Tensor/README.md`.

## Examples
- Creating a 3D tensor:
  ```cpp
  Tensor<float, 3> t_3d(2, 3, 4);
  t_3d = Tensor<float, 3>(3, 4, 3); // Resizing
  ```
- Running tests:
  ```bash
  cd build
  ./buildtests.sh
  ```

## References
- [Eigen Documentation](https://libeigen.gitlab.io/docs/)
- [Eigen GitLab](https://gitlab.com/libeigen/eigen)

---

**If any section is unclear or missing, please provide feedback for further refinement.**
