#ifndef ROTATE_RC_BASIC_H_
#define ROTATE_RC_BASIC_H_

#include <dbg.h>
#include <fmt/core.h>

#include <Eigen/Dense>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <string_view>
#include <vector>

#include "cmake.h"

using Index = std::size_t;

inline constexpr double kPi = std::numbers::pi;

inline constexpr double kEpsilon = std::numeric_limits<double>::epsilon();

enum class CoreMode : Index { kRayCasting, kShapeJudge };

enum class ReadMode : Index { kText, kBinary };

enum class MaskMode : Index { kBool, kIndex };

enum class OutputMode : Index { kText, kBinary };

struct Grid {
  std::array<Index, 2> number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
  std::unique_ptr<Eigen::Matrix4Xi> index_;
};

struct Xii {
  Index number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
};

struct Mask {
  Index number_;
  std::unique_ptr<Eigen::VectorXi> result_;
};

struct GridOut {
  std::unique_ptr<Eigen::VectorXi> out_num_;
  std::vector<std::unique_ptr<Eigen::Matrix3Xd>> out_points_;
};

struct GridHandle {
  Index number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
};

class File {
 public:
  const std::filesystem::path root_{kProjectSourceDir};
  const std::filesystem::path grid_0_dir_{kProjectSourceDir / "dat/grid/grid_0"};
  const std::filesystem::path x_ii_dir_{kProjectSourceDir / "dat/X_ii"};
  const std::filesystem::path output_dir_{kProjectSourceDir / "build/out"};
  const std::filesystem::path mask_output_dir_{kProjectSourceDir / "build/out/mask"};
  const std::filesystem::path test_input_dir_{kProjectSourceDir / "build/out/mask"};
  const std::filesystem::path test_output_file_{kProjectSourceDir / "build/out/test.plt"};

  File();
};

class HandleFile : public File {
 public:
  const std::filesystem::path grid_handle_dir_{kProjectSourceDir / "dat/grid/grid_handle"};

  HandleFile();
};

class RotateFile : public File {
 public:
  const std::filesystem::path test_output_dir_{kProjectSourceDir / "build/out/test"};

  std::vector<std::filesystem::path> grid_copy_file_vec_;
  std::vector<std::filesystem::path> grid_rotate_file_vec_;
  std::vector<std::filesystem::path> grid_rotate_output_dir_vec_;

  RotateFile();
};

#endif
