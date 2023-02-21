#ifndef ROTATE_RC_GRID_HANDLE_H_
#define ROTATE_RC_GRID_HANDLE_H_

#include <fmt/core.h>

#include <Eigen/Dense>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

#include "core/basic.h"

namespace GridHandle {

struct Rotation {
  int theta_;
  std::unique_ptr<Eigen::Vector3d> point_;
  std::unique_ptr<Eigen::Vector3d> vec_;
};

struct GridStructure {
  std::array<Index, 2> number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
  std::unique_ptr<Eigen::Matrix3Xd> handle_point_;
  std::unique_ptr<Eigen::Matrix4Xi> index_;
};

void solve(const std::filesystem::path& grid_handle_file, const std::filesystem::path& grid_handle_output_file,
           const Rotation& rotation);
void solve(double scale, const std::filesystem::path& grid_handle_file,
           const std::filesystem::path& grid_handle_output_file, ReadMode output_mode = ReadMode::kText, bool flag = true);
void readGrid(const std::filesystem::path& grid_handle_file, GridStructure& grid);
void translateGrid(const Rotation& rotation, GridStructure& grid);
void rotateGrid(const Rotation& rotation, GridStructure& grid);
void scaleGrid(double scale, GridStructure& grid);
void outputGrid(ReadMode output_mode, bool flag, const std::filesystem::path& grid_handle_output_file, GridStructure& grid);

}  // namespace GridHandle

#endif