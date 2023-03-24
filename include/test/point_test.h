#ifndef ROTATE_RC_POINT_TEST_H_
#define ROTATE_RC_POINT_TEST_H_

#include <fmt/core.h>
#include <omp.h>

#include <Eigen/Dense>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "basic/basic.h"
#include "basic/file.h"
#include "basic/io.h"

namespace PointTest {

void solve(Index x_ii_num, const std::filesystem::path& x_ii_dir, const std::filesystem::path& test_input_dir,
           const std::filesystem::path& test_output_file, ReadMode read_mode);
void solve(Index x_ii_num, int theta, const std::filesystem::path& x_ii_dir,
           const std::filesystem::path& test_input_dir, const std::filesystem::path& test_output_dir,
           ReadMode read_mode);
void calPoints(Index i, GridOut& grid_out, Xii& x_ii, Mask& mask);

}  // namespace PointTest

#endif
