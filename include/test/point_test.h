#ifndef ROTATE_RC_POINT_TEST_H_
#define ROTATE_RC_POINT_TEST_H_

#include <fmt/core.h>
#include <omp.h>

#include <Eigen/Dense>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "core/basic.h"

namespace PointTest {

struct Mask {
  std::shared_ptr<Eigen::VectorXi> out_num_;
  std::vector<std::unique_ptr<Eigen::Matrix3Xd>> out_points_;
};

void solve(Index x_ii_num, const std::filesystem::path& x_ii_dir, const std::filesystem::path& test_input_dir,
           const std::filesystem::path& test_output_file, ReadMode mode);
void solve(Index x_ii_num, int theta, const std::filesystem::path& x_ii_dir,
           const std::filesystem::path& test_input_dir, const std::filesystem::path& test_output_dir, ReadMode read_mode);
void readXii(const std::filesystem::path& x_ii_file, Xii& x_ii, ReadMode read_mode);
void readResult(const std::filesystem::path& test_input_file, Xii& x_ii);
void calPoints(Index i, Mask& mask, Xii& x_ii);
void outputPlt(Index num, Mask& mask, const std::filesystem::path& test_output_file);

}  // namespace PointTest

#endif