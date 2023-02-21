#ifndef ROTATE_RC_RAY_CASTING_H_
#define ROTATE_RC_RAY_CASTING_H_

#include <fmt/core.h>
#include <omp.h>

#include <Eigen/Dense>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include "core/basic.h"

namespace RayCasting {

enum class TriFlag : Index { kOnTri, kCrossTri, kOutTri, kOnCorner };

struct GridStructure {
  std::array<Index, 2> number_;
  std::unique_ptr<Eigen::Matrix3Xd> point_;
  std::unique_ptr<Eigen::Matrix3Xi> index_;
};

struct Box {
  std::unique_ptr<Eigen::Matrix<double, 6, Eigen::Dynamic>> axis_box_;
  std::unique_ptr<Eigen::Vector<double, 6>> object_box_;
};

void solve(const std::filesystem::path& grid_dir, const std::filesystem::path& x_ii_file,
           const std::filesystem::path& mask_output_file, ReadMode read_mode, MaskMode mask_mode);
void readGrid(const std::filesystem::path& grid_dir, std::vector<GridStructure>& grid_vec);
void readXii(const std::filesystem::path& x_ii_file, Xii& x_ii, ReadMode read_mode);
void calAABB(std::vector<GridStructure>& grid_vec, std::vector<Box>& box_vec);
TriFlag intersectTri(const Eigen::Vector3d& point, Eigen::Matrix3d& tri_coord);
bool castRay(const Eigen::Vector3d& point, GridStructure& grid, Box& box);
bool judgeSingularity(Index num, std::vector<Eigen::Index>& tri_index, const Eigen::Vector3d& point, GridStructure& grid,
                      Box& box);
bool detectIntersect(Eigen::Index tri1, Eigen::Index tri2, GridStructure& grid);
void outputMask(Xii& x_ii, const std::filesystem::path& mask_output_file);

}  // namespace RayCasting

#endif
