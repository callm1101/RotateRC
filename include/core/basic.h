#ifndef ROTATE_RC_BASIC_H_
#define ROTATE_RC_BASIC_H_

#include <fmt/core.h>

#include <Eigen/Dense>
#include <filesystem>
#include <limits>
#include <memory>
#include <numbers>

using Index = std::size_t;

inline constexpr double kPi = std::numbers::pi;

inline constexpr double kEpsilon = std::numeric_limits<double>::epsilon();

enum class ReadMode : Index { kText, kBinary };

enum class MaskMode : Index { kBool, kIndex };

enum class OutputMode : Index { kText, kBinary };

struct Xii {
  Index number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
  std::unique_ptr<Eigen::VectorXi> result_;
};

struct File {
  std::filesystem::path root_;
  std::filesystem::path grid_0_dir_;
  std::vector<std::filesystem::path> grid_copy_file_vec_;
  std::vector<std::filesystem::path> grid_rotate_file_vec_;
  std::filesystem::path x_ii_dir_;
  std::filesystem::path output_dir_;
  std::filesystem::path mask_output_dir_;
  std::filesystem::path test_input_dir_;
  std::filesystem::path test_output_dir_;
  std::filesystem::path test_output_file_;

  std::vector<std::filesystem::path> grid_rotate_output_dir_vec_;
};

std::filesystem::path gridRotateDir(int theta, const std::filesystem::path& grid_0_dir);

std::filesystem::path gridScaleDir(const std::filesystem::path& grid_0_dir);

std::filesystem::path xiiFile(Index i, const std::filesystem::path& x_ii_dir, ReadMode read_mode);

std::filesystem::path maskOutputFile(Index i, const std::filesystem::path& mask_output_dir);

std::filesystem::path maskOutputFile(Index i, int theta, const std::filesystem::path& mask_output_dir);

std::filesystem::path testInputFile(Index i, const std::filesystem::path& test_input_dir);

std::filesystem::path testInputFile(Index i, int theta, const std::filesystem::path& test_input_dir);

std::filesystem::path testOutputFile(int theta, const std::filesystem::path& test_output_dir);

#endif
