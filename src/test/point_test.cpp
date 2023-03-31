#include "test/point_test.h"

namespace PointTest {

void solve(const Index x_ii_num, const std::filesystem::path& x_ii_dir, const std::filesystem::path& test_input_dir,
           const std::filesystem::path& test_output_file, ReadMode read_mode) {
  GridOut grid_out;
  grid_out.out_num_ = std::make_unique<Eigen::VectorXi>(x_ii_num);
  grid_out.out_num_->setZero();
  std::cout << "Begin test.";
  for (Index i = 0; i < x_ii_num; i++) {
    Xii x_ii;
    Mask mask;
    std::filesystem::path x_ii_file = FileName::xiiInputFile(i, x_ii_dir, read_mode);
    std::filesystem::path test_input_file = FileName::maskOutputFile(i, test_input_dir);
    read(x_ii_file, test_input_file, x_ii, mask, read_mode);
    calPoints(i, grid_out, x_ii, mask);
  }
  std::cout << fmt::format("Output file: {}", test_output_file.filename().string()) << std::endl;
  TecIO::outputPlt(x_ii_num, grid_out, test_output_file);
}

void solve(const Index x_ii_num, const int theta, const std::filesystem::path& x_ii_dir,
           const std::filesystem::path& test_input_dir, const std::filesystem::path& test_output_dir,
           ReadMode read_mode) {
  GridOut grid_out;
  grid_out.out_num_ = std::make_unique<Eigen::VectorXi>(x_ii_num);
  grid_out.out_num_->setZero();
  std::cout << "Begin test.";
  for (Index i = 0; i < x_ii_num; i++) {
    Xii x_ii;
    Mask mask;
    std::filesystem::path x_ii_file = FileName::xiiInputFile(i, x_ii_dir, read_mode);
    std::filesystem::path test_input_file = FileName::maskOutputFile(i, theta, test_input_dir);
    read(x_ii_file, test_input_file, x_ii, mask, read_mode);
    calPoints(i, grid_out, x_ii, mask);
  }
  std::filesystem::path test_output_file = FileName::testOutputFile(theta, test_output_dir);
  std::cout << fmt::format("Output file: {}", test_output_file.filename().string()) << std::endl;
  TecIO::outputPlt(x_ii_num, grid_out, test_output_file);
}

void calPoints(const Index i, GridOut& grid_out, Xii& x_ii, Mask& mask) {
  int num = 0;
  for (Eigen::Index i = 0; i < static_cast<Eigen::Index>(mask.number_); i++) {
    if (mask.result_->operator()(i) > 0) {
      num++;
    }
  }
  grid_out.out_num_->operator()(static_cast<int>(i)) = num;
  std::unique_ptr<Eigen::Matrix3Xd> out_points = std::make_unique<Eigen::Matrix3Xd>(3, num);
  for (Eigen::Index i = 0, j = 0; j < static_cast<Eigen::Index>(x_ii.number_); j++) {
    if (mask.result_->operator()(j) > 0) {
      out_points->col(i++) << x_ii.point_->col(j);
    }
  }
  grid_out.out_points_.emplace_back(std::move(out_points));
}

}  // namespace PointTest
